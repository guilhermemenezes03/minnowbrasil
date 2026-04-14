#include "util/address.hh"

#include <cstring>
#include <stdexcept>
#include <string>

#include <netdb.h>

using namespace std;

namespace minnow {

Address::Address( string_view host, string_view service ) : host_( host ), service_( service )
{
  addrinfo hints {};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo* results = nullptr;
  const auto status = ::getaddrinfo( host_.c_str(), service_.c_str(), &hints, &results );

  if ( status != 0 ) {
    throw runtime_error( string( "getaddrinfo: " ) + ::gai_strerror( status ) );
  }

  const addrinfo* choice = results;

  if ( choice == nullptr ) {
    ::freeaddrinfo( results );
    throw runtime_error( "getaddrinfo returned no addresses" );
  }

  if ( choice->ai_addrlen > sizeof( storage_ ) ) {
    ::freeaddrinfo( results );
    throw runtime_error( "sockaddr_storage too small for resolved address" );
  }

  memcpy( &storage_, choice->ai_addr, choice->ai_addrlen );
  size_ = static_cast<socklen_t>( choice->ai_addrlen );
  family_ = choice->ai_family;
  socktype_ = choice->ai_socktype;
  protocol_ = choice->ai_protocol;

  ::freeaddrinfo( results );
}

const sockaddr* Address::sockaddr_ptr() const
{
  return reinterpret_cast<const sockaddr*>( &storage_ );
}

socklen_t Address::size() const
{
  return size_;
}

int Address::family() const
{
  return family_;
}

int Address::socktype() const
{
  return socktype_;
}

int Address::protocol() const
{
  return protocol_;
}

string Address::to_string() const
{
  return host_ + ":" + service_;
}

} // namespace minnow

