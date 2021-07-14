// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

// c++ include headers
#include <filesystem>
#include <string>

// third-party include headers
#include <gtest/gtest.h>

// AIM include headers
#include "src/computationalMesh/meshReading/meshReading.hpp"
#include "src/types/enums.hpp"

class MeshReadingFixture : public ::testing::Test {
public:
  MeshReadingFixture() {}

protected:
  AIM::Mesh::MeshReader meshReader_{std::filesystem::path("test2D.cgns"), AIM::Enum::Dimension::Two};
};

TEST_F(MeshReadingFixture, testReadCoordinates) {
  // arrange

  // act
  const auto sutX = meshReader_.readCoordinate<AIM::Enum::Coordinate::X>();
  const auto sutY = meshReader_.readCoordinate<AIM::Enum::Coordinate::Y>();

  // assert
  EXPECT_EQ(sutX.size(), 10);
  EXPECT_EQ(sutY.size(), 10);

  EXPECT_DOUBLE_EQ(sutX[0], 1.0);
  EXPECT_DOUBLE_EQ(sutX[1], 0.5);
  EXPECT_DOUBLE_EQ(sutX[2], 0.0);
  EXPECT_DOUBLE_EQ(sutX[3], 0.0);
  EXPECT_DOUBLE_EQ(sutX[4], 0.0);
  EXPECT_DOUBLE_EQ(sutX[5], 0.5);
  EXPECT_DOUBLE_EQ(sutX[6], 1.0);
  EXPECT_DOUBLE_EQ(sutX[7], 1.0);
  EXPECT_DOUBLE_EQ(sutX[8], 0.5);
  EXPECT_DOUBLE_EQ(sutX[9], 0.5);

  EXPECT_DOUBLE_EQ(sutY[0], 1.0);
  EXPECT_DOUBLE_EQ(sutY[1], 1.0);
  EXPECT_DOUBLE_EQ(sutY[2], 1.0);
  EXPECT_DOUBLE_EQ(sutY[3], 0.1);
  EXPECT_DOUBLE_EQ(sutY[4], 0.0);
  EXPECT_DOUBLE_EQ(sutY[5], 0.0);
  EXPECT_DOUBLE_EQ(sutY[6], 0.0);
  EXPECT_DOUBLE_EQ(sutY[7], 0.1);
  EXPECT_NEAR(sutY[8], 0.40577137, 1e-4);
  EXPECT_DOUBLE_EQ(sutY[9], 0.1);
}

TEST_F(MeshReadingFixture, testConnectivityTable) {
  // arrange

  // act
  const auto sut = meshReader_.readConnectivityTable();

  // assert
  auto rhs = std::vector<int>{9, 3, 4};
  EXPECT_EQ(sut[0], rhs);
  rhs = std::vector<int>{9, 1, 2};
  EXPECT_EQ(sut[1], rhs);
  rhs = std::vector<int>{9, 2, 3};
  EXPECT_EQ(sut[2], rhs);
  rhs = std::vector<int>{9, 10, 8};
  EXPECT_EQ(sut[3], rhs);
  rhs = std::vector<int>{9, 8, 1};
  EXPECT_EQ(sut[4], rhs);
  rhs = std::vector<int>{4, 10, 9};
  EXPECT_EQ(sut[5], rhs);
  rhs = std::vector<int>{5, 6, 10, 4};
  EXPECT_EQ(sut[6], rhs);
  rhs = std::vector<int>{6, 7, 8, 10};
  EXPECT_EQ(sut[7], rhs);
}

TEST_F(MeshReadingFixture, readBoundaryConditionsInfo) {
  // arrange

  // act
  const auto sut = meshReader_.readBoundaryConditions();

  // assert
  EXPECT_EQ(sut.size(), 4);
  EXPECT_EQ(sut[0].first, AIM::Enum::BoundaryCondition::Wall);
  EXPECT_EQ(sut[1].first, AIM::Enum::BoundaryCondition::Inlet);
  EXPECT_EQ(sut[2].first, AIM::Enum::BoundaryCondition::Outlet);
  EXPECT_EQ(sut[3].first, AIM::Enum::BoundaryCondition::Symmetry);

  EXPECT_STREQ(sut[0].second.c_str(), "bottom");
  EXPECT_STREQ(sut[1].second.c_str(), "left");
  EXPECT_STREQ(sut[2].second.c_str(), "right");
  EXPECT_STREQ(sut[3].second.c_str(), "top");
}

TEST_F(MeshReadingFixture, readBoundaryConditionConnectivity) {
  // arrange

  // act
  const auto sut = meshReader_.readBoundaryConditionConnectivity();

  // assert
  EXPECT_EQ(sut.size(), 4);
  EXPECT_EQ(sut[0].size(), 2);
  EXPECT_EQ(sut[1].size(), 2);
  EXPECT_EQ(sut[2].size(), 2);
  EXPECT_EQ(sut[3].size(), 2);

  EXPECT_EQ(sut[0][0], 9);
  EXPECT_EQ(sut[0][1], 10);
  EXPECT_EQ(sut[1][0], 11);
  EXPECT_EQ(sut[1][1], 12);
  EXPECT_EQ(sut[2][0], 13);
  EXPECT_EQ(sut[2][1], 14);
  EXPECT_EQ(sut[3][0], 15);
  EXPECT_EQ(sut[3][1], 16);
}