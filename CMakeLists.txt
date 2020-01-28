cmake_minimum_required (VERSION 2.6)
project (rpg)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-gdwarf-2 -g3 -DDEBUG -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(PACKAGE_STRING rpg)
set(PACKAGE_BUGREPORT held@cs.sbg.ac.at)
set(PACKAGE_VERSION 3.0)

# More verbose output by make
# set(CMAKE_VERBOSE_MAKEFILE "ON")
add_subdirectory(src)
