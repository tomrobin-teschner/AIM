// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

// c++ include headers
#include <any>
#include <string>

// third-party include headers

// AIM include headers
#include "src/parameterFileHandling/dataContainer/dataContainer.hpp"

namespace AIM {
namespace Parameters {

/// \name Constructors and destructors
/// @{
DataContainer::DataContainer(const std::string& name, const std::string& description, const std::any& data)
    : name_(name), description_(description), data_(data) {}
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

/// @}

}  // namespace Parameters
}  // end namespace AIM
