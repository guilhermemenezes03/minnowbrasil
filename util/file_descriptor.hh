#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace minnow {

class FileDescriptor {
public:
  explicit FileDescriptor( int fd );

  FileDescriptor( const FileDescriptor& ) = delete;
  FileDescriptor& operator=( const FileDescriptor& ) = delete;

  FileDescriptor( FileDescriptor&& other ) noexcept;
  FileDescriptor& operator=( FileDescriptor&& other ) noexcept;

  ~FileDescriptor();

  [[nodiscard]] int fd() const;
  [[nodiscard]] bool valid() const;

  void close();
  void write_all( std::string_view data ) const;
  [[nodiscard]] std::string read_some( std::size_t max_bytes = 4096 ) const;

protected:
  FileDescriptor() = default;

private:
  int fd_ { -1 };
};

} // namespace minnow

