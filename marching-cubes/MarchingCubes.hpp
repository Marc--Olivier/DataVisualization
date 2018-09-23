/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "marching-cubes/Geometry3D.hpp"
#include "marching-cubes/Triangle.hpp"

#include <memory>
#include <vector>

namespace marchingcubes {

class Grid3D;
class Tensor3D;
using Triangle3D = triangle::Type<Point3D>;

/*!
 * \class MarchingCubes
 * \brief The MarchingCubes class calculates isosurfaces as triangles for a
 * given 3D tensor applied on a given 3D grid domain.
 *
 * Note that this class hides some dependencies by using the private
 * implementation idiom.
 */
class MarchingCubes {

public:
  MarchingCubes();
  ~MarchingCubes();

  std::vector<Triangle3D> isoSurface(const Grid3D &grid, const Tensor3D &tensor,
                                     double isoValue) const;

private:
  const std::unique_ptr<class MarchingCubesImpl> pImpl;
};

} // namespace marchingcubes
