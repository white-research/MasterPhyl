cmake_minimum_required(VERSION 3.10)
project(MasterPhyl)

set(CMAKE_CXX_STANDARD 11)

add_library(${CMAKE_PROJECT_NAME}  src/matrix.cpp src/tree.cpp src/rf.cpp src/dynamic_homology.cpp src/output.cpp src/analysis.cpp)

#
#include_directories(src)
#
#add_subdirectory(src)
add_subdirectory(tests)