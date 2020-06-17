cmake_minimum_required(VERSION 3.10)
project(ged LANGUAGES C)

enable_testing()
include(CheckIncludeFiles)

set(CMAKE_C_STANDARD 99)
set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_LINK_SEARCH_START_STATIC on)

# ----------------------------------------------------------------------------------------
# Make sure that the PCRE2 library components (headers and archive) are reachable.
# ----------------------------------------------------------------------------------------

message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("Path to PCRE2 library (PCRE2_LIBRARY_PATH): $ENV{PCRE2_LIBRARY_PATH}")
message("Path to PCRE2 headers (PCRE2_INCLUDE_PATH): $ENV{PCRE2_INCLUDE_PATH}")

# Try to locate the archive "libpcre2-8.a"
# Note: if Cmake cannot find the library, try this:
#       rm -rf cmake* CMakeFiles CMakeCache.txt

find_library(LIB_PCRE2 pcre2-8 HINTS ENV PCRE2_LIBRARY_PATH)
if(NOT LIB_PCRE2)
    message(FATAL_ERROR "PCRE2 library not found. Please configure PCRE2_LIBRARY_PATH.")
else()
    message("OK: PCRE2 library found")
endif()

# Try to locate the header "pcre2.h"
# WARNING: the macro PCRE2_CODE_UNIT_WIDTH must be defined before "pcre2.h" inclusion.
#          Thus, you cannot test for "pcre2.h" directly. That's why we use an
#          intermediate header file.
# Note: if Cmake cannot find the header, try this:
#       rm -rf cmake* CMakeFiles CMakeCache.txt

set(CMAKE_REQUIRED_INCLUDES "${CMAKE_SOURCE_DIR}/extra;$ENV{PCRE2_INCLUDE_PATH}")
message("Expect the test header \"pcre2_header_tester.h\" to be under \"${CMAKE_SOURCE_DIR}/extra\"")

CHECK_INCLUDE_FILES("pcre2_header_tester.h" HEADERS_PCRE2 LANGUAGE C)
if(NOT HEADERS_PCRE2)
    message(FATAL_ERROR "PCRE2 header files not found. Please configure PCRE2_INCLUDE_PATH.")
else()
    message("OK: PCRE2 header files found")
endif()

# ----------------------------------------------------------------------------------------
# Configure the compiler / linker environment.
# ----------------------------------------------------------------------------------------

# Note: if you use CLion in full remote mode, then you must also configure the CMake execution.
#       >> File | Settings | Build, Execution, Deployment | CMake | Environment
#       >> Tools | CMake | Reset Cache and Reload Project

include_directories($ENV{PCRE2_INCLUDE_PATH})
link_directories($ENV{PCRE2_LIBRARY_PATH})

# ----------------------------------------------------------------------------------------
# Build tools used for the compilation.
# ----------------------------------------------------------------------------------------

add_executable(version.exe extra/version.c)

set_target_properties(
        version.exe
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY bin
        EXCLUDE_FROM_ALL off)

# ----------------------------------------------------------------------------------------
# Create the file "src/date.h" that contains the auto-generated version number
# ----------------------------------------------------------------------------------------

add_custom_target(version
        COMMAND bin/version.exe src/date.h
        COMMENT "Create the date that represents the version"
        )
add_dependencies(version version.exe)

# ----------------------------------------------------------------------------------------
# Build the application
# ----------------------------------------------------------------------------------------

add_executable(app.exe src/main.c src/toolbox.c src/toolbox.h src/date.h)
add_dependencies(app.exe version)
target_link_libraries(app.exe pcre2-8)
set_target_properties(
        app.exe
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY bin
        EXCLUDE_FROM_ALL off)
