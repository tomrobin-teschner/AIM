# define test target, link against GTest and add it to CTest
add_executable(meshReadingTest meshReadingTest.cpp)
target_link_libraries(meshReadingTest PRIVATE GTest::GTest Threads::Threads ${CMAKE_PROJECT_NAME})
gtest_discover_tests(meshReadingTest)

# include root directory so header files can be specified relative to the project root
target_include_directories(meshReadingTest PRIVATE ${PROJECT_SOURCE_DIR})

# copy required test mesh into test executable directory
add_custom_command(TARGET meshReadingTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
${PROJECT_SOURCE_DIR}/tests/testingResources/mesh/test2D.cgns
${CMAKE_BINARY_DIR}/tests/unit/computationalMesh/meshReading)