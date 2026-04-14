#pragma once

#include <string>
#include <string_view>

#include <sys/socket.h>

namespace minnow {

class Address {
public:
  Address( std::string_view host, std::string_view service );

  [[nodiscard]] const sockaddr* sockaddr_ptr() const;
  [[nodiscard]] socklen_t size() const;
  [[nodiscard]] int family() const;
  [[nodiscard]] int socktype() const;
  [[nodiscard]] int protocol() const;
  [[nodiscard]] std::string to_string() const;

private:
  sockaddr_storage storage_ {};
  socklen_t size_ { 0 };
  int family_ { AF_UNSPEC };
  int socktype_ { SOCK_STREAM };
  int protocol_ { 0 };
  std::string host_ {};
  std::string service_ {};
};

} // namespace minnow

