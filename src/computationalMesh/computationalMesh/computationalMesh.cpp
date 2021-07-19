// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

// c++ include headers
#include <cassert>

// third-party include headers

// AIM include headers
#include "src/computationalMesh/computationalMesh/computationalMesh.hpp"
#include "src/types/enums.hpp"

namespace AIM {
namespace Mesh {

/// \name Constructors and destructors
/// @{
ComputationalMesh::ComputationalMesh(const MeshReader& meshReader) : meshReader_(meshReader) {
  coordinateX_ = meshReader_.readCoordinate<AIM::Enum::Coordinate::X>();
  coordinateY_ = meshReader_.readCoordinate<AIM::Enum::Coordinate::Y>();
  if (meshReader_.getDimensions() == AIM::Enum::Dimension::Three)
    coordinateZ_ = meshReader_.readCoordinate<AIM::Enum::Coordinate::Z>();

  connectivityTable_ = meshReader_.readConnectivityTable();

  boundaryConditionInfo_ = meshReader_.readBoundaryConditions();
  boundaryConditionConnectivityTable_ = meshReader_.readBoundaryConditionConnectivity();
}
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

/// \name Encapsulated data (private or protected variables)
/// @{

/// @}

}  // namespace Mesh
}  // end namespace AIM