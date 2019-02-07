cmake_minimum_required(VERSION 3.10)
project(MasterPhyl)

set(CMAKE_CXX_STANDARD 11)

add_library(${CMAKE_PROJECT_NAME} src/tree.cpp)

#
#include_directories(src)
#
#add_subdirectory(src)
add_subdirectory(tests)