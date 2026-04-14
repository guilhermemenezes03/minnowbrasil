#include "util/socket.hh"

#include <cerrno>
#include <system_error>

#include <sys/socket.h>

using namespace std;

namespace minnow {

namespace {

system_error make_system_error( const char* what )
{
  return system_error( errno, generic_category(), what );
}

} // namespace

void Socket::connect( const Address& address )
{
  if ( ::connect( fd(), address.sockaddr_ptr(), address.size() ) < 0 ) {
    throw make_system_error( "connect" );
  }
}

TCPSocket TCPSocket::connect_to( const Address& address )
{
  const auto socket_fd = ::socket( address.family(), address.socktype(), address.protocol() );

  if ( socket_fd < 0 ) {
    throw make_system_error( "socket" );
  }

  TCPSocket socket( socket_fd );
  socket.connect( address );
  return socket;
}

} // namespace minnow

