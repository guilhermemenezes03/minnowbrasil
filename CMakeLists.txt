cmake_minimum_required(VERSION 3.16)
project(cs144_selfstudy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include(CTest)
find_package(Python3 COMPONENTS Interpreter QUIET)

set(PROJECT_SOURCES
    util/file_descriptor.cc
    util/address.cc
    util/socket.cc
    src/byte_stream.cc)

set(PROJECT_HEADERS
    util/file_descriptor.hh
    util/address.hh
    util/socket.hh
    src/byte_stream.hh)

add_library(minnowish ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_include_directories(minnowish PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(webget apps/webget.cc)
target_link_libraries(webget PRIVATE minnowish)

if(BUILD_TESTING)
  add_executable(test_byte_stream tests/test_byte_stream.cc)
  target_link_libraries(test_byte_stream PRIVATE minnowish)

  add_test(NAME t_byte_stream COMMAND test_byte_stream)

  if(Python3_Interpreter_FOUND)
    add_test(
      NAME t_webget
      COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_webget.py $<TARGET_FILE:webget>)
  endif()
endif()

add_custom_target(check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS webget test_byte_stream)

add_custom_target(check_webget
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R t_webget
  DEPENDS webget)

add_custom_target(check0
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "t_byte_stream|t_webget"
  DEPENDS webget test_byte_stream)

find_program(CLANG_FORMAT NAMES clang-format)
if(CLANG_FORMAT)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT} -i
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
            apps/webget.cc
            tests/test_byte_stream.cc)
else()
  add_custom_target(format
    COMMAND ${CMAKE_COMMAND} -E echo "clang-format nao encontrado. Instale-o na VM para usar este target.")
endif()

find_program(CLANG_TIDY NAMES clang-tidy)
if(CLANG_TIDY)
  add_custom_target(tidy
    COMMAND ${CLANG_TIDY}
            apps/webget.cc
            src/byte_stream.cc
            src/byte_stream.hh
            util/address.cc
            util/socket.cc
            util/file_descriptor.cc
            --
            -std=c++20
            -I${CMAKE_CURRENT_SOURCE_DIR})
else()
  add_custom_target(tidy
    COMMAND ${CMAKE_COMMAND} -E echo "clang-tidy nao encontrado. Instale-o na VM para usar este target.")
endif()

