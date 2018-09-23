/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "marching-cubes/AllConfigs.hpp"

namespace marchingcubes {

/*!
 * \class BaseConfigs
 * \brief The class BaseConfigs contains the 15 base cube configurations
 * that are used to calculate all 256 cube configurations.
 */
class BaseConfigs {

public:
  BaseConfigs();

public:
  const std::array<cube::Configuration, 15> configs;
  const std::array<TrianglesOnCubeEdges, 15> triangles;
  const std::array<std::size_t, 6> startIndicesByTrueValueCount;
};

/*!
 * \class ConfigsGenerator
 * \brief The class ConfigsGenerator can generate AllConfigs objects that
 * contains all 256 cube configurations from the 15 base cube configurations.
 */
class ConfigsGenerator {

public:
  ConfigsGenerator(const BaseConfigs baseConfigs);

  AllConfigs generateConfigs() const;

  cube::Permutation findSimilarConfiguration(cube::Configuration config) const;

public:
  const BaseConfigs baseConfigs;

private:
  const std::array<cube::Permutation, 48> allowedPermutations;
};

} // namespace marchingcubes
