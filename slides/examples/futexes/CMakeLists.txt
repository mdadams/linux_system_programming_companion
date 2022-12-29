cmake_minimum_required(VERSION 3.14)

project(slides_futexes LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(ENABLE_EXTRAS "Enable extra code." false)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)

option(USE_FMTLIB "Use fmtlib for text formatting." false)
set(fmtlib_target "")
if(USE_FMTLIB)
	find_package(Fmt REQUIRED)
	list(APPEND fmtlib_target Fmt::Fmt)
	link_libraries(${fmtlib_target})
endif()

link_libraries(Threads::Threads)
link_libraries(Boost::thread)

if(ENABLE_EXTRAS)
	#add_executable(spinlock_test spinlock_test.cpp)
	#add_executable(condvar2_test mutex2.cpp condvar2_test.cpp)
	add_executable(condvar1_test mutex1.cpp condvar_test.cpp)

	add_executable(ticketlock2_test ticketlock2_test.cpp)
	list(APPEND mutex_program_targets ticketlock2_test)
endif()

add_executable(spinlock1_app spinlock1_app.cpp)
add_executable(barrier_app barrier_app.cpp barrier.cpp mutex2.cpp)

add_executable(spinlock1_test spinlock1_test.cpp)
list(APPEND mutex_program_targets spinlock1_test)

add_executable(spinlock2_test spinlock2_test.cpp)
list(APPEND mutex_program_targets spinlock2_test)

add_executable(mutex1_test mutex1.cpp mutex1_test.cpp)
list(APPEND mutex_program_targets mutex1_test)

add_executable(mutex2_test mutex2.cpp mutex2_test.cpp)
list(APPEND mutex_program_targets mutex2_test)

add_executable(ticketlock1_test ticketlock1_test.cpp)
list(APPEND mutex_program_targets ticketlock1_test)

configure_file("${CMAKE_SOURCE_DIR}/mutex_demo"
  "${CMAKE_BINARY_DIR}/mutex_demo" @ONLY)
add_custom_target(demo
  DEPENDS ${mutex_program_targets}
  COMMAND
  "${CMAKE_CURRENT_SOURCE_DIR}/mutex_demo"
  "${CMAKE_CURRENT_BINARY_DIR}"
)
