/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "marching-cubes/Tensor3D.hpp"

namespace marchingcubes {

std::vector<double> equidistantPoints(double min, double max, size_t count) {
  assert(min < max);
  assert(count > 1);
  std::vector<double> vec(count);
  double increment = (max - min) / (count - 1);
  for (std::size_t i = 0; i < count; ++i) {
    vec.at(i) = min + increment * i;
  }
  return vec;
}

Grid3D::Grid3D(std::vector<double> x, std::vector<double> y,
               std::vector<double> z)
    : values{{std::move(x), std::move(y), std::move(z)}} {
  for (size_t iDim = 0; iDim < DIM_COUNT; ++iDim) {
    const auto &vals = values.at(iDim);
    assert(vals.size() > 0);
    for (size_t i = 0; i < vals.size() - 1; ++i) {
      assert(vals.at(i) < vals.at(i + 1));
    }
  }
}

Tensor3DIndexer::Tensor3DIndexer(size_t xSize, size_t ySize, size_t zSize)
    : size{{xSize, ySize, zSize}} {
  assert(size[X] > 0);
  assert(size[Y] > 0);
  assert(size[Z] > 0);
}

Tensor3D::Tensor3D(size_t xSize, size_t ySize, size_t zSize,
                   std::vector<double> values)
    : indexer{xSize, ySize, zSize}, values{std::move(values)} {
  assert(size(X) * size(Y) * size(Z) == this->values.size());
}

std::pair<double, double> Tensor3D::minMax() const {
  const auto minMaxIt = std::minmax_element(values.cbegin(), values.cend());
  return std::make_pair(*minMaxIt.first, *minMaxIt.second);
}

Tensor3D createSphere(const Grid3D &grid) {
  auto xPointCount = grid.values[X].size();
  auto yPointCount = grid.values[Y].size();
  auto zPointCount = grid.values[Z].size();
  std::vector<double> values(xPointCount * yPointCount * zPointCount);
  size_t index = 0;
  for (size_t k = 0; k < zPointCount; ++k) {
    double zSquare = grid.values[Z][k] * grid.values[Z][k];
    for (size_t j = 0; j < yPointCount; ++j) {
      double ySquare = grid.values[Y][j] * grid.values[Y][j];
      for (size_t i = 0; i < xPointCount; ++i) {
        double x = grid.values[X][i];
        values[index++] = x * x + ySquare + zSquare;
      }
    }
  }
  return Tensor3D(xPointCount, yPointCount, zPointCount, std::move(values));
}

} // namespace marchingcubes
