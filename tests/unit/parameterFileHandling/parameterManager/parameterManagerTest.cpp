// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

// c++ include headers

// third-party include headers
#include <gtest/gtest.h>

// AIM include headers
#include "src/parameterFileHandling/dataContainer/dataContainer.hpp"
#include "src/parameterFileHandling/parameterManager/parameterManager.hpp"

TEST(ParameterManagerTest, expectThrowForEmpytParameterNodeTest) {
  // arrange
  auto& manager = AIM::Parameters::ParameterManager::getInstance();

  // act
  // empty parameter node, retriving anything should throw

  // assert
  EXPECT_THROW(manager.find("root/anything"), std::runtime_error);
}

TEST(ParameterManagerTest, expectThrowForMissingParameterTest) {
  // arrange
  auto& manager = AIM::Parameters::ParameterManager::getInstance();

  // act
  manager.insert("existing/parameter", AIM::Parameters::DataContainer{"", "", 0});

  // assert
  EXPECT_THROW(manager.find("missing/parameter"), std::runtime_error);
}

TEST(ParameterManagerTest, insertionAndRetrivalTest) {
  // arrange
  auto& manager = AIM::Parameters::ParameterManager::getInstance();

  // act
  manager.insert("root/parameter1", AIM::Parameters::DataContainer{"name", "description", 42});

  // assert
  EXPECT_STREQ(manager.find("root/parameter1").getName().c_str(), "name");
  EXPECT_STREQ(manager.find("root/parameter1").getDescription().c_str(), "description");
  EXPECT_EQ(manager.find("root/parameter1").getValue<int>(), 42);
}
