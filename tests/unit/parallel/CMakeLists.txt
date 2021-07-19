# define test target, link against GTest and add it to CTest
add_executable(parallelTest parallelMain.cpp parallelTest.cpp)
target_link_libraries(parallelTest PRIVATE GTest::GTest Threads::Threads ${CMAKE_PROJECT_NAME})
gtest_discover_tests(parallelTest)

# include root directory so header files can be specified relative to the project root
target_include_directories(parallelTest PRIVATE ${PROJECT_SOURCE_DIR})