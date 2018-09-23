/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "marching-cubes/MarchingCubes.hpp"

#include "marching-cubes/ConfigsGenerator.hpp"
#include "marching-cubes/Tensor3D.hpp"

namespace marchingcubes {

constexpr auto VERTEX_COUNT = cube::VERTEX_COUNT;

/*!
 * \class MarchingCubesImpl
 * \brief The MarchingCubesImpl class is the actual class that performs the
 * calculation of isosurfaces for a given 3D tensor applied on a given 3D grid
 * domain.
 */
class MarchingCubesImpl {

public:
  MarchingCubesImpl();

  std::vector<Triangle3D> isoSurface(const Grid3D &grid, const Tensor3D &tensor,
                                     double isoValue) const;

private:
  static bool areGridAndTensorConsistent(const Grid3D &grid,
                                         const Tensor3D &tensor);

private:
  const AllConfigs configs;
};

MarchingCubesImpl::MarchingCubesImpl()
    : configs{ConfigsGenerator{BaseConfigs{}}.generateConfigs()} {}

bool MarchingCubesImpl::areGridAndTensorConsistent(const Grid3D &grid,
                                                   const Tensor3D &tensor) {
  for (size_t iDim = 0; iDim < DIM_COUNT; ++iDim) {
    if (grid.values[iDim].size() != tensor.size(iDim))
      return false;
  }
  return true;
}

static double offset(const std::array<double, VERTEX_COUNT> &valuesOnCube,
                     cube::Edge edge) {
  auto localOffset = [](double start, double end) {
    assert(start != end); // May explode...
    return start / (start - end);
  };
  using namespace cube;
  switch (edge) {
  case e01:
    return localOffset(valuesOnCube[0], valuesOnCube[1]);
  case e02:
    return localOffset(valuesOnCube[0], valuesOnCube[2]);
  case e04:
    return localOffset(valuesOnCube[0], valuesOnCube[4]);
  case e13:
    return localOffset(valuesOnCube[1], valuesOnCube[3]);
  case e15:
    return localOffset(valuesOnCube[1], valuesOnCube[5]);
  case e23:
    return localOffset(valuesOnCube[2], valuesOnCube[3]);
  case e26:
    return localOffset(valuesOnCube[2], valuesOnCube[6]);
  case e37:
    return localOffset(valuesOnCube[3], valuesOnCube[7]);
  case e45:
    return localOffset(valuesOnCube[4], valuesOnCube[5]);
  case e46:
    return localOffset(valuesOnCube[4], valuesOnCube[6]);
  case e57:
    return localOffset(valuesOnCube[5], valuesOnCube[7]);
  case e67:
    return localOffset(valuesOnCube[6], valuesOnCube[7]);
  }
}

std::vector<Triangle3D> MarchingCubesImpl::isoSurface(const Grid3D &grid,
                                                      const Tensor3D &tensor,
                                                      double isoValue) const {
  assert(areGridAndTensorConsistent(grid, tensor));
  std::vector<Triangle3D> triangles;
  triangles.reserve(10000);
  const auto &gridX = grid.values.at(X);
  const auto &gridY = grid.values.at(Y);
  const auto &gridZ = grid.values.at(Z);
  /**
   *      6_____7
   *     /|    /|        z
   *    4_____5 |        |  y
   *    | |   | |        | /
   *    | |   | |        |/
   *    | |   | |        #----- x
   *    | 2___|_3
   *    |/    |/
   *    0_____1
   */
  const std::vector<double> &values = tensor.allValues();
  for (size_t iZ = 1; iZ < tensor.size(Z); ++iZ) {
    for (size_t iY = 1; iY < tensor.size(Y); ++iY) {

      cube::Cube3D cube3D{{{std::make_pair(gridX[0], gridX[1]),
                            std::make_pair(gridY[iY - 1], gridY[iY]),
                            std::make_pair(gridZ[iZ - 1], gridZ[iZ])}}};

      auto v1 = values.begin();
      std::advance(v1, static_cast<long>(tensor.index(0, iY - 1, iZ - 1)));
      auto v3 = values.cbegin();
      std::advance(v3, static_cast<long>(tensor.index(0, iY, iZ - 1)));
      auto v5 = values.cbegin();
      std::advance(v5, static_cast<long>(tensor.index(0, iY - 1, iZ)));
      auto v7 = values.cbegin();
      std::advance(v7, static_cast<long>(tensor.index(0, iY, iZ)));
      std::array<double, VERTEX_COUNT> cubeValues{
          {0.0, *v1 - isoValue, 0.0, *v3 - isoValue, 0.0, *v5 - isoValue, 0.0,
           *v7 - isoValue}};
      auto bitConfigSet = [&cubeValues](int index) {
        return (cubeValues[static_cast<size_t>(index)] < 0.0 ? 0 : 1) << index;
      };
      auto bitsToKeepFromPreviousConfig = static_cast<uint8_t>(0b10101010);
      auto configBitSet =
          static_cast<uint8_t>(bitConfigSet(1) | bitConfigSet(3) |
                               bitConfigSet(5) | bitConfigSet(7));
      for (size_t iX = 1; iX < tensor.size(X); ++iX) {
        cube3D.setMinMax(X, gridX[iX - 1], gridX[iX]);
        cubeValues[0] = cubeValues[1];
        cubeValues[2] = cubeValues[3];
        cubeValues[4] = cubeValues[5];
        cubeValues[6] = cubeValues[7];
        cubeValues[1] = *(++v1) - isoValue;
        cubeValues[3] = *(++v3) - isoValue;
        cubeValues[5] = *(++v5) - isoValue;
        cubeValues[7] = *(++v7) - isoValue;
        configBitSet = (configBitSet & bitsToKeepFromPreviousConfig) >> 1 |
                       static_cast<uint8_t>(bitConfigSet(1) | bitConfigSet(3) |
                                            bitConfigSet(5) | bitConfigSet(7));

        auto configIndex = cube::Configuration(configBitSet).toUint8();
        auto trianglesOnEdges = configs.triangles.at(configIndex);

        /*
        std::string ixyz = "(" + std::to_string(iX) + "," + std::to_string(iY) +
                           "," + std::to_string(iZ) + ")";
        std::cout << ixyz << " configBitSet: " << std::bitset<8>{configIndex}
                  << " #triangles: " << triangles.size() << " - "
                  << triangles.size() + trianglesOnEdges.size() - 1
                  << std::endl;
        */

        auto toTriangle3D = [&](TriangleOnCubeEdges triangleOnEdges) {
          Triangle3D triangle3D;
          for (size_t i = 0; i < triangle3D.size(); ++i) {
            auto edge = triangleOnEdges[i];
            auto intersectionOffset = offset(cubeValues, edge);
            triangle3D[i] = cube3D.interpolatedPoint(edge, intersectionOffset);
          }
          return triangle3D;
        };

        for (size_t i = 0; i < trianglesOnEdges.size(); ++i) {
          triangles.push_back(toTriangle3D(trianglesOnEdges[i]));
        }
      }
    }
  }
  triangles.shrink_to_fit();
  return triangles;
}

MarchingCubes::MarchingCubes() : pImpl{new MarchingCubesImpl()} {}

MarchingCubes::~MarchingCubes() = default;

std::vector<Triangle3D> MarchingCubes::isoSurface(const Grid3D &grid,
                                                  const Tensor3D &tensor,
                                                  double isoValue) const {
  return pImpl->isoSurface(grid, tensor, isoValue);
}

} // namespace marchingcubes
