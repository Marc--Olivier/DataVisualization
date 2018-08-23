/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "marching-cubes/Geometry3D.hpp"

#include <array>
#include <cassert>
#include <vector>

namespace marchingcubes {

/*!
 * \fn equidistantPoints
 * \brief The function equidistantPoints splits a segment [min,max] into
 * count-1 segments of equal length.
 *
 * For example, equidistantPoints(0.0, 5.0, 6) returns the points {0.0, 1.0,
 * 2.0, 3.0, 4.0, 5.0}.
 */
extern std::vector<double> equidistantPoints(double min, double max,
                                             size_t count);

/*!
 * \class Grid3D
 * \brief The class Grid3D defines a 3D grid as the cartesian product of
 * the axis x, y, and z: the points of the grid are all the possible
 * combinations of the provided x, y, and z values.
 */
class Grid3D {
public:
  Grid3D(std::vector<double> x, std::vector<double> y, std::vector<double> z);

public:
  const std::array<std::vector<double>, DIM_COUNT> values;
};

/*!
 * \class Tensor3DIndexer
 * \brief The class Tensor3DIndexer is an utility class used by Tensor3D.
 *
 * A Tensor3D stores all its values in a vector, and this class transforms x, y,
 * z indices to the corresponding vector index `i`.
 */
class Tensor3DIndexer {
public:
  Tensor3DIndexer(size_t xSize, size_t ySize, size_t zSize);

public:
  const std::array<size_t, DIM_COUNT> size;

  inline size_t index(size_t x, size_t y, size_t z) const {
    assert(x < size[X]);
    assert(y < size[Y]);
    assert(z < size[Z]);
    return x + y * size[X] + z * size[X] * size[Y];
  }
};

/*!
 * \class Tensor3D
 * \brief The class Tensor3D stores values of a 3D tensor on a 3D grid.
 *
 * Although the values are stored in a vector of size xSize * ySize * zSize,
 * the values are accessed by passing x,y,z indices. The class Tensor3DIndexer
 * takes care of converting these x,y,z indices into the corresponding vector
 * index `i`.
 */
class Tensor3D {

public:
  Tensor3D(size_t xSize, size_t ySize, size_t zSize,
           std::vector<double> values);
  Tensor3D(const Tensor3D &) = delete;
  Tensor3D(Tensor3D &&) = default;

public:
  size_t size(size_t dimIndex) const { return indexer.size.at(dimIndex); }
  inline size_t index(size_t x, size_t y, size_t z) const {
    return indexer.index(x, y, z);
  }
  inline double value(size_t x, size_t y, size_t z) const {
    return values[index(x, y, z)];
  }
  std::pair<double, double> minMax() const;
  const std::vector<double> &allValues() const { return values; }

private:
  const Tensor3DIndexer indexer;
  std::vector<double> values;
};

extern Tensor3D createSphere(const Grid3D &grid);

} // namespace marchingcubes
