cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(slides_examples LANGUAGES CXX)

include(ExternalProject)

option(USE_FMTLIB "Use fmtlib for text formatting." false)

################################################################################

if(USE_FMTLIB)
	ExternalProject_Add(fmt
	  SOURCE_DIR ${CMAKE_BINARY_DIR}/fmt-source
	  BINARY_DIR ${CMAKE_BINARY_DIR}/fmt
	  #INSTALL_DIR "${CMAKE_BINARY_DIR}/fmt-install"
	  INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
	  GIT_REPOSITORY https://github.com/mdadams/sde.git
	  #GIT_TAG        v5.0.1
	  GIT_TAG        5d564f0eb372b66ecacd909e0e483ec82b8c1e6f
	  CONFIGURE_COMMAND ""
	  BUILD_COMMAND ""
	  UPDATE_COMMAND ""
	  INSTALL_COMMAND
	  <SOURCE_DIR>/bin/sde_install_fmtlib
	  -x
	  -y
	  -v 9.1.0
	  -d <INSTALL_DIR>
	  )
endif()

################################################################################

list(APPEND dirs
  args
  basics
  capabilities
  futexes
  loaders/aslr
  hello
  loaders/libgreet
  processes
  ptrace
  seccomp
  signals
  unix_domain_sockets
  virtual_memory
  x86_64/general
  x86_64/pic
  files
  unused
  #experimental
  #loaders/experimental
  #pic_experimental
)

set(cmake_args "")
list(APPEND cmake_args "-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
list(APPEND cmake_args "-DUSE_FMTLIB=${USE_FMTLIB}")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
	list(APPEND cmake_args
	  "-DCMAKE_EXPORT_COMPILE_COMMANDS=${CMAKE_EXPORT_COMPILE_COMMANDS}")
endif()
list(APPEND cmake_args "-DCMAKE_EXPORT_COMPILE_COMMANDS=true")
if(CMAKE_CXX_COMPILER)
	list(APPEND cmake_args "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
endif()
if(CMAKE_C_COMPILER)
	list(APPEND cmake_args "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
endif()

message("cmake_args ${cmake_args}")

add_custom_target(demo)

# Add each project as an external project.
foreach(dir IN LISTS dirs)

	# Skip the processing of the directory if it does not exist.
	if(NOT EXISTS "${CMAKE_SOURCE_DIR}/${dir}")
		message(WARNING "Skipping directory ${dir}")
		continue()
	endif()

	# Set target name to directory name with any
	# spaces changed to underscores.
	string(REPLACE " " "_" target "${dir}")
	string(REPLACE "/" "__" target "${dir}")

	# Add external project.
	externalproject_add("${target}"
	  SOURCE_DIR "${CMAKE_SOURCE_DIR}/${dir}"
	  BINARY_DIR "${CMAKE_BINARY_DIR}/${target}"
	  STEP_TARGETS configure build install demo
	  CMAKE_ARGS ${cmake_args}
	  INSTALL_COMMAND "")

	ExternalProject_Add_Step("${target}" demo
	  ALWAYS TRUE
	  DEPENDEES build
	  EXCLUDE_FROM_MAIN TRUE
	  EXCLUDE_FROM_ALL TRUE
	  WORKING_DIRECTORY <BINARY_DIR>
	  COMMENT "Running demo"
	  COMMAND echo ==================================================
	  COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config <CONFIG> --target demo
	  COMMAND echo ==================================================
	  )

	add_dependencies(demo ${target}-demo)
	add_dependencies(${target} fmt)

endforeach()
