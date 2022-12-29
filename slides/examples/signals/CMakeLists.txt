cmake_minimum_required(VERSION 3.14)

project(slides_signals LANGUAGES CXX C ASM-ATT)

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

add_executable(sigill_main sigill_main.cpp sigill_x86.s)
list(APPEND programs sigill_main)
add_executable(sigsegv_main sigsegv_main.cpp sigsegv_x86.s)
list(APPEND programs sigsegv_main)
add_executable(breakpoint_main breakpoint_main.cpp breakpoint_x86.s)
list(APPEND programs breakpoint_main)
add_executable(kill_1 kill_1.cpp)
list(APPEND programs kill_1)
add_executable(signal_1 signal_1.cpp)
list(APPEND programs signal_1)

configure_file("${CMAKE_SOURCE_DIR}/demo"
  "${CMAKE_BINARY_DIR}/demo" @ONLY)
add_custom_target(demo
  DEPENDS ${programs}
  COMMAND "${CMAKE_BINARY_DIR}/demo"
)
