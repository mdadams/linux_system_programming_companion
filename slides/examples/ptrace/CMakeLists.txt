cmake_minimum_required(VERSION 3.14)

project(slides_ptrace LANGUAGES CXX C)

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

add_custom_target(syscall_names
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  COMMAND bash -c "${CMAKE_CURRENT_SOURCE_DIR}/make_syscall_names > ${CMAKE_CURRENT_BINARY_DIR}/syscall_names.hpp"
  VERBATIM
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
add_executable(ptrace_1 ptrace_1.cpp)
add_dependencies(ptrace_1 syscall_names)
list(APPEND programs ptrace_1)

add_executable(ptrace_2 ptrace_2.cpp)
list(APPEND programs ptrace_2)

add_executable(ptrace_3 ptrace_3.cpp)
list(APPEND programs ptrace_3)

configure_file("${CMAKE_SOURCE_DIR}/demo"
  "${CMAKE_BINARY_DIR}/demo" @ONLY)
add_custom_target(demo
  DEPENDS ${programs}
  COMMAND "${CMAKE_BINARY_DIR}/demo"
)
