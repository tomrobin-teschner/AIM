// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

#pragma once

// c++ include headers
#include <filesystem>

// third-party include headers

// AIM include headers

// concept definition

namespace AIM {
namespace Utilities {

/**
 * \class FileChecker
 * \brief Brief description (one line)
 * \ingroup utilities
 *
 * Purpose of class with example usage below (in code section)
 *
 * \code
 *
 * \endcode
 */

class FileChecker {
  /// \name Custom types used in this class
  /// @{

  /// @}

  /// \name Constructors and destructors
  /// @{
public:
  FileChecker() = delete;
  FileChecker(const FileChecker& other) = delete;
  /// @}

  /// \name API interface that exposes behaviour to the caller
  /// @{
public:
  static auto checkIfFileExists(const std::filesystem::path& file) -> void;
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
};

}  // namespace Utilities
}  // end namespace AIM

#include "fileChecker.tpp"