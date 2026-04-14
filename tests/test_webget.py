#!/usr/bin/env python3

import socket
import subprocess
import sys
import threading


EXPECTED_RESPONSE = (
    "HTTP/1.1 200 OK\r\n"
    "Content-Length: 20\r\n"
    "Content-Type: text/plain\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello from fixture!\n"
)


def serve_one(server_socket, error_holder):
    try:
        connection, _ = server_socket.accept()
        with connection:
            request = b""
            while b"\r\n\r\n" not in request:
                chunk = connection.recv(4096)
                if not chunk:
                    break
                request += chunk

            request_text = request.decode("latin1")
            required_lines = [
                "GET /hello HTTP/1.1\r\n",
                "Host: 127.0.0.1\r\n",
                "Connection: close\r\n",
            ]

            for line in required_lines:
                if line not in request_text:
                    raise AssertionError(f"request missing line: {line!r}\nrequest was:\n{request_text}")

            connection.sendall(EXPECTED_RESPONSE.encode("latin1"))
    except Exception as exc:  # pragma: no cover - diagnostic path
        error_holder.append(exc)


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: test_webget.py /path/to/webget", file=sys.stderr)
        return 2

    webget_path = sys.argv[1]
    server_errors = []

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind(("127.0.0.1", 0))
        server_socket.listen(1)
        port = server_socket.getsockname()[1]

        thread = threading.Thread(target=serve_one, args=(server_socket, server_errors), daemon=True)
        thread.start()

        result = subprocess.run(
            [webget_path, "127.0.0.1", "/hello", str(port)],
            capture_output=True,
            text=True,
            check=False,
        )

        thread.join(timeout=5)

    if server_errors:
        print(server_errors[0], file=sys.stderr)
        return 1

    if result.returncode != 0:
        print(result.stderr, file=sys.stderr)
        return 1

    if result.stdout != EXPECTED_RESPONSE:
        print("webget output mismatch", file=sys.stderr)
        print("expected:", file=sys.stderr)
        print(EXPECTED_RESPONSE, file=sys.stderr)
        print("got:", file=sys.stderr)
        print(result.stdout, file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

