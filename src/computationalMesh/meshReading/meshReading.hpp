// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

#pragma once

// c++ include headers
#include <filesystem>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// third-party include headers
#include "cgnslib.h"

// AIM include headers
#include "src/types/types.hpp"

// concept definition
namespace AIM {
namespace Mesh {

/**
 * \class MeshReader
 * \brief This class processes a CGNS file from which the mesh properties are read
 * \ingroup mesh
 *
 * This class provides a wrapper around the cgns file format and reads mesh data from these files. The constructor takes
 * two arguments, the first being the location of the mesh and the second the dimensionality of the mesh. Then, the user
 * can load different aspects from the file. No data is stored in this class and it is the responsibility of the calling
 * method to store the data after calling it (i.e. there are no getter or setter methods implemented). The example below
 * shows how this class may be used.
 *
 * \code
 * // 2D mesh file
 * auto meshReader = AIM::Mesh::MeshReader{std::filesystem::path("path/to/file.cgns"), AIM::Enum::Dimension::Two};
 * // 3D mesh file
 * auto meshReader = AIM::Mesh::MeshReader{std::filesystem::path("path/to/file.cgns"), AIM::Enum::Dimension::Three};
 * ...
 * // read coordinates
 * auto x = meshReader.readCoordinate<AIM::Enum::Coordinate::X>();
 * auto y = meshReader.readCoordinate<AIM::Enum::Coordinate::Y>();
 * auto z = meshReader.readCoordinate<AIM::Enum::Coordinate::Z>();
 * ...
 * // read connectivity table
 * auto connectivityTable = meshReader.readConnectivityTable();
 * ...
 * // read boundary conditions
 * auto bc = meshReader.readBoundaryConditions();
 * ...
 * // read boundary condition connectivity array
 * auto bcc = meshReader.readBoundaryConditionConnectivity();
 * \endcode
 *
 * The coordinate arrays require a template index parameter to identify which coordinate to read (i.e x = 0, y = 1, and
 * z = 2). This can be circumvented by using a built-in enum to aid documentation, i.e. AIM::Enum::Coordinate::X,
 * AIM::Enum::Coordinate::Y or AIM::Enum::Coordinate::Z. It is a one dimensional array of type std::vector<FloatType>
 * where FloatType is typically a wrapper around double, but can be set to float in the src/types/types.hpp file.
 *
 * \code
 * // loop over coordinates
 * auto x = meshReader.readCoordinate<AIM::Enum::Coordinate::X>();
 * for (const auto &c : x)
 *   std::cout << "coordinate x: " << c << std::endl;
 *
 * // loop using classical for loop
 * for (int i = 0; i < x.size(); ++i)
 *   std::cout << "coordinate x[" << i << "]: " << x[i] << std::endl;
 * \endcode
 *
 * The connectivity table is a 2D vector with the first index being the number of cells in the mesh and the second index
 * being the number of vertices for the current cell type. For example, if we have a mesh with two elements, one tri and
 * one quad element, we may have the following structure:
 *
 * \code
 * auto connectivityTable = meshReader.readConnectivityTable();
 *
 * // first element is a tri element with 3 vertices
 * assert(connectivityTable[0].size() == 3);
 *
 * // second element is a quad element with 4 vertices
 * assert(connectivityTable[1].size() == 4);
 *
 * auto cell_0_vertex_0 = connectivityTable[0][0];
 * auto cell_0_vertex_1 = connectivityTable[0][1];
 * auto cell_0_vertex_2 = connectivityTable[0][2];
 *
 * auto cell_1_vertex_0 = connectivityTable[1][0];
 * auto cell_1_vertex_1 = connectivityTable[1][1];
 * auto cell_1_vertex_2 = connectivityTable[1][2];
 * auto cell_1_vertex_3 = connectivityTable[1][3];
 * \endcode
 *
 * The boundary conditions are read into two different arrays. The first will provide information about the type
 * and boundary name and is stored in a std::vector<std::pair<int, std::string>>. The first index of the pair is an int
 * whose boundary condition can be queried using the build in enums. The second argument is the name of the boundary
 * condition assigned at the meshing stage. Example usage:
 *
 * \code
 * auto bc = meshReader.readBoundaryConditions();
 *
 * for (const auto &boundary : bc) {
 *   if (boundary.first == AIM::Enum::BoundaryCondition::Wall)
 *     std::cout << "Wall BC with name: " << boundary.second;
 *   ...
 *   // other available types are:
 *   auto inletBC    = AIM::Enum::BoundaryCondition::Inlet;
 *   auto outletBC   = AIM::Enum::BoundaryCondition::Outlet;
 *   auto symmetryBC = AIM::Enum::BoundaryCondition::Symmetry;
 *
 *   // see src/types/enums.hpp for a full list of supported boundary conditions
 * }
 * \endcode
 *
 * The second part of the boundary condition readings provide the elements that are connected to each boundary condition
 * read above. For each boundary condition, we have a std::vector that provides us with the element indices attached to
 * that boundary condition. Example usage:
 *
 * \code
 * auto bc = meshReader.readBoundaryConditions();
 * auto bcc = meshReader.readBoundaryConditionConnectivity();
 *
 * assert(bc.size() == bcc.size() && "Boundary condition info and connectivity must have the same size");
 *
 * for (int i = 0; i < bcc.size(); ++i) {
 *   std::cout << "Elements connected to " << bc[i].second << " are: ";
 *   for (const auto& e : bcc[i])
 *     std::cout << e << " ";
 *   std::cout << std::endl;
 * }
 * \endcode
 */

class MeshReader {
  /// \name Custom types used in this class
  /// @{
public:
  using CoordinateType = typename std::vector<AIM::Types::FloatType>;
  using ConnectivityTableType = typename std::vector<std::vector<AIM::Types::UInt>>;
  using BoundaryConditionType = typename std::vector<std::pair<int, std::string>>;
  using BoundaryConditionConnectivityType = typename std::vector<std::vector<AIM::Types::CGNSInt>>;
  /// @}

  /// \name Constructors and destructors
  /// @{
public:
  MeshReader(const std::filesystem::path& meshFile, short int dimensions);
  ~MeshReader();
  /// @}

  /// \name API interface that exposes behaviour to the caller
  /// @{
public:
  template <int Index>
  auto readCoordinate() -> CoordinateType;
  auto readConnectivityTable() -> ConnectivityTableType;
  auto readBoundaryConditions() -> BoundaryConditionType;
  auto readBoundaryConditionConnectivity() -> BoundaryConditionConnectivityType;
  /// @}

  /// \name Getters and setters
  /// @{

  /// @}

  /// \name Overloaded operators
  /// @{

  /// @}

  /// \name Private or protected implementation details, not exposed to the caller
  /// @{
private:
  auto checkIfMeshExists() -> void;
  auto getNumberOfBases() -> AIM::Types::UInt;
  auto getNumberOfZones() -> AIM::Types::UInt;
  auto getNumberOfVertices() -> AIM::Types::UInt;
  auto getNumberOfCells() -> AIM::Types::UInt;
  auto getNumberOfSections() -> AIM::Types::UInt;
  auto getNumberOfBoundaryConditions() -> AIM::Types::UInt;
  auto getNumberOfFamilies() -> AIM::Types::UInt;
  auto getCellType(AIM::Types::UInt section) -> CGNS_ENUMT(ElementType_t);
  auto addCurrentCellTypeToConnectivityTable(
    AIM::Types::UInt section, AIM::Types::UInt numberOfVerticesPerCell, ConnectivityTableType& connectivity) -> void;
  auto getNumberOfConnectivitiesForCellType(AIM::Types::UInt section, AIM::Types::UInt numVerticesPerCell)
    -> AIM::Types::UInt;
  auto writeConnectivityTable(AIM::Types::UInt section, AIM::Types::UInt elementSize,
    AIM::Types::UInt numberOfVerticesPerCell, ConnectivityTableType& connectivity) -> void;
  auto getCurrentBoundaryType(AIM::Types::UInt boundary)
    -> std::tuple<CGNS_ENUMT(BCType_t), std::string, AIM::Types::UInt>;
  auto getCurrentFamilyType(AIM::Types::UInt boundary) -> CGNS_ENUMT(BCType_t);
  auto writeBoundaryConnectivityIntoArray(AIM::Types::UInt boundary, BoundaryConditionConnectivityType& bcc) -> void;
  /// @}

  /// \name Encapsulated data (private or protected variables)
  /// @{
private:
  const std::filesystem::path meshFile_{};
  const short int dimensions_{0};
  int fileIndex_{0};
  AIM::Types::UInt numberOfVertices_{0};
  AIM::Types::UInt numberOfCells_{0};
  AIM::Types::UInt numberOfBCs_{0};
  AIM::Types::UInt numberOfFamilies_{0};
  /// @}
};

}  // namespace Mesh
}  // end namespace AIM

#include "meshReading.tpp"