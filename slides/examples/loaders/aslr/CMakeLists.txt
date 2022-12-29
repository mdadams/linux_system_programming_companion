# Needs CMake 3.14 for CMP0083 NEW
cmake_minimum_required(VERSION 3.14)
project(slides_loaders_aslr_demo LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_VERBOSE_MAKEFILE true)

option(USE_FMTLIB "Use fmtlib for text formatting." false)
set(fmtlib_target "")
if(USE_FMTLIB)
	find_package(Fmt REQUIRED)
	list(APPEND fmtlib_target Fmt::Fmt)
	link_libraries(${fmtlib_target})
endif()

include(CheckPIESupported)
check_pie_supported()
if(NOT CMAKE_CXX_LINK_PIE_SUPPORTED)
	message(FATAL_ERROR "PIE is not supported\n")
endif()

add_executable(aslr_nopie aslr.cpp)
list(APPEND programs aslr_nopie)
set_property(TARGET aslr_nopie PROPERTY POSITION_INDEPENDENT_CODE false)
add_executable(aslr_pie aslr.cpp)
list(APPEND programs aslr_pie)
set_property(TARGET aslr_pie PROPERTY POSITION_INDEPENDENT_CODE true)

configure_file("${CMAKE_SOURCE_DIR}/demo"
  "${CMAKE_BINARY_DIR}/demo" @ONLY)
add_custom_target(demo
  DEPENDS ${programs}
  COMMAND "${CMAKE_BINARY_DIR}/demo"
)
