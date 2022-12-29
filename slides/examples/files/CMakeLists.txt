cmake_minimum_required(VERSION 3.14)

project(slides_files LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

option(ENABLE_EXTRAS "Enable extras." FALSE)

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(USE_FMTLIB "Use fmtlib for text formatting." false)
set(fmtlib_target "")
if(USE_FMTLIB)
	find_package(Fmt REQUIRED)
	list(APPEND fmtlib_target Fmt::Fmt)
	link_libraries(${fmtlib_target})
endif()

add_executable(copy_file_1 copy_file_1.cpp)
list(APPEND programs copy_file_1)

add_executable(file_1 file_1.cpp)
list(APPEND programs file_1)

add_executable(file_2 file_2.cpp)
list(APPEND programs file_2)

add_executable(file_3 file_3.cpp)
list(APPEND programs file_3)

add_executable(io_redirection_1 io_redirection_1.cpp)
list(APPEND programs io_redirection_1)

add_executable(pipe_1 pipe_1.cpp)
list(APPEND programs pipe_1)

if(ENABLE_EXTRAS)
	add_executable(pipe_2 pipe_2.cpp)
	list(APPEND programs pipe_2)
endif()

add_executable(unique_fd_1 unique_fd_1.cpp)
list(APPEND programs unique_fd_1)

add_executable(vectored_io_1_a vectored_io_1_a.cpp)
list(APPEND programs vectored_io_1_a)

add_executable(vectored_io_1_b vectored_io_1_b.cpp)
list(APPEND programs vectored_io_1_b)

configure_file("${CMAKE_SOURCE_DIR}/demo"
  "${CMAKE_BINARY_DIR}/demo" @ONLY)
add_custom_target(demo
  DEPENDS ${programs}
  COMMAND "${CMAKE_BINARY_DIR}/demo"
)
