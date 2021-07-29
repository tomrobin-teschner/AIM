// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

// c++ include headers
#include <filesystem>

// third-party include headers

// AIM include headers
#include "src/parameterFileReading/parameterFileReading.hpp"

namespace AIM {
namespace Parameters {

/// \name Constructors and destructors
/// @{

/// @}

/// \name API interface that exposes behaviour to the caller
/// @{

/// @}

/// \name Getters and setters
/// @{

/// @}

/// \name Overloaded operators
/// @{

/// @}

/// \name Private or protected implementation details, not exposed to the caller
/// @{
auto ParameterFileReading::getJSONFile(const std::filesystem::path& file) -> nlohmann::json {
  AIM::Utilities::FileChecker::checkIfFileExists(file);
  std::ifstream rawFile(file);
  auto jsonFile = nlohmann::json{};
  rawFile >> jsonFile;
  return jsonFile;
}
/// @}

}  // namespace Parameters
}  // end namespace AIM
