// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

#pragma once

// c++ include headers
#include <filesystem>
#include <optional>
#include <string>

// third-party include headers
#include "nlohmann/json.hpp"

// AIM include headers

// concept definition

namespace AIM {
namespace Parameters {

/**
 * \class ParameterFileReading
 * \brief Read parameters from any input file in JSON format
 * \ingroup Parameters
 *
 * This class exposes only a single static method and can't be instantiated (constructors, move constructors and
 * assignment operator are deleted as there is no internal state that is stored by the class). The exposed method will
 * attempt to read a parameter from the provided input file and convert it to the type provided. Exception checking will
 * ensure that the parameter exists and that it can be converted to the specified type. Any unhandled exceptions will be
 * captured and the solver will terminate if it can't recover from this situation. A default value can optionally be
 * provided which will be used if the specified parameter can't be located in the input file. If a default value can be
 * prescribed, it should. The philosophy here is that the solver should be able to run with as small of an input script
 * as possible.
 *
 * Example code:
 * \code
 * // read a parameter providing a default value, parameter of stype std::string (e.g. filename of the mesh)
 * auto file = std::filesystem::path("inputFile.json");
 * auto parameter = AIM::Parameters::ParameterFileReading::readParameterOrGetDefaultValue<std::string>(file,
 * "/mesh/filename", "mesh/mesh.cgns");
 *
 * // read a parameter providing without providing a default value, parameter of stype std::string
 * auto parameter = AIM::Parameters::ParameterFileReading::readParameterOrGetDefaultValue<std::string>(file,
 * "/mesh/filename", std::nullopt); \endcode
 */

class ParameterFileReading {
  /// \name Custom types used in this class
  /// @{

  /// @}

  /// \name Constructors and destructors
  /// @{
public:
  ParameterFileReading() = delete;
  ParameterFileReading(const ParameterFileReading& other) = delete;
  ParameterFileReading(ParameterFileReading&& other) = delete;
  /// @}

  /// \name API interface that exposes behaviour to the caller
  /// @{
public:
  template <typename ParameterType>
  static auto readParameterOrGetDefaultValue(const std::filesystem::path& file, const std::string& parameter,
    const std::optional<ParameterType>& defaultValue) -> ParameterType;
  /// @}

  /// \name Getters and setters
  /// @{

  /// @}

  /// \name Overloaded operators
  /// @{
public:
  auto operator=(const ParameterFileReading& other) -> void = delete;
  /// @}

  /// \name Private or protected implementation details, not exposed to the caller
  /// @{
private:
  static auto getJSONFile(const std::filesystem::path& file) -> nlohmann::json;
  template <typename ParameterType>
  static auto getParameterFromJSONFile(const nlohmann::json& jsonFile, const std::filesystem::path& file,
    const std::string& parameter, const std::optional<ParameterType>& defaultValue) -> ParameterType;
  /// @}

  /// \name Encapsulated data (private or protected variables)
  /// @{
private:
  /// @}
};

}  // namespace Parameters
}  // end namespace AIM

#include "parameterFileReading.tpp"
