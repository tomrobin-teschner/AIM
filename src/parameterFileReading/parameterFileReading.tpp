// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

// c++ include headers
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

// third-party include headers
#include "nlohmann/json.hpp"

// AIM include headers
#include "src/utilities/fileChecker/fileChecker.hpp"

namespace AIM {
namespace Parameters {

/// \name Constructors and destructors
/// @{

/// @}

/// \name API interface that exposes behaviour to the caller
/// @{
template <typename ParameterType>
auto ParameterFileReading::readParameterOrGetDefaultValue(const std::filesystem::path& inputFile,
  const std::string& parameter, const std::optional<ParameterType>& defaultValue) -> ParameterType {
  auto jsonFile = getJSONFile(inputFile);
  auto parameterValue = getParameterFromJSONFile<ParameterType>(jsonFile, inputFile, parameter, defaultValue);
  return parameterValue;
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
template <typename ParameterType>
auto ParameterFileReading::getParameterFromJSONFile(const nlohmann::json& jsonFile,
  const std::filesystem::path& inputFile, const std::string& parameter,
  const std::optional<ParameterType>& defaultValue) -> ParameterType {
  try {
    auto parameterInJSON = jsonFile.at(static_cast<nlohmann::json_pointer<nlohmann::basic_json<>>>(parameter));
    return static_cast<ParameterType>(parameterInJSON);
  } catch (const nlohmann::detail::out_of_range& e) {
    if (defaultValue.has_value())
      return static_cast<ParameterType>(defaultValue.value());
    else {
      auto errorMessage = std::stringstream{"parameter \"" + parameter + "\" not found in \"" + inputFile.string() +
                                            "\" and no default value is available"};
      throw std::runtime_error(errorMessage.str());
    }
  } catch (...) {
    std::cerr << "Unknown exception caught when trying to read parameters from: " << inputFile.string() << std::endl;
    std::terminate();
  }
}
/// @}

}  // namespace Parameters
}  // end namespace AIM
