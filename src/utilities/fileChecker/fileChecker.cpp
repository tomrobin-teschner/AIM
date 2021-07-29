// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

// c++ include headers
#include <filesystem>
#include <iostream>

// third-party include headers

// AIM include headers
#include "src/utilities/fileChecker/fileChecker.hpp"

namespace AIM {
namespace Utilities {

/// \name Constructors and destructors
/// @{

/// @}

/// \name API interface that exposes behaviour to the caller
/// @{
auto FileChecker::checkIfFileExists(const std::filesystem::path& file) -> void {
  try {
    if (!std::filesystem::exists(file))
      throw std::runtime_error(std::string("can't find the following file: " + file.string()));
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    std::cout << "Tried opening file from current path (" << std::filesystem::current_path() << ")" << std::endl;
    std::terminate();
  }
}
/// @}

/// \name Getters and setters
/// @{

/// @}

/// \name Overloaded operators
/// @{

/// @}

/// \name Private or protected implementation details, not exposed to the caller
/// @{

/// @}

/// \name Encapsulated data (private or protected variables)
/// @{

/// @}

}  // namespace Utilities
}  // end namespace AIM