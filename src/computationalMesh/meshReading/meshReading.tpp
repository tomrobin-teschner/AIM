// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

// c++ include headers
#include <iostream>
#include <string>

// third-party include headers
#include "cgnslib.h"

// AIM include headers
#include "src/types/types.hpp"

namespace AIM {
namespace Mesh {

/// \name Constructors and destructors
/// @{

/// @}

/// \name API interface that exposes behaviour to the caller
/// @{
template <int Index>
auto MeshReader::readCoordinate() -> MeshReader::CoordinateType {
  AIM::Types::CGNSInt begin{1}, end{static_cast<AIM::Types::CGNSInt>(numberOfVertices_)};
  auto coordinate = MeshReader::CoordinateType(numberOfVertices_);
  auto coordinateName = std::vector<std::string>{"CoordinateX", "CoordinateY", "CoordinateZ"};
  auto name = coordinateName[Index].c_str();

  assert(coordinate.size() > 0 && "Coordinate does not have any entries");
  auto errorCode = cg_coord_read(fileIndex_, 1, 1, name, CGNS_ENUMV(RealDouble), &begin, &end, &coordinate[0]);
  assert(errorCode == 0 && "Could not read coordinates from zone");

  return coordinate;
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

/// \name Encapsulated data (private or protected variables)
/// @{

/// @}

}  // namespace Mesh
}  // end namespace AIM