Companion Git Repository for "Lecture Slides for Linux System Programming"
==========================================================================

This repository contains all of the code examples that are associated
with the following slide deck:

  - Michael D. Adams.
    Lecture Slides for Linux System Programming.
    Edition 0.0,
    Dec. 2022.

Obtaining the Slide Deck
------------------------

The most recent version of the slide deck is available for download at:

  - <https://www.ece.uvic.ca/~mdadams/cppbook>

A direct link to the slide deck in PDF format is:

  - <https://www.ece.uvic.ca/~mdadams/cppbook/downloads/lecture_slides_for_linux_system_programming-0.0.pdf>

Organization of Repository
--------------------------

The repository is organized as follows:

- slides/examples
  - This directory hierarchy contains all of the code examples from the
    slide deck.

Each code example or group of code examples is structured as a separate
CMake project.

Prerequisites to Building the Software
--------------------------------------

The code examples have the following software dependencies:

  - CMake
  - GCC or Clang (for C++ and C compilers)
  - Make
  - Boost
  - seccomp library
  - cap library
  - ausyscall program
  - numerous basic Unix utilities such as awk, grep, and so on
    (which should be included in any reasonable Linux distribution)

These dependencies must be installed prior to building the code examples.

If the C++ standard library does not support std::format, a customized
version of the fmt library can be automatically installed (as part of
the build process) to provide this support.  The C++ standard library
included with version 13 (and above) of GCC has support for std::format.

The seccomp and cap libraries are likely to be packaged for all reasonable
Linux distributions.  In some versions of Fedora Linux, for example, the
seccomp and cap libraries are provided by the packages:

  - libseccomp-devel
  - libcap-devel

The ausyscall program is part of a collection of userspace tools for
kernel auditing.  Some Linux distributions include this program in
a package with a name resembling "audit" or "auditd".

Building the Software
---------------------

The code examples employ a CMake-based build process.  Each code example
or group of related examples is structured as a separate CMake project.
For convenience, a script is provided for building all of the code
examples in one step.

Some of the code examples require std::format (introduced in C++20).
GCC 13 (which is currently under development) has support for std::format.
Alternatively, the fmt library can be used for std::format support.

Assuming that all of the dependencies have been installed, the
software can be built as follows:

1. Set the current working directory to the top-level directory of the
repository (i.e., the top-level directory of the Git working tree).

2. Invoke the build script with the appropriate options.  To invoke
the build script with no options, use the command:

       ./build

   If the C++ standard library being used does not support std::format, the
   "-f" option will need to be added to the invocation of the build script
   above.  That is, use the command:

       ./build -f

   **Note that most systems do not currently provide a C++ standard library
   that supports std::format.  So, in most cases the "-f" option will
   probably be required.**
   The build script supports several different options.  To obtain help
   information for the script (including information on the supported
   options), invoke it with the "-h" (i.e., help) option:

       ./build -h

Some of the code examples require a reasonably recent version of Linux
in order to be built.  If any of the code examples fail to build, the
directories that fail to build can be removed from the list of
directories to be processed in the file:

    slides/examples/CMakeLists.txt

The output of the build process is placed in the directory:

    slides/examples/tmp_cmake

The output for each CMake project is placed in a directory having the same
name as that project.  For example, the build output for the project called
basics is placed in the directory:

    slides/examples/tmp_cmake/basics

Most projects have a demo script (either called "demo" or with a name
containing "demo").  For example, to run the demo script for the basics
project, use the command:

    slides/examples/tmp_cmake/basics/demo

Supported Platforms
-------------------

This software should work (i.e., build and run) on most modern
distributions of Linux.  Since this software is intended for system
programming specifically under Linux, only Linux-based platforms are
officially supported.  This said, however, some of the code is likely
to work on non-Linux systems that are reasonably compliant with the
POSIX standard.

A GitHub CI workflow is used to build the software on an ongoing
basis on the following platforms:

  - Ubuntu 22.04 with GCC
  - Ubuntu 22.04 with Clang
  - Ubuntu 20.04 with Clang

So, the software should build reliably on these platforms.
The development of the software is done primarily on Fedora Linux.
So, the code should also build reliably on recent versions of Fedora Linux.
