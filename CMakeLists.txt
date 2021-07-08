# initialise project
cmake_minimum_required(VERSION 3.16)
project(AIM VERSION 0.0.0 LANGUAGES CXX)

# use c++20 standard
set(CMAKE_CXX_STANDARD 20)

# handling of packages transparent from conan
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

# create shared library
add_library(${CMAKE_PROJECT_NAME} SHARED "")

# find required libraries
find_package(Eigen3 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cgns REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# link against libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Eigen3::Eigen3)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE cgns::cgns)

# add source files to main target by traversing source folders
add_subdirectory(src)

# add soure files to testing targets by traversing test folders
add_subdirectory(tests)