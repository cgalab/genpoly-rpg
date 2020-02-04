cmake_minimum_required (VERSION 2.6)
project (rpg)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -gdwarf-2 -g3 -DDEBUG -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 -DNDEBUG")

set(PACKAGE_STRING rpg)
set(PACKAGE_BUGREPORT held@cs.sbg.ac.at)
set(PACKAGE_VERSION 3.0)

# More verbose output by make
# set(CMAKE_VERBOSE_MAKEFILE "ON")
add_subdirectory(src)
