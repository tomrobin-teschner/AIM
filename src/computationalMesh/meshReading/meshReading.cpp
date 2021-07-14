// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

// c++ include headers
#include <algorithm>
#include <cassert>
#include <exception>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <tuple>
#include <vector>

// third-party include headers
#include "cgnslib.h"

// AIM include headers
#include "src/computationalMesh/meshReading/meshReading.hpp"
#include "src/types/enums.hpp"

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
  for (int section = 0; section < numberOfSections; ++section) {
    auto cellType = getCellType(section);
    if (dimensions_ == AIM::Enum::Dimension::Two) {
      if (cellType == CGNS_ENUMV(TRI_3)) addCurrentCellTypeToConnectivityTable(section, 3, connectivity);
      if (cellType == CGNS_ENUMV(QUAD_4)) addCurrentCellTypeToConnectivityTable(section, 4, connectivity);
    } else if (dimensions_ == AIM::Enum::Dimension::Three) {
      throw std::runtime_error("currently 3D mesh reading is not implemented");
    }
  }

  return connectivity;
}

auto MeshReader::readBoundaryConditions() -> BoundaryConditionType {
  auto bc = BoundaryConditionType{};

  for (int boundary = 0; boundary < numberOfBCs_; ++boundary) {
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
  for (int boundary = 0; boundary < numberOfBCs_; ++boundary) writeBoundaryConnectivityIntoArray(boundary, bcc);
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

auto MeshReader::getNumberOfBases() -> int {
  int numberOfBases{0};
  auto errorCode = cg_nbases(fileIndex_, &numberOfBases);
  assert(errorCode == 0 && "Could not read number of bases from file");
  return numberOfBases;
}

auto MeshReader::getNumberOfZones() -> int {
  int numberOfZones{0};
  auto errorCode = cg_nzones(fileIndex_, 1, &numberOfZones);
  assert(errorCode == 0 && "Could not read number of bases from file");
  return numberOfZones;
}

auto MeshReader::getNumberOfVertices() -> unsigned {
  cgsize_t gridSizeProperties[3][1]{};
  char zoneName[64];
  auto errorCode = cg_zone_read(fileIndex_, 1, 1, zoneName, gridSizeProperties[0]);
  assert(errorCode == 0 && "Could not read number of vertices from zone");
  return gridSizeProperties[0][0];
}

auto MeshReader::getNumberOfCells() -> unsigned {
  cgsize_t gridSizeProperties[3][1]{};
  char zoneName[64];
  auto errorCode = cg_zone_read(fileIndex_, 1, 1, zoneName, gridSizeProperties[0]);
  assert(errorCode == 0 && "Could not read number of cells from zone");
  return gridSizeProperties[1][0];
}

auto MeshReader::getNumberOfSections() -> unsigned {
  auto numberOfSections = int{0};
  auto errorCode = cg_nsections(fileIndex_, 1, 1, &numberOfSections);
  assert(errorCode == 0 && "Could not read number of sections from file");
  assert(numberOfSections > 0 && "No sections found, but required to set up connectivity table!");
  return numberOfSections;
}

auto MeshReader::getNumberOfBoundaryConditions() -> unsigned {
  auto numBCs = cgsize_t{0};
  auto errorCode = cg_nbocos(fileIndex_, 1, 1, &numBCs);
  assert(errorCode == 0 && "Could not read number of boundary conditions from file");
  return numBCs;
}

auto MeshReader::getNumberOfFamilies() -> unsigned {
  auto numFamilies = cgsize_t{0};
  auto errorCode = cg_nfamilies(fileIndex_, 1, &numFamilies);
  assert(errorCode == 0 && "Could not read number of families from file");
  return numFamilies;
}

auto MeshReader::getCellType(int section) -> CGNS_ENUMT(ElementType_t) {
  auto begin = cgsize_t{0};
  auto end = cgsize_t{0};
  char sectionName[33]{};
  auto nbndry = int{0};
  auto iparentFlag = int{0};
  auto cellType = CGNS_ENUMT(ElementType_t){};

  auto errorCode =
    cg_section_read(fileIndex_, 1, 1, section + 1, sectionName, &cellType, &begin, &end, &nbndry, &iparentFlag);
  assert(errorCode == 0 && "Could not read section from zone");
  return cellType;
}

auto MeshReader::addCurrentCellTypeToConnectivityTable(
  int section, int numberOfVerticesPerCell, ConnectivityTableType &connectivity) -> void {
  auto numberOfConnectivities = getNumberOfConnectivitiesForCellType(section, numberOfVerticesPerCell);
  addCurrentCellTypeToConnectivityTable(section, numberOfConnectivities, numberOfVerticesPerCell, connectivity);
}

auto MeshReader::getNumberOfConnectivitiesForCellType(int section, short int numberOfVerticesPerCell) -> unsigned {
  auto elementSize = cgsize_t{0};
  auto errorCode = cg_ElementDataSize(fileIndex_, 1, 1, section + 1, &elementSize);
  assert(errorCode == 0 && "Could not read element size from section");
  assert(elementSize % numberOfVerticesPerCell == 0 &&
         "error reading elements, number of connectivities not divisible by number of vertices per cell");
  return elementSize;
}

auto MeshReader::addCurrentCellTypeToConnectivityTable(
  int section, unsigned elementSize, int numberOfVerticesPerCell, ConnectivityTableType &connectivity) -> void {
  auto iparentdata = cgsize_t{0};
  auto temp = std::vector<cgsize_t>{};
  temp.resize(elementSize);

  auto errorCode = cg_elements_read(fileIndex_, 1, 1, section + 1, &temp[0], &iparentdata);

  auto numberOfElements = elementSize / numberOfVerticesPerCell;
  for (unsigned i = 0; i < numberOfElements; ++i) {
    auto offset = i * numberOfVerticesPerCell;
    auto localElementConnectivity = std::vector<cgsize_t>{};
    localElementConnectivity.reserve(numberOfElements);
    for (int vertex = 0; vertex < numberOfVerticesPerCell; ++vertex)
      localElementConnectivity.push_back(temp[offset + vertex]);
    connectivity.push_back(localElementConnectivity);
  }
}

auto MeshReader::getCurrentBoundaryType(int boundary) -> std::tuple<CGNS_ENUMT(BCType_t), std::string, cgsize_t> {
  int normalindex[3], ndataset;
  char boconame[64];
  CGNS_ENUMT(BCType_t) ibocotype;
  CGNS_ENUMT(PointSetType_t) iptset;
  CGNS_ENUMT(DataType_t) normaldatatype;
  cgsize_t npts, normallistflag;

  auto errorCode = cg_boco_info(fileIndex_, 1, 1, boundary + 1, boconame, &ibocotype, &iptset, &npts, normalindex,
    &normallistflag, &normaldatatype, &ndataset);
  assert(errorCode == 0 && "Could not read boundary condition from boundary node");
  return {ibocotype, std::string(boconame), npts};
}
auto MeshReader::getCurrentFamilyType(int boundary) -> CGNS_ENUMT(BCType_t) {
  char familyBCName[64];
  CGNS_ENUMT(BCType_t) ifamilytype;

  auto errorCode = cg_fambc_read(fileIndex_, 1, boundary + 2, 1, familyBCName, &ifamilytype);
  assert(errorCode == 0 && "Could not read boundary condition from family node");
  return ifamilytype;
}
auto MeshReader::writeBoundaryConnectivityIntoArray(int boundary, BoundaryConditionConnectivityType &bcc) -> void {
  auto normallist = int{0};
  auto ipnts = std::vector<cgsize_t>{};
  auto [ibocotype, boconame, npts] = getCurrentBoundaryType(boundary);
  ipnts.resize(npts);
  cg_boco_read(fileIndex_, 1, 1, boundary + 1, &ipnts[0], &normallist);
  bcc[boundary] = ipnts;
}
/// @}

}  // namespace Mesh
}  // end namespace AIM