# define test target, link against GTest and add it to CTest
add_executable(parameterFileReadingTest parameterFileReadingTest.cpp)

# link against gtest and include root folder
target_link_libraries(parameterFileReadingTest PRIVATE GTest::GTest Threads::Threads nlohmann_json::nlohmann_json
  ${CMAKE_PROJECT_NAME})
target_include_directories(parameterFileReadingTest PRIVATE ${PROJECT_SOURCE_DIR})

# copy required mesh files into test executable directory
add_custom_command(TARGET parameterFileReadingTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
${PROJECT_SOURCE_DIR}/tests/testingResources/inputFiles/aim.json
${CMAKE_BINARY_DIR}/tests/unit/ParameterFileReading)

# let CTest find gtests
gtest_discover_tests(parameterFileReadingTest)
