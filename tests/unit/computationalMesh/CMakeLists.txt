# define test target, link against GTest and add it to CTest
add_executable(computationalMeshTest "")

# add tests to target
add_subdirectory(meshReading)
add_subdirectory(computationalMesh)

# link against gtest and include root folder
target_link_libraries(computationalMeshTest PRIVATE GTest::GTest Threads::Threads ${CMAKE_PROJECT_NAME})
target_include_directories(computationalMeshTest PRIVATE ${PROJECT_SOURCE_DIR})

# copy required mesh files into test executable directory
add_custom_command(TARGET computationalMeshTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
${PROJECT_SOURCE_DIR}/tests/testingResources/mesh/test2D.cgns
${CMAKE_BINARY_DIR}/tests/unit/computationalMesh)

# let CTest find gtests
gtest_discover_tests(computationalMeshTest)