cmake_minimum_required(VERSION 3.10)
project(MasterPhyl
        VERSION 0.10.0)
set(CMAKE_CXX_STANDARD 11)


########################################
## BUILD MASTERPHYL CORE LIBRARY CODE ##
########################################
add_subdirectory(lib)

########################################
## BUILD SHARED MASTERPHYL LIBRARY #####
########################################
add_library(masterphyl SHARED lib/masterphyl_shared.cpp)
target_link_libraries(masterphyl masterphyl_core)

########################################
## BUILD MASTERPHYL EXECUTABLE #########
########################################
add_executable(${CMAKE_PROJECT_NAME}  app/mock_main.cpp)



add_subdirectory(tests)