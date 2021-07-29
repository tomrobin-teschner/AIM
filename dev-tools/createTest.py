import sys
import os


def main():
    assert len(sys.argv) == 2, """
        One command line argument are expected:

        1: Folder name in camelCase, e.g. parameterNodeManager
        """
    folder_name = sys.argv[1]

    if not os.path.exists(folder_name):
        os.makedirs(folder_name)

    test = open(os.path.join(folder_name, folder_name + "Test.cpp"), "w")
    write_file(test, folder_name)
    test.close()
    create_cmake_file(folder_name)


def write_file(file_id, folder_name):
    file_id.write(
        "// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection\n")
    file_id.write(
        "// methods based on hybrid Artificial compressibility and Pressure Projection methods.\n")
    file_id.write(
        "// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.\n")
    file_id.write("\n")
    file_id.write("// c++ include headers\n")
    file_id.write("\n")
    file_id.write("// third-party include headers\n")
    file_id.write("#include <gtest/gtest.h>\n")
    file_id.write("\n")
    file_id.write("// AIM include headers\n")
    file_id.write("\n")
    file_id.write("class " + folder_name[0].capitalize() +
                  folder_name[1:] + "Fixture : public ::testing::Test {\n")
    file_id.write("public:\n")
    file_id.write(
        "  " + folder_name[0].capitalize() + folder_name[1:]+"Fixture() { } \n")
    file_id.write(
        "  ~" + folder_name[0].capitalize() + folder_name[1:]+"Fixture() { } \n")
    file_id.write("  void SetUp() override { } \n")
    file_id.write("  void TearDown() override { } \n")
    file_id.write("\n")
    file_id.write("protected:\n")
    file_id.write("\n")
    file_id.write("};\n")
    file_id.write("\n")
    file_id.write(
        "TEST_F(" + folder_name[0].capitalize() + folder_name[1:] + "Fixture, subNameTest) {\n")
    file_id.write("  // arrange\n")
    file_id.write("\n")
    file_id.write("  // act\n")
    file_id.write("\n")
    file_id.write("  // assert\n")
    file_id.write("\n")
    file_id.write("}\n")
    file_id.write("\n")
    file_id.write(
        "TEST(" + folder_name[0].capitalize() + folder_name[1:] + ", subNameTest) {\n")
    file_id.write("  // arrange\n")
    file_id.write("\n")
    file_id.write("  // act\n")
    file_id.write("\n")
    file_id.write("  // assert\n")
    file_id.write("\n")
    file_id.write("}\n")


def create_cmake_file(folder_name):
    executable = folder_name + "Test"
    cmake = open(os.path.join(folder_name, "CMakeLists.txt"), "w")
    cmake.write("# define test target, link against GTest and add it to CTest\n")
    cmake.write("add_executable(" + executable + " " + executable + ".cpp)\n")
    cmake.write("\n")
    cmake.write("# link against gtest and include root folder\n")
    cmake.write("target_link_libraries(" + executable +
                " PRIVATE GTest::GTest Threads::Threads ${CMAKE_PROJECT_NAME})\n")
    cmake.write("target_include_directories(" + executable +
                " PRIVATE ${PROJECT_SOURCE_DIR})\n")
    cmake.write("\n")
    cmake.write("# copy required mesh files into test executable directory\n")
    cmake.write("add_custom_command(TARGET " + executable +
                "POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy\n")
    cmake.write(
        "${PROJECT_SOURCE_DIR}/tests/testingResources/FILE.EXTENSION\n")
    cmake.write("${CMAKE_BINARY_DIR}/tests/unit/LOCATION)\n")
    cmake.write("\n")
    cmake.write("# let CTest find gtests\n")
    cmake.write("gtest_discover_tests(" + executable + ")\n")
    cmake.close()

    # update top level cmake file if present and does not already contain current subdirectory
    if os.path.exists("CMakeLists.txt"):
        cmake = open("CMakeLists.txt", "r")
        target_line = "add_subdirectory(" + folder_name + ")\n"
        line_exists = False

        for line in cmake:
            if target_line in line:
                print(line)
                line_exists = True
        cmake.close()

        cmake = open("CMakeLists.txt", "a")
        if line_exists == False:
            cmake.write(target_line)
        cmake.close()


if __name__ == "__main__":
    main()
