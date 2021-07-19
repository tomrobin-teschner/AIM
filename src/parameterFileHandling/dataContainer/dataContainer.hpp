// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

#pragma once

// c++ include headers
#include <any>
#include <string>

// third-party include headers

// AIM include headers

// concept definition

namespace AIM {
namespace Parameters {

/**
 * \class DataContainer
 * \brief Brief description (one line)
 * \ingroup Parameters
 *
 * Purpose of class with example usage below (in code section)
 *
 * \code
 *
 * \endcode
 */

class DataContainer {
  /// \name Custom types used in this class
  /// @{

  /// @}

  /// \name Constructors and destructors
  /// @{
public:
  DataContainer(const std::string& name, const std::string& description, const std::any& data);
  /// @}

  /// \name API interface that exposes behaviour to the caller
  /// @{

  /// @}

  /// \name Getters and setters
  /// @{
public:
  auto getName() const -> std::string { return name_; }
  auto getDescription() const -> std::string { return description_; }
  template <typename DestinationType>
  auto getValue() const -> DestinationType;
  /// @}

  /// \name Overloaded operators
  /// @{

  /// @}

  /// \name Private or protected implementation details, not exposed to the caller
  /// @{

  /// @}

  /// \name Encapsulated data (private or protected variables)
  /// @{
private:
  const std::string name_;
  const std::string description_;
  std::any data_;
  /// @}
};

}  // namespace Parameters
}  // end namespace AIM

#include "src/parameterFileHandling/dataContainer/dataContainer.tpp"
