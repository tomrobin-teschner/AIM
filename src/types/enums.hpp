// This file is part of Artificial-based Incompressibile Methods (AIM), a CFD solver for exact projection
// methods based on hybrid Artificial compressibility and Pressure Projection methods.
// (c) by Tom-Robin Teschner 2021. This file is distribuited under the MIT license.

#pragma once

namespace AIM {
namespace Enum {

enum Dimension { Two = 2, Three = 3 };
enum Coordinate { X = 0, Y, Z };
enum BoundaryCondition { Wall = 0, Inlet, Outlet, Symmetry };

}  // namespace Enum
}  // end namespace AIM