#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace minnow {

class ByteStream {
public:
  explicit ByteStream( uint64_t capacity );

  void push( std::string data );
  void close();

  [[nodiscard]] bool is_closed() const;
  [[nodiscard]] uint64_t available_capacity() const;
  [[nodiscard]] uint64_t bytes_pushed() const;

  [[nodiscard]] std::string_view peek() const;
  void pop( uint64_t len );

  [[nodiscard]] bool is_finished() const;
  [[nodiscard]] bool has_error() const;
  [[nodiscard]] uint64_t bytes_buffered() const;
  [[nodiscard]] uint64_t bytes_popped() const;

private:
  void compact();

  uint64_t capacity_ { 0 };
  bool closed_ { false };
  bool error_ { false };
  uint64_t bytes_pushed_ { 0 };
  uint64_t bytes_popped_ { 0 };
  std::string storage_ {};
  std::size_t head_ { 0 };
};

} // namespace minnow

