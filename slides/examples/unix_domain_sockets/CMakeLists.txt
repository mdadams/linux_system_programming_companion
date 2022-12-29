cmake_minimum_required(VERSION 3.14)

project(slides_unix_domain_sockets LANGUAGES CXX C)

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

add_executable(passing_descriptors_1 passing_descriptors_1.cpp)
add_executable(dgram_server dgram_server.cpp)
add_executable(dgram_client dgram_client.cpp)
add_executable(stream_server stream_server.cpp)
add_executable(stream_client stream_client.cpp)
add_executable(stream_server_2 stream_server_2.cpp)
add_executable(stream_client_2 stream_client_2.cpp)

add_custom_target(demo)
