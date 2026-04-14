#include "src/byte_stream.hh"

#include <stdexcept>
#include <string>

using namespace std;
using minnow::ByteStream;

namespace {

void expect( const bool condition, const string& message )
{
  if ( !condition ) {
    throw runtime_error( message );
  }
}

void test_capacity_and_peek()
{
  ByteStream stream( 5 );

  expect( stream.available_capacity() == 5, "new stream should expose full capacity" );
  expect( stream.bytes_buffered() == 0, "new stream should start empty" );

  stream.push( "hello" );

  expect( stream.peek() == "hello", "peek should show pushed bytes" );
  expect( stream.bytes_pushed() == 5, "bytes_pushed should count accepted bytes" );
  expect( stream.bytes_buffered() == 5, "all accepted bytes should remain buffered" );
  expect( stream.available_capacity() == 0, "capacity should be exhausted after filling buffer" );
}

void test_partial_push_and_sliding_window()
{
  ByteStream stream( 3 );

  stream.push( "abcd" );
  expect( stream.peek() == "abc", "push should respect capacity limit" );
  expect( stream.bytes_pushed() == 3, "only bytes that fit should be counted as pushed" );

  stream.pop( 2 );
  expect( stream.peek() == "c", "pop should advance the readable prefix" );
  expect( stream.bytes_popped() == 2, "bytes_popped should track removed bytes" );
  expect( stream.available_capacity() == 2, "popping should reopen capacity" );

  stream.push( "XYZ" );
  expect( stream.peek() == "cXY", "stream should accept new bytes after older bytes are popped" );
  expect( stream.bytes_pushed() == 5, "historical pushed count should include later accepted bytes" );
}

void test_close_and_finish()
{
  ByteStream stream( 4 );

  stream.push( "data" );
  stream.close();

  expect( stream.is_closed(), "close should mark the stream as closed" );
  expect( !stream.is_finished(), "closed stream with unread data is not finished yet" );

  stream.pop( 4 );
  expect( stream.is_finished(), "stream is finished when closed and fully popped" );
}

} // namespace

int main()
{
  test_capacity_and_peek();
  test_partial_push_and_sliding_window();
  test_close_and_finish();
  return 0;
}

