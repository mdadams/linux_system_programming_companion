cmake_minimum_required(VERSION 3.14)

project(slides_x86_64_pic LANGUAGES CXX C ASM-ATT)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")

option(ENABLE_EXTRAS false)

set(CMAKE_VERBOSE_MAKEFILE true)

# Note: The check_pie_supported function does not support the ASM-ATT language.
#include(CheckPIESupported)
#check_pie_supported()
#if(NOT CMAKE_ASM-ATT_LINK_PIE_SUPPORTED)
#	message(FATAL_ERROR "PIE is not supported\n")
#endif()
#set_property(TARGET target_name PROPERTY POSITION_INDEPENDENT_CODE true)

option(USE_FMTLIB "Use fmtlib for text formatting." false)
set(fmtlib_target "")
if(USE_FMTLIB)
	find_package(Fmt REQUIRED)
	list(APPEND fmtlib_target Fmt::Fmt)
	link_libraries(${fmtlib_target})
endif()

add_executable(no_pie no_pie.s)
# We desire static linking with no PIE.
set_property(TARGET no_pie APPEND_STRING PROPERTY
  LINK_FLAGS " -no-dynamic-linker")
set_property(TARGET no_pie APPEND_STRING PROPERTY
  LINK_FLAGS " -no-pie")
list(APPEND programs no_pie)

add_executable(pie pie.s)
# Use PIE with static linking.
set_property(TARGET pie APPEND_STRING PROPERTY LINK_FLAGS " -pie")
set_property(TARGET pie APPEND_STRING PROPERTY
  LINK_FLAGS " -no-dynamic-linker")
list(APPEND programs pie)

if(ENABLE_EXTRAS)
	add_executable(pie_dl pie.s)
	# Use PIE and dynamic linker (just for fun).
	set_property(TARGET pie_dl APPEND_STRING PROPERTY
	  LINK_FLAGS " -dynamic-linker=/usr/lib64/ld-linux-x86-64.so.2")
	set_property(TARGET pie_dl APPEND_STRING PROPERTY LINK_FLAGS " -pie")
	list(APPEND programs pie_dl)
endif()

configure_file("${CMAKE_SOURCE_DIR}/demo"
  "${CMAKE_BINARY_DIR}/demo" @ONLY)
add_custom_target(demo DEPENDS ${programs}
  COMMAND "${CMAKE_BINARY_DIR}/demo")
