include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include(GoogleTest)

add_executable(Unit_Tests_run test_tree.cpp test_tree_rf.cpp)
target_include_directories(Unit_Tests_run PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}lib/)
#gtest_add_tests(TARGET Unit_Tests_run
#        SOURCES test_tree.cpp
#        )

target_link_libraries(Unit_Tests_run gtest gtest_main)
target_link_libraries(Unit_Tests_run masterphyl_core)