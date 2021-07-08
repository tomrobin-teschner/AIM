// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

#pragma once

// c++ include headers
#include <vector>

// third-party include headers

// AIM include headers

// concept definition

namespace AIM {
namespace Mesh {

/**
 * \class CoordinateArray
 * \brief This class contains the definition of the coordinate array, a wrapper around std::vector
 * \ingroup mesh
 *
 * The intention of this class is to wrapp functionality around a std::vector and expose additional functionality as
 * required to the user. Use it like you would use a std::vector<T>.
 *
 * \code
 *
 * \endcode
 */

class CoordinateArray {
  /// \name Custom types used in this class
  /// @{
public:
  using CoordinateType = typename std::vector<double>;
  /// @}

  /// \name Constructors and destructors
  /// @{

  /// @}

  /// \name API interface that exposes behaviour to the caller
  /// @{
public:
  auto push_back(double data) -> void { data_.push_back(data); }
  auto size() -> std::size_t { return data_.size(); }
  auto capacity() -> std::size_t { return data_.capacity(); }
  auto begin() -> CoordinateType::iterator { return data_.begin(); }
  auto end() -> CoordinateType::iterator { return data_.end(); }
  /// @}

  /// \name Getters and setters
  /// @{
public:
  auto getReference() -> const CoordinateType& { return data_; }
  auto getModifiable() -> CoordinateType& { return data_; }
  /// @}

  /// \name Overloaded operators
  /// @{
public:
  auto operator[](int index) -> double { return data_[index]; }
  /// @}

  /// \name Private or protected implementation details, not exposed to the caller
  /// @{

  /// @}

  /// \name Encapsulated data (private or protected variables)
  /// @{
private:
  CoordinateType data_;
  /// @}
};

}  // namespace Mesh
}  // end namespace AIM

#include "coordinateArray.tpp"