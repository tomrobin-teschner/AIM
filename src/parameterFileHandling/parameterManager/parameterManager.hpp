// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

#pragma once

// c++ include headers
#include <memory>
#include <string>
#include <unordered_map>

// third-party include headers

// AIM include headers
#include "src/parameterFileHandling/dataContainer/dataContainer.hpp"

// concept definition

namespace AIM {
namespace Parameters {

/**
 * \class ParameterManager
 * \brief Brief description (one line)
 * \ingroup Parameters
 *
 * Purpose of class with example usage below (in code section)
 *
 * \code
 *
 * \endcode
 */

class ParameterManager {
  /// \name Custom types used in this class
  /// @{
public:
  using ParameterNodeType = typename std::unordered_map<std::string, DataContainer>;
  /// @}

  /// \name Constructors and destructors
  /// @{
private:
  ParameterManager() = default;

public:
  ParameterManager(const ParameterManager& other) = delete;
  /// @}

  /// \name API interface that exposes behaviour to the caller
  /// @{
public:
  auto insert(std::string parameter, DataContainer data) -> void;
  auto find(const std::string& parameter) const -> const DataContainer&;
  /// @}

  /// \name Getters and setters
  /// @{
public:
  static ParameterManager& getInstance();
  /// @}

  /// \name Overloaded operators
  /// @{
public:
  void operator=(const ParameterManager& other) = delete;
  /// @}

  /// \name Private or protected implementation details, not exposed to the caller
  /// @{

  /// @}

  /// \name Encapsulated data (private or protected variables)
  /// @{
private:
  static std::shared_ptr<ParameterManager> instance_;
  ParameterNodeType parameters_;
  /// @}
};

std::shared_ptr<ParameterManager> ParameterManager::instance_ = nullptr;

}  // namespace Parameters
}  // end namespace AIM

#include "parameterManager.tpp"
