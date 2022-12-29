cmake_minimum_required(VERSION 3.14)

project(slides_hello LANGUAGES CXX C)

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

set(programs "")

add_executable(hello_1 hello_1.c)
list(APPEND programs hello_1)

add_executable(hello_2 hello_2.cpp)
list(APPEND programs hello_2)

configure_file("${CMAKE_SOURCE_DIR}/demo"
  "${CMAKE_BINARY_DIR}/demo" @ONLY)
add_custom_target(demo
  DEPENDS ${programs}
  COMMAND
  "${CMAKE_CURRENT_BINARY_DIR}/demo"
)
