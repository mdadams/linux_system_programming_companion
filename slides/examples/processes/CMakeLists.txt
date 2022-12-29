cmake_minimum_required(VERSION 3.14)

project(slides_processes LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
#find_package(Boost REQUIRED COMPONENTS thread)

option(USE_FMTLIB "Use fmtlib for text formatting." false)
set(fmtlib_target "")
if(USE_FMTLIB)
	find_package(Fmt REQUIRED)
	list(APPEND fmtlib_target Fmt::Fmt)
	link_libraries(${fmtlib_target})
endif()

add_executable(exec_1 exec_1.cpp)
list(APPEND programs exec_1)

add_executable(fork_1 fork_1.cpp)
list(APPEND programs fork_1)

add_executable(fork_exec_1 fork_exec_1.cpp)
list(APPEND programs fork_exec_1)

add_executable(fork_wait_1 fork_wait_1.cpp)
list(APPEND programs fork_wait_1)

add_executable(fork_wait_2 fork_wait_2.cpp)
list(APPEND programs fork_wait_2)

add_executable(clone_1 clone_1.cpp)
list(APPEND programs clone_1)

add_executable(get_process_ids get_process_ids.cpp)
list(APPEND programs get_process_ids)

if(ENABLE_EXTRAS)
	add_executable(test_1 test_1.cpp)
	list(APPEND programs test_1)
	target_link_libraries(test_1 Threads::Threads Boost::thread)
endif()

configure_file("${CMAKE_SOURCE_DIR}/demo"
  "${CMAKE_BINARY_DIR}/demo" @ONLY)
add_custom_target(demo
  DEPENDS ${programs}
  COMMAND "${CMAKE_BINARY_DIR}/demo"
)
