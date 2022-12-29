cmake_minimum_required(VERSION 3.14)

project(slides_loaders_loaders_libgreet LANGUAGES CXX)

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

add_library(greet_static STATIC greet.cpp)
set_target_properties(greet_static PROPERTIES OUTPUT_NAME greet)

add_library(greet_shared SHARED greet.cpp)
set_target_properties(greet_shared PROPERTIES OUTPUT_NAME greet)

add_executable(app_static app.cpp)
target_link_libraries(app_static PUBLIC greet_static)

add_executable(app_shared app.cpp)
target_link_libraries(app_shared PUBLIC greet_shared)

add_executable(app_dl app_dl.cpp)
target_link_libraries(app_dl PUBLIC dl)

configure_file("${CMAKE_SOURCE_DIR}/run_app_dl"
  "${CMAKE_BINARY_DIR}/run_app_dl" @ONLY)
add_custom_target(demo DEPENDS app_dl
  COMMAND "${CMAKE_BINARY_DIR}/run_app_dl")
