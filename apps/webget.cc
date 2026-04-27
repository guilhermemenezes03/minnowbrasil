#include "util/address.hh"
#include "util/address.cc"
#include "util/socket.hh"
#include "util/file_descriptor.hh"
#include "util/socket.cc"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

namespace {

string normalize_path( const string& path )
{
  if ( path.empty() ) {
    return "/";
  }

  if ( path.front() == '/' ) {
    return path;
  }

  return "/" + path;
}

} // namespace

string get_URL( const string& host, const string& path, const string& service = "80" )
{
  const string normalized_path = normalize_path( path );

// 1. Create an address for the web server.
  const auto address = minnow::Address address( host, service );;
// 2. Connect a TCP socket to that address.
  auto socket = minnow::TCPSocket::connect_to( address );
// 3. Send an HTTP request.
std::string request = "GET " + normalized_path + " HTTP/1.1\r\n"
                     + "Host: " + host + "\r\n"
                     + "Connection: close\r\n"
                     + "\r\n";
    socket.write_all( request );
// 4. Read the response until EOF.
auto response = string();

    while (true) {
        const auto chunk = socket.read_some();
        if (chunk.empty()) {
            break; // EOF reached
        }
        response += chunk;
    }

// 5. return the response.
    return response;

}

int main( int argc, char* argv[] )
{
  if ( argc != 3 && argc != 4 ) {
    cerr << "Usage: " << argv[0] << " HOST PATH [PORT]\n";
    return 1;
  }

  const string host = argv[1];
  const string path = argv[2];
  const string service = argc == 4 ? argv[3] : "80";

  try {
    cout << get_URL( host, path, service );
  } catch ( const exception& e ) {
    cerr << "webget: " << e.what() << '\n';
    return 1;
  }

  return 0;
}

