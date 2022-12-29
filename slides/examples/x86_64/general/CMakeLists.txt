cmake_minimum_required(VERSION 3.14)

project(slides_x86_64_general LANGUAGES CXX C ASM-ATT)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")

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

# This is only needed if the language ASM is used.
#set(CMAKE_ASM_LINK_EXECUTABLE
#  "ld <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# Note: This is not actually intended to be a runnable program.
add_executable(breakpoint_single_byte_opcode_1
  breakpoint_single_byte_opcode_1.s)

add_executable(addints_test_1 addints_test.cpp addints_1.s addints_2.s)

add_executable(factorial_test_0 factorial_test.cpp factorial_0.cpp)
add_executable(factorial_test_1 factorial_test.cpp factorial_1.s)

add_executable(hamming_weight_test_0
  hamming_weight_test.cpp hamming_weight_0.cpp)
add_executable(hamming_weight_test_1
  hamming_weight_test.cpp hamming_weight_1.s)

add_executable(hello_1 hello_1.s)
add_executable(hello_2 hello_2.s)

add_executable(hello_world_0 hello_world_0.cpp)

add_executable(max_test_0 max_test.cpp max_0.cpp)
add_executable(max_test_1 max_test.cpp max_1.s)

add_executable(true_0 true_0.cpp)
add_executable(true_1 true_1.s)

add_custom_target(demo)
