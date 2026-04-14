#include "util/file_descriptor.hh"

#include <cerrno>
#include <stdexcept>
#include <system_error>
#include <utility>
#include <vector>

#include <unistd.h>

using namespace std;

namespace minnow {

namespace {

system_error make_system_error( const char* what )
{
  return system_error( errno, generic_category(), what );
}

} // namespace

FileDescriptor::FileDescriptor( const int fd ) : fd_( fd )
{
  if ( fd_ < 0 ) {
    throw invalid_argument( "FileDescriptor requires a valid file descriptor" );
  }
}

FileDescriptor::FileDescriptor( FileDescriptor&& other ) noexcept : fd_( exchange( other.fd_, -1 ) ) {}

FileDescriptor& FileDescriptor::operator=( FileDescriptor&& other ) noexcept
{
  if ( this != &other ) {
    if ( valid() ) {
      ::close( fd_ );
    }

    fd_ = exchange( other.fd_, -1 );
  }

  return *this;
}

FileDescriptor::~FileDescriptor()
{
  if ( valid() ) {
    ::close( fd_ );
  }
}

int FileDescriptor::fd() const
{
  return fd_;
}

bool FileDescriptor::valid() const
{
  return fd_ >= 0;
}

void FileDescriptor::close()
{
  if ( !valid() ) {
    return;
  }

  if ( ::close( fd_ ) < 0 ) {
    throw make_system_error( "close" );
  }

  fd_ = -1;
}

void FileDescriptor::write_all( string_view data ) const
{
  size_t written_total = 0;

  while ( written_total < data.size() ) {
    const auto written = ::write( fd_, data.data() + written_total, data.size() - written_total );

    if ( written < 0 ) {
      if ( errno == EINTR ) {
        continue;
      }

      throw make_system_error( "write" );
    }

    if ( written == 0 ) {
      throw runtime_error( "write returned 0 before all bytes were sent" );
    }

    written_total += static_cast<size_t>( written );
  }
}

string FileDescriptor::read_some( const size_t max_bytes ) const
{
  vector<char> buffer( max_bytes );

  while ( true ) {
    const auto bytes_read = ::read( fd_, buffer.data(), buffer.size() );

    if ( bytes_read < 0 ) {
      if ( errno == EINTR ) {
        continue;
      }

      throw make_system_error( "read" );
    }

    return string( buffer.data(), static_cast<size_t>( bytes_read ) );
  }
}

} // namespace minnow

