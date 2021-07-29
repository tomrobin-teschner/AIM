import sys
import os


def main():
    assert len(sys.argv) == 4, """
        Three command line argument are expected:

        1: Folder name in camelCase, e.g. parameterNodeManager
        2: class name in camelCase, can be the same as folder name (if you type same, the folder name will be used)
        3: Group name in PascalCase (remember to update the src/groupDefinition.hpp file if a new group is created)
        """
    folder_name = sys.argv[1]
    class_name = sys.argv[2]
    group_name = sys.argv[3]

    if not os.path.exists(folder_name):
        os.makedirs(folder_name)

    header = open(os.path.join(folder_name, class_name + ".hpp"), "w")
    source = open(os.path.join(folder_name, class_name + ".cpp"), "w")
    template = open(os.path.join(folder_name, class_name + ".tpp"), "w")

    write_file(header, class_name, group_name, True)
    write_file(source, class_name, group_name, False)
    write_file(template, class_name, group_name, False)

    header.close()
    source.close()
    template.close()

    create_cmake_file(folder_name, class_name)


def write_file(file_id, class_name, group_name, is_header_file):
    file_id.write(
        "// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection\n")
    file_id.write(
        "// methods based on hybrid Artificial compressibility and Pressure Projection methods.\n")
    file_id.write(
        "// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.\n")
    file_id.write("\n")
    if is_header_file == True:
        file_id.write("#pragma once\n")
        file_id.write("\n")
    file_id.write("// c++ include headers\n")
    file_id.write("\n")
    file_id.write("// third-party include headers\n")
    file_id.write("\n")
    file_id.write("// AIM include headers\n")
    file_id.write("\n")

    if is_header_file == True:
        file_id.write("// concept definition\n")
        file_id.write("\n")
        file_id.write("namespace AIM {\n")
        file_id.write("namespace " + group_name + " {\n")
        file_id.write("\n")
        file_id.write("/**\n")
        file_id.write(
            "* \class " + class_name[0].capitalize() + class_name[1:] + "\n")
        file_id.write("* \\brief Brief description (one line)\n")
        file_id.write("* \ingroup " + group_name + "\n")
        file_id.write("*\n")
        file_id.write(
            "* Purpose of class with example usage below (in code section)\n")
        file_id.write("*\n")
        file_id.write("* Example usage:\n")
        file_id.write("* \code\n")
        file_id.write("*\n")
        file_id.write("* \endcode\n")
        file_id.write("*/\n")
        file_id.write("\n")
        file_id.write(
            "class " + class_name[0].capitalize() + class_name[1:] + " {\n")
        file_id.write("/// \\name Custom types used in this class\n")
        file_id.write("/// @{\n")
        file_id.write("\n")
        file_id.write("/// @}\n")
        file_id.write("\n")
    else:
        file_id.write("namespace AIM {\n")
        file_id.write("namespace " + group_name + " {\n")
        file_id.write("\n")

    file_id.write("/// \\name Constructors and destructors\n")
    file_id.write("/// @{\n")
    file_id.write("\n")
    file_id.write("/// @}\n")
    file_id.write("\n")
    file_id.write(
        "/// \\name API interface that exposes behaviour to the caller\n")
    file_id.write("/// @{\n")
    file_id.write("\n")
    file_id.write("/// @}\n")
    file_id.write("\n")
    file_id.write("/// \\name Getters and setters\n")
    file_id.write("/// @{\n")
    file_id.write("\n")
    file_id.write("/// @}\n")
    file_id.write("\n")
    file_id.write("/// \\name Overloaded operators\n")
    file_id.write("/// @{\n")
    file_id.write("\n")
    file_id.write("/// @}\n")
    file_id.write("\n")
    file_id.write(
        "/// \\name Private or protected implementation details, not exposed to the caller\n")
    file_id.write("/// @{\n")
    file_id.write("\n")
    file_id.write("/// @}\n")
    file_id.write("\n")
    if is_header_file == True:
        file_id.write(
            "/// \\name Encapsulated data (private or protected variables)\n")
        file_id.write("/// @{\n")
        file_id.write("\n")
        file_id.write("/// @}\n")
        file_id.write("};\n")
    file_id.write("\n")
    file_id.write("}// namespace " + group_name + "\n")
    file_id.write("}// end namespace AIM\n")
    if is_header_file == True:
        file_id.write("\n")
        file_id.write("#include \"" + class_name + ".tpp\n")


def create_cmake_file(folder_name, class_name):
    cmake = open(os.path.join(folder_name, "CMakeLists.txt"), "w")
    cmake.write(
        "target_sources(${CMAKE_PROJECT_NAME} PRIVATE " + class_name + ".cpp)\n")
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
