#include "src/byte_stream.hh"

#include <algorithm>
#include <stdexcept>

using namespace std;

namespace minnow {

ByteStream::ByteStream( const uint64_t capacity ) : capacity_( capacity ) {}

void ByteStream::compact()
{
  if ( head_ == 0 ) {
    return;
  }

  storage_.erase( 0, head_ );
  head_ = 0;
}

void ByteStream::push( string data )
{
  if ( closed_ && !data.empty() ) {
    throw logic_error( "cannot push into a closed ByteStream" );
  }
  if ( available_capacity() == 0 ) {
    return; // No more capacity to accept new data.
  } else if ( data.size() > available_capacity() ) {
    data.resize( available_capacity() ); // Truncate data to fit available capacity.
   }
    if ( head_ > 0 ) {
      compact(); // Compact storage before appending new data.
    }
    storage_ += data; // Append new data to storage.
    bytes_pushed_ = bytes_pushed_ + data.size();
}

void ByteStream::close()
{
  closed_ = true;
}

bool ByteStream::is_closed() const
{
  return closed_;
}

uint64_t ByteStream::available_capacity() const
{
  return capacity_ - bytes_buffered();
}

uint64_t ByteStream::bytes_pushed() const
{
  return bytes_pushed_;
}

string_view ByteStream::peek() const
{
  if ( bytes_buffered() == 0 ) {
    return {};
  }
    return string_view( storage_ ).substr( head_ );
}

void ByteStream::pop( const uint64_t len )
{
    const uint64_t amount = min( len, bytes_buffered());
    head_ += amount; // Move head forward by amount bytes.
    bytes_popped_ += amount; // Update total bytes popped.
    if ( head_ >= storage_.size() ) {
      storage_.clear(); // Clear storage when all buffered data has been popped.
      head_ = 0; // Reset head to the beginning of the storage.
    } else if ( head_ > storage_.size() / 2 ) {
      compact(); // Compact storage if head has moved past half of the current size.
    }
  }

bool ByteStream::is_finished() const
{
  return closed_ && bytes_buffered() == 0;
}

bool ByteStream::has_error() const
{
  return error_;
}

uint64_t ByteStream::bytes_buffered() const
{
  return static_cast<uint64_t>( storage_.size() - head_ );
}

uint64_t ByteStream::bytes_popped() const
{
  return bytes_popped_;
}

} // namespace minnow

