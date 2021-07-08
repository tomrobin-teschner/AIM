# define test target, link against GTest and add it to CTest
add_executable(coordinateArrayTest coordinateArrayTest.cpp)
target_link_libraries(coordinateArrayTest PRIVATE GTest::GTest Threads::Threads ${CMAKE_PROJECT_NAME})
gtest_discover_tests(coordinateArrayTest)