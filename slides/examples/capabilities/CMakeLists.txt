cmake_minimum_required(VERSION 3.14)

project(slides_capabilities LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

option(ENABLE_EXTRAS "Enable extras" FALSE)

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

find_library(CAP_LIBRARY cap REQUIRED)

link_libraries(${CAP_LIBRARY})
add_executable(getfcap_1 getfcap_1.cpp)
list(APPEND programs getfcap_1)
add_executable(getpcap_1 getpcap_1.cpp)
list(APPEND programs getpcap_1)
if(ENABLE_EXTRAS)
	add_executable(setpcap_1 setpcap_1.cpp)
endif()

configure_file("${CMAKE_SOURCE_DIR}/demo"
  "${CMAKE_BINARY_DIR}/demo" @ONLY)
add_custom_target(demo
  DEPENDS ${programs}
  COMMAND "${CMAKE_BINARY_DIR}/demo"
)
