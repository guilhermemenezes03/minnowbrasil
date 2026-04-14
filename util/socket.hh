#pragma once

#include "util/address.hh"
#include "util/file_descriptor.hh"

namespace minnow {

class Socket : public FileDescriptor {
public:
  using FileDescriptor::FileDescriptor;

  void connect( const Address& address );
};

class TCPSocket : public Socket {
public:
  [[nodiscard]] static TCPSocket connect_to( const Address& address );

private:
  explicit TCPSocket( int fd ) : Socket( fd ) {}
};

} // namespace minnow

