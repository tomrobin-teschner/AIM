// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

// c++ include headers
#include <utility>

// third-party include headers

// AIM include headers
#include "src/parameterFileHandling/parameterManager/parameterManager.hpp"

namespace AIM {
namespace Parameters {

/// \name Constructors and destructors
/// @{

/// @}

/// \name API interface that exposes behaviour to the caller
/// @{
void ParameterManager::insert(std::string parameter, DataContainer data) {
  parameters_.insert(std::make_pair(parameter, data));
}

auto ParameterManager::find(const std::string& parameter) const -> const DataContainer& {
  auto parameterInMap = parameters_.find(parameter);

  if (parameters_.size() == 0) throw(std::runtime_error("parameter node is empty"));
  if (parameterInMap != parameters_.end())
    return parameterInMap->second;
  else
    throw(std::runtime_error("could not read parameter from list"));
}
/// @}

/// \name Getters and setters
/// @{
ParameterManager& ParameterManager::getInstance() {
  if (instance_ == nullptr) instance_ = std::shared_ptr<ParameterManager>(new ParameterManager);
  return *instance_.get();
}
/// @}

/// \name Overloaded operators
/// @{

/// @}

/// \name Private or protected implementation details, not exposed to the caller
/// @{

/// @}

}  // namespace Parameters
}  // end namespace AIM
