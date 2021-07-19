# define test target, link against GTest and add it to CTest
add_executable(parameterManagerTest parameterManagerTest.cpp)

# link against gtest and include root folder
target_link_libraries(parameterManagerTest PRIVATE GTest::GTest Threads::Threads ${CMAKE_PROJECT_NAME})
target_include_directories(parameterManagerTest PRIVATE ${PROJECT_SOURCE_DIR})

# let CTest find gtests
gtest_discover_tests(parameterManagerTest)
