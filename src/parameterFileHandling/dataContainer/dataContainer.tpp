// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

// c++ include headers

// third-party include headers

// AIM include headers

namespace AIM {
namespace Parameters {

/// \name Constructors and destructors
/// @{

/// @}

/// \name API interface that exposes behaviour to the caller
/// @{
template <typename DestinationType>
auto DataContainer::getValue() const -> DestinationType {
  return std::any_cast<DestinationType>(data_);
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

}  // namespace Parameters
}  // end namespace AIM
