// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021-present. This file is distribuited under the MIT license.

// c++ include headers
#include <filesystem>

// third-party include headers
#include <gtest/gtest.h>

// AIM include headers
#include "src/parameterFileReading/parameterFileReading.hpp"

TEST(parameterFileReadingTest, readParameterFromFileTest) {
  // arrange
  auto file = std::filesystem::path("aim.json");

  // act
  auto meshFilename = AIM::Parameters::ParameterFileReading::readParameterOrGetDefaultValue<std::string>(
    file, "/mesh/filename", std::nullopt);

  // assert
  EXPECT_STREQ(meshFilename.c_str(), "input/mesh.cgns");
}

TEST(parameterFileReadingTest, getDefaultValueForNonExistingParameterTest) {
  // arrange
  auto file = std::filesystem::path("aim.json");

  // act
  auto defaultValue = AIM::Parameters::ParameterFileReading::readParameterOrGetDefaultValue<std::string>(
    file, "/non/existing/parameter", "success");

  // assert
  EXPECT_STREQ(defaultValue.c_str(), "success");
}

TEST(parameterFileReadingTest, expectThrowForMissingParameterWithoutDefaultValueTest) {
  // arrange
  auto file = std::filesystem::path("aim.json");

  // act
  // act method in assert section as exception needs to be caught

  // assert
  EXPECT_THROW(AIM::Parameters::ParameterFileReading::readParameterOrGetDefaultValue<std::string>(
                 file, "/non/existing/parameter", std::nullopt),
    std::runtime_error);
}