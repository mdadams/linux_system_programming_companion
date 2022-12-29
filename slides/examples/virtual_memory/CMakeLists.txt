cmake_minimum_required(VERSION 3.14)

project(slides_virtual_memory LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

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

add_executable(mmap_1 mmap_1.cpp)
list(APPEND programs mmap_1)

add_executable(mmap_2 mmap_2.cpp)
list(APPEND programs mmap_2)

add_executable(checksum_1 checksum_1.cpp)
list(APPEND programs checksum_1)

add_executable(crash_1 crash_1.cpp)
list(APPEND programs crash_1)

add_executable(mmap_allocator_main mmap_allocator_main.cpp)
list(APPEND programs mmap_allocator_main)

add_executable(mprotect_1 mprotect_1.cpp)
list(APPEND programs mprotect_1)

configure_file("${CMAKE_SOURCE_DIR}/demo"
  "${CMAKE_BINARY_DIR}/demo" @ONLY)
add_custom_target(demo
  DEPENDS ${programs}
  COMMAND "${CMAKE_BINARY_DIR}/demo"
)
