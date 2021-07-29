// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

// c++ include headers
#include <filesystem>
#include <string>

// third-party include headers
#include <gtest/gtest.h>

// AIM include headers
#include "src/computationalMesh/computationalMesh/computationalMesh.hpp"
#include "src/computationalMesh/meshReading/meshReading.hpp"
#include "src/types/enums.hpp"
#include "src/types/types.hpp"

class ComputationalMeshFixture : public ::testing::Test {
public:
  ComputationalMeshFixture() {}

protected:
  AIM::Mesh::MeshReader meshReader_{AIM::Enum::Dimension::Two};
};

TEST_F(ComputationalMeshFixture, testReadCoordinates) {
  // arrange
  auto sut = AIM::Mesh::ComputationalMesh{meshReader_};

  // act
  const auto coordinateX = sut.getCoordinateX();
  const auto coordinateY = sut.getCoordinateY();

  // assert
  EXPECT_EQ(coordinateX.size(), 10);
  EXPECT_EQ(coordinateY.size(), 10);

  EXPECT_DOUBLE_EQ(coordinateX[0], 1.0);
  EXPECT_DOUBLE_EQ(coordinateX[1], 0.5);
  EXPECT_DOUBLE_EQ(coordinateX[2], 0.0);
  EXPECT_DOUBLE_EQ(coordinateX[3], 0.0);
  EXPECT_DOUBLE_EQ(coordinateX[4], 0.0);
  EXPECT_DOUBLE_EQ(coordinateX[5], 0.5);
  EXPECT_DOUBLE_EQ(coordinateX[6], 1.0);
  EXPECT_DOUBLE_EQ(coordinateX[7], 1.0);
  EXPECT_DOUBLE_EQ(coordinateX[8], 0.5);
  EXPECT_DOUBLE_EQ(coordinateX[9], 0.5);

  EXPECT_DOUBLE_EQ(coordinateY[0], 1.0);
  EXPECT_DOUBLE_EQ(coordinateY[1], 1.0);
  EXPECT_DOUBLE_EQ(coordinateY[2], 1.0);
  EXPECT_DOUBLE_EQ(coordinateY[3], 0.1);
  EXPECT_DOUBLE_EQ(coordinateY[4], 0.0);
  EXPECT_DOUBLE_EQ(coordinateY[5], 0.0);
  EXPECT_DOUBLE_EQ(coordinateY[6], 0.0);
  EXPECT_DOUBLE_EQ(coordinateY[7], 0.1);
  EXPECT_NEAR(coordinateY[8], 0.40577137, 1e-4);
  EXPECT_DOUBLE_EQ(coordinateY[9], 0.1);
}

TEST_F(ComputationalMeshFixture, testConnectivityTable) {
  // arrange
  auto sut = AIM::Mesh::ComputationalMesh{meshReader_};

  // act
  const auto connectivityTable = sut.getConnectivityTable();

  // assert
  auto rhs = std::vector<AIM::Types::UInt>{9, 3, 4};
  EXPECT_EQ(connectivityTable[0], rhs);
  rhs = std::vector<AIM::Types::UInt>{9, 1, 2};
  EXPECT_EQ(connectivityTable[1], rhs);
  rhs = std::vector<AIM::Types::UInt>{9, 2, 3};
  EXPECT_EQ(connectivityTable[2], rhs);
  rhs = std::vector<AIM::Types::UInt>{9, 10, 8};
  EXPECT_EQ(connectivityTable[3], rhs);
  rhs = std::vector<AIM::Types::UInt>{9, 8, 1};
  EXPECT_EQ(connectivityTable[4], rhs);
  rhs = std::vector<AIM::Types::UInt>{4, 10, 9};
  EXPECT_EQ(connectivityTable[5], rhs);
  rhs = std::vector<AIM::Types::UInt>{5, 6, 10, 4};
  EXPECT_EQ(connectivityTable[6], rhs);
  rhs = std::vector<AIM::Types::UInt>{6, 7, 8, 10};
  EXPECT_EQ(connectivityTable[7], rhs);
}

TEST_F(ComputationalMeshFixture, readBoundaryConditionsInfo) {
  // arrange
  auto sut = AIM::Mesh::ComputationalMesh{meshReader_};

  // act
  const auto boundaryConditionInfo = sut.getBoundaryConditionInfo();

  // assert
  EXPECT_EQ(boundaryConditionInfo.size(), 4);
  EXPECT_EQ(boundaryConditionInfo[0].first, AIM::Enum::BoundaryCondition::Wall);
  EXPECT_EQ(boundaryConditionInfo[1].first, AIM::Enum::BoundaryCondition::Inlet);
  EXPECT_EQ(boundaryConditionInfo[2].first, AIM::Enum::BoundaryCondition::Outlet);
  EXPECT_EQ(boundaryConditionInfo[3].first, AIM::Enum::BoundaryCondition::Symmetry);

  EXPECT_STREQ(boundaryConditionInfo[0].second.c_str(), "bottom");
  EXPECT_STREQ(boundaryConditionInfo[1].second.c_str(), "left");
  EXPECT_STREQ(boundaryConditionInfo[2].second.c_str(), "right");
  EXPECT_STREQ(boundaryConditionInfo[3].second.c_str(), "top");
}

TEST_F(ComputationalMeshFixture, readBoundaryConditionConnectivity) {
  // arrange
  auto sut = AIM::Mesh::ComputationalMesh{meshReader_};

  // act
  const auto boundaryConditionConnectivity = sut.getBoundaryConditionConnvectivity();

  // assert
  EXPECT_EQ(boundaryConditionConnectivity.size(), 4);
  EXPECT_EQ(boundaryConditionConnectivity[0].size(), 2);
  EXPECT_EQ(boundaryConditionConnectivity[1].size(), 2);
  EXPECT_EQ(boundaryConditionConnectivity[2].size(), 2);
  EXPECT_EQ(boundaryConditionConnectivity[3].size(), 2);

  EXPECT_EQ(boundaryConditionConnectivity[0][0], 9);
  EXPECT_EQ(boundaryConditionConnectivity[0][1], 10);
  EXPECT_EQ(boundaryConditionConnectivity[1][0], 11);
  EXPECT_EQ(boundaryConditionConnectivity[1][1], 12);
  EXPECT_EQ(boundaryConditionConnectivity[2][0], 13);
  EXPECT_EQ(boundaryConditionConnectivity[2][1], 14);
  EXPECT_EQ(boundaryConditionConnectivity[3][0], 15);
  EXPECT_EQ(boundaryConditionConnectivity[3][1], 16);
}