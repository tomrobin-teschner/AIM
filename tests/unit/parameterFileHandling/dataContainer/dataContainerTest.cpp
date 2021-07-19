// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

// c++ include headers
#include <string>

// third-party include headers
#include <gtest/gtest.h>

// AIM include headers
#include "src/parameterFileHandling/dataContainer/dataContainer.hpp"

TEST(DataContainerTest, createDataContainerTest) {
  // arrange
  auto dataContainer = AIM::Parameters::DataContainer{"parameterName", "parameterDescription", static_cast<int>(42)};

  // act
  auto name = dataContainer.getName();
  auto description = dataContainer.getDescription();
  auto data = dataContainer.getValue<int>();

  // assert
  EXPECT_STREQ(name.c_str(), "parameterName");
  EXPECT_STREQ(description.c_str(), "parameterDescription");
  EXPECT_EQ(data, 42);
}
