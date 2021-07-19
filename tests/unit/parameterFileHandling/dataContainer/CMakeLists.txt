# define test target, link against GTest and add it to CTest
add_executable(dataContainerTest dataContainerTest.cpp)

# link against gtest and include root folder
target_link_libraries(dataContainerTest PRIVATE GTest::GTest Threads::Threads ${CMAKE_PROJECT_NAME})
target_include_directories(dataContainerTest PRIVATE ${PROJECT_SOURCE_DIR})

# copy required mesh files into test executable directory
add_custom_command(TARGET dataContainerTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
${PROJECT_SOURCE_DIR}/tests/testingResources/inputFiles/aim.json
${CMAKE_BINARY_DIR}/tests/unit/parameterFileHandling)

# let CTest find gtests
gtest_discover_tests(dataContainerTest)