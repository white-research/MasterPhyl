add_subdirectory(tree)



add_library(masterphyl_core masterphyl_core.cpp)

target_link_libraries(masterphyl_core tree)




#include_directories(tree/)

