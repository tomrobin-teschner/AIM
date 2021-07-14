// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

#pragma once

#include "cgnslib.h"

namespace AIM {
namespace Types {

// type used for all floating point numbers
using FloatType = double;

// type used to store large integer numbers (e.g. number of elements / cells) or for indexing arrays
using UInt = unsigned;

// wrapper around int defined in the CGNS library
using CGNSInt = cgsize_t;

}  // namespace Types
}  // end namespace AIM