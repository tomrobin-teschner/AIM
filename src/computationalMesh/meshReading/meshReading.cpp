// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

// c++ include headers
#include <algorithm>
#include <cassert>
#include <exception>
#include <filesystem>
#include <iostream>
#include <tuple>
#include <vector>

// third-party include headers
#include "cgnslib.h"

// AIM include headers
#include "src/computationalMesh/meshReading/meshReading.hpp"
#include "src/types/enums.hpp"
#include "src/types/types.hpp"

namespace AIM {
namespace Mesh {

/// \name Constructors and destructors
/// @{
MeshReader::MeshReader(const std::filesystem::path &meshFile, short int dimensions)
    : meshFile_(meshFile), dimensions_(dimensions) {
  checkIfMeshExists();

  assert(dimensions_ == AIM::Enum::Dimension::Two && "Currently only 2D meshes are supported");

  auto errorCode = cg_open(meshFile_.string().c_str(), CG_MODE_READ, &fileIndex_);
  assert(errorCode == 0 && "Error reading CGNS file");

  assert(getNumberOfBases() == 1 && "Currently only single-base mesh is supported");
  assert(getNumberOfZones() == 1 && "Currently only single-zone mesh is supported");

  numberOfVertices_ = getNumberOfVertices();
  numberOfCells_ = getNumberOfCells();
  numberOfBCs_ = getNumberOfBoundaryConditions();
  numberOfFamilies_ = getNumberOfFamilies();
}

MeshReader::~MeshReader() {
  auto errorCode = cg_close(fileIndex_);
  assert(errorCode == 0 && "Error closing CGNS file");
}
/// @}

/// \name API interface that exposes behaviour to the caller
/// @{
auto MeshReader::readConnectivityTable() -> ConnectivityTableType {
  auto connectivity = MeshReader::ConnectivityTableType{};
  connectivity.reserve(numberOfCells_);

  auto numberOfSections = getNumberOfSections();
  for (AIM::Types::UInt section = 0; section < numberOfSections; ++section) {
    auto cellType = getCellType(section);
    if (dimensions_ == AIM::Enum::Dimension::Two) {
      if (cellType == CGNS_ENUMV(TRI_3)) addCurrentCellTypeToConnectivityTable(section, 3u, connectivity);
      if (cellType == CGNS_ENUMV(QUAD_4)) addCurrentCellTypeToConnectivityTable(section, 4u, connectivity);
    } else if (dimensions_ == AIM::Enum::Dimension::Three) {
      throw std::runtime_error("currently 3D mesh reading is not implemented");
    }
  }

  return connectivity;
}

auto MeshReader::readBoundaryConditions() -> BoundaryConditionType {
  auto bc = BoundaryConditionType{};

  for (AIM::Types::UInt boundary = 0; boundary < numberOfBCs_; ++boundary) {
    auto [boundaryConditionType, boundaryName, _] = getCurrentBoundaryType(boundary);
    if (boundaryConditionType == CGNS_ENUMV(FamilySpecified)) {
      auto familyBCType = getCurrentFamilyType(boundary);
      if (familyBCType == CGNS_ENUMV(BCWall))
        bc.push_back(std::make_pair(AIM::Enum::BoundaryCondition::Wall, boundaryName));
      if (familyBCType == CGNS_ENUMV(BCSymmetryPlane))
        bc.push_back(std::make_pair(AIM::Enum::BoundaryCondition::Symmetry, boundaryName));
      if (familyBCType == CGNS_ENUMV(BCInflow))
        bc.push_back(std::make_pair(AIM::Enum::BoundaryCondition::Inlet, boundaryName));
      if (familyBCType == CGNS_ENUMV(BCOutflow))
        bc.push_back(std::make_pair(AIM::Enum::BoundaryCondition::Outlet, boundaryName));
    }
  }

  return bc;
}
auto MeshReader::readBoundaryConditionConnectivity() -> BoundaryConditionConnectivityType {
  auto bcc = BoundaryConditionConnectivityType(numberOfBCs_);
  for (AIM::Types::UInt boundary = 0; boundary < numberOfBCs_; ++boundary)
    writeBoundaryConnectivityIntoArray(boundary, bcc);
  return bcc;
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
auto MeshReader::checkIfMeshExists() -> void {
  try {
    if (!std::filesystem::exists(meshFile_))
      throw std::runtime_error(std::string("can't find the following file: " + meshFile_.string()));
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    std::terminate();
  }
}

auto MeshReader::getNumberOfBases() -> AIM::Types::UInt {
  auto numberOfBases = int{0};
  auto errorCode = cg_nbases(fileIndex_, &numberOfBases);
  assert(errorCode == 0 && "Could not read number of bases from file");
  return static_cast<AIM::Types::UInt>(numberOfBases);
}

auto MeshReader::getNumberOfZones() -> AIM::Types::UInt {
  auto numberOfZones = int{0};
  auto errorCode = cg_nzones(fileIndex_, 1, &numberOfZones);
  assert(errorCode == 0 && "Could not read number of bases from file");
  return static_cast<AIM::Types::UInt>(numberOfZones);
}

auto MeshReader::getNumberOfVertices() -> AIM::Types::UInt {
  AIM::Types::CGNSInt gridSizeProperties[3][1]{};
  char zoneName[64];
  auto errorCode = cg_zone_read(fileIndex_, 1, 1, zoneName, gridSizeProperties[0]);
  assert(errorCode == 0 && "Could not read number of vertices from zone");
  return static_cast<AIM::Types::UInt>(gridSizeProperties[0][0]);
}

auto MeshReader::getNumberOfCells() -> AIM::Types::UInt {
  AIM::Types::CGNSInt gridSizeProperties[3][1]{};
  char zoneName[64];
  auto errorCode = cg_zone_read(fileIndex_, 1, 1, zoneName, gridSizeProperties[0]);
  assert(errorCode == 0 && "Could not read number of cells from zone");
  return static_cast<AIM::Types::UInt>(gridSizeProperties[1][0]);
}

auto MeshReader::getNumberOfSections() -> AIM::Types::UInt {
  auto numberOfSections = int{0};
  auto errorCode = cg_nsections(fileIndex_, 1, 1, &numberOfSections);
  assert(errorCode == 0 && "Could not read number of sections from file");
  assert(numberOfSections > 0 && "No sections found, but required to set up connectivity table!");
  return static_cast<AIM::Types::UInt>(numberOfSections);
}

auto MeshReader::getNumberOfBoundaryConditions() -> AIM::Types::UInt {
  auto numBCs = int{0};
  auto errorCode = cg_nbocos(fileIndex_, 1, 1, &numBCs);
  assert(errorCode == 0 && "Could not read number of boundary conditions from file");
  return static_cast<AIM::Types::UInt>(numBCs);
}

auto MeshReader::getNumberOfFamilies() -> AIM::Types::UInt {
  auto numFamilies = int{0};
  auto errorCode = cg_nfamilies(fileIndex_, 1, &numFamilies);
  assert(errorCode == 0 && "Could not read number of families from file");
  return static_cast<AIM::Types::UInt>(numFamilies);
}

auto MeshReader::getCellType(AIM::Types::UInt section) -> CGNS_ENUMT(ElementType_t) {
  auto begin = AIM::Types::CGNSInt{0};
  auto end = AIM::Types::CGNSInt{0};
  char sectionName[33]{};
  auto indexOfLastElement = int{0};
  auto parentDataExist = int{0};
  auto cellType = CGNS_ENUMT(ElementType_t){};

  auto errorCode = cg_section_read(fileIndex_, 1, 1, static_cast<int>(section + 1), sectionName, &cellType, &begin,
    &end, &indexOfLastElement, &parentDataExist);
  assert(errorCode == 0 && "Could not read section from zone");
  return cellType;
}

auto MeshReader::addCurrentCellTypeToConnectivityTable(
  AIM::Types::UInt section, AIM::Types::UInt numberOfVerticesPerCell, ConnectivityTableType &connectivity) -> void {
  auto numberOfConnectivities = getNumberOfConnectivitiesForCellType(section, numberOfVerticesPerCell);
  writeConnectivityTable(section, numberOfConnectivities, numberOfVerticesPerCell, connectivity);
}

auto MeshReader::getNumberOfConnectivitiesForCellType(
  AIM::Types::UInt section, AIM::Types::UInt numberOfVerticesPerCell) -> AIM::Types::UInt {
  auto elementSize = AIM::Types::CGNSInt{0};
  auto errorCode = cg_ElementDataSize(fileIndex_, 1, 1, static_cast<int>(section + 1), &elementSize);
  assert(errorCode == 0 && "Could not read element size from section");
  assert(static_cast<AIM::Types::UInt>(elementSize) % numberOfVerticesPerCell == 0 &&
         "error reading elements, number of connectivities not divisible by number of vertices per cell");
  return static_cast<AIM::Types::UInt>(elementSize);
}

auto MeshReader::writeConnectivityTable(AIM::Types::UInt section, AIM::Types::UInt elementSize,
  AIM::Types::UInt numberOfVerticesPerCell, ConnectivityTableType &connectivity) -> void {
  auto parentData = AIM::Types::CGNSInt{0};
  auto temp = std::vector<AIM::Types::CGNSInt>{};
  temp.resize(elementSize);

  auto errorCode = cg_elements_read(fileIndex_, 1, 1, static_cast<int>(section + 1), &temp[0], &parentData);
  assert(errorCode == 0 && "Could not read elements from current section");

  auto numberOfElements = elementSize / numberOfVerticesPerCell;
  for (AIM::Types::UInt i = 0; i < numberOfElements; ++i) {
    auto offset = i * numberOfVerticesPerCell;
    auto localElementConnectivity = std::vector<AIM::Types::UInt>{};
    localElementConnectivity.reserve(numberOfElements);
    for (AIM::Types::UInt vertex = 0; vertex < numberOfVerticesPerCell; ++vertex)
      localElementConnectivity.push_back(static_cast<AIM::Types::UInt>(temp[offset + vertex]));
    connectivity.push_back(localElementConnectivity);
  }
}

auto MeshReader::getCurrentBoundaryType(AIM::Types::UInt boundary)
  -> std::tuple<CGNS_ENUMT(BCType_t), std::string, AIM::Types::UInt> {
  int indexOfNormalVector[3], numberOfDatasets;
  char boundaryName[64];
  CGNS_ENUMT(BCType_t) boundaryElementType;
  CGNS_ENUMT(PointSetType_t) pointSetType;
  CGNS_ENUMT(DataType_t) normalVectorType;
  AIM::Types::CGNSInt normalVectorsExistFlag, numberOfBoundaryElements{0};

  auto errorCode =
    cg_boco_info(fileIndex_, 1, 1, static_cast<int>(boundary + 1), boundaryName, &boundaryElementType, &pointSetType,
      &numberOfBoundaryElements, indexOfNormalVector, &normalVectorsExistFlag, &normalVectorType, &numberOfDatasets);
  assert(errorCode == 0 && "Could not read boundary condition from boundary node");
  return {boundaryElementType, std::string(boundaryName), static_cast<AIM::Types::UInt>(numberOfBoundaryElements)};
}

auto MeshReader::getCurrentFamilyType(AIM::Types::UInt boundary) -> CGNS_ENUMT(BCType_t) {
  char familyBCName[64];
  CGNS_ENUMT(BCType_t) ifamilytype;

  auto errorCode = cg_fambc_read(fileIndex_, 1, static_cast<int>(boundary + 2), 1, familyBCName, &ifamilytype);
  assert(errorCode == 0 && "Could not read boundary condition from family node");
  return ifamilytype;
}

auto MeshReader::writeBoundaryConnectivityIntoArray(AIM::Types::UInt boundary, BoundaryConditionConnectivityType &bcc)
  -> void {
  auto normalVectorList = int{0};
  auto boundaryConnectivityTable = std::vector<AIM::Types::CGNSInt>{};
  auto [boundaryElementType, boundaryName, numberOfBoundaryElements] = getCurrentBoundaryType(boundary);
  boundaryConnectivityTable.resize(numberOfBoundaryElements);
  cg_boco_read(fileIndex_, 1, 1, static_cast<int>(boundary + 1), &boundaryConnectivityTable[0], &normalVectorList);
  bcc[boundary] = boundaryConnectivityTable;
}
/// @}

}  // namespace Mesh
}  // end namespace AIM