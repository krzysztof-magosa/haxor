cmake_minimum_required(VERSION 2.8.7)
project(Haxor)
#enable_testing()
#include(ExternalProject)
include_directories(src)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-register")
set(CMAKE_MACOSX_RPATH 1)

add_subdirectory (src)
