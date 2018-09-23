/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "marching-cubes/Cube.hpp"
#include "marching-cubes/Triangle.hpp"
#include "utils/TinyVector.hpp"

namespace marchingcubes {

constexpr const std::size_t CONFIGS_COUNT = 256;

/*!
 * \class TriangleOnCubeEdges
 * \brief The alias TriangleOnCubeEdges represents the edges that contain
 * the vertices of a triangle used to approximate the isosurface within a cube.
 */
using TriangleOnCubeEdges =
    utils::TinyArray<cube::Edge, cube::ONE_EDGE_SIZE, triangle::POINT_COUNT>;

constexpr auto OneTriangleOnCubeEdgesSize =
    cube::ONE_EDGE_SIZE * triangle::POINT_COUNT;

/*!
 * \class TrianglesOnCubeEdges
 * \brief The alias TrianglesOnCubeEdges represents the triangles
 * that approximate the isosurface within a cube.
 */
using TrianglesOnCubeEdges =
    utils::TinyVector<TriangleOnCubeEdges, OneTriangleOnCubeEdgesSize>;

/*!
 * \class AllConfigs
 * \brief The class AllConfigs contains all 256 cube configurations and the
 * triangles approximating the isosurface within the corresponding cube.
 */
class AllConfigs {

public:
  AllConfigs(const std::array<TrianglesOnCubeEdges, CONFIGS_COUNT> &triangles);

public:
  const std::array<TrianglesOnCubeEdges, CONFIGS_COUNT> triangles;
};

} // namespace marchingcubes
