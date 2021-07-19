// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

#pragma once

// c++ include headers

// third-party include headers

// AIM include headers
#include "src/computationalMesh/meshReading/meshReading.hpp"

// concept definition

namespace AIM {
namespace Mesh {

/**
 * \class ComputationalMesh
 * \brief Brief description (one line)
 * \ingroup mesh
 *
 * Purpose of class with example usage below (in code section)
 *
 * \code
 *
 * \endcode
 */

class ComputationalMesh {
  /// \name Custom types used in this class
  /// @{
public:
  using CoordinateType = typename MeshReader::CoordinateType;
  using ConnectivityTableType = typename MeshReader::ConnectivityTableType;
  using BoundaryConditionType = typename MeshReader::BoundaryConditionType;
  using BoundaryConditionConnectivityType = typename MeshReader::BoundaryConditionConnectivityType;
  /// @}

  /// \name Constructors and destructors
  /// @{
public:
  ComputationalMesh(const MeshReader& meshReader);
  /// @}

  /// \name API interface that exposes behaviour to the caller
  /// @{

  /// @}

  /// \name Getters and setters
  /// @{
public:
  auto getCoordinateX() const -> const CoordinateType& { return coordinateX_; }
  auto getCoordinateY() const -> const CoordinateType& { return coordinateY_; }
  auto getCoordinateZ() const -> const CoordinateType& { return coordinateZ_; }
  auto getConnectivityTable() const -> const ConnectivityTableType& { return connectivityTable_; }
  auto getBoundaryConditionInfo() const -> const BoundaryConditionType& { return boundaryConditionInfo_; }
  auto getBoundaryConditionConnvectivity() const -> const BoundaryConditionConnectivityType& {
    return boundaryConditionConnectivityTable_;
  }
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
  MeshReader meshReader_;

  CoordinateType coordinateX_;
  CoordinateType coordinateY_;
  CoordinateType coordinateZ_;

  ConnectivityTableType connectivityTable_;
  BoundaryConditionType boundaryConditionInfo_;
  BoundaryConditionConnectivityType boundaryConditionConnectivityTable_;
  /// @}
};

}  // namespace Mesh
}  // end namespace AIM

#include "computationalMesh.tpp"