/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "marching-cubes/ConfigsGenerator.hpp"

#include <catch2/catch.hpp>

namespace marchingcubes::tests {

SCENARIO("Validate base configuration triangles") {
  const auto baseConfigs = BaseConfigs();
  WHEN("I retrieve the triangles of a base configuration") {
    THEN("The start and end vertices of each intersection with an edge must "
         "have opposite signs") {
      for (size_t iConfig = 0; iConfig < baseConfigs.triangles.size();
           ++iConfig) {
        const auto config = baseConfigs.configs.at(iConfig);
        INFO("Config #" + std::to_string(iConfig) + ": " + config.to_string());
        const auto &triangles = baseConfigs.triangles.at(iConfig);
        for (size_t iTriangle = 0; iTriangle < triangles.size(); ++iTriangle) {
          INFO("Triangle #" + std::to_string(iTriangle));
          const auto &triangle = triangles[iTriangle];
          for (size_t iEdge = 0; iEdge < triangle.size(); ++iEdge) {
            const auto edge = triangle[iEdge];
            INFO("Edge #" + std::to_string(iEdge) + ": " + to_string(edge));
            REQUIRE(config[toInt(startVertex(edge))] !=
                    config[toInt(endVertex(edge))]);
          }
        }
      }
    }
  }
}

static cube::Configuration
findSimilarConfiguration(const ConfigsGenerator &generator,
                         cube::Configuration srcConfig) {
  auto permutation = generator.findSimilarConfiguration(srcConfig);
  return utils::applyPermutation(permutation, srcConfig);
}

SCENARIO("Find similar configuration") {
  const auto generator = ConfigsGenerator{BaseConfigs()};
  const auto &baseConfigs = generator.baseConfigs;
  WHEN("I search base configurations") {
    THEN("It must find them") {
      std::for_each(baseConfigs.configs.cbegin(), baseConfigs.configs.cend(),
                    [&generator](auto config) {
                      auto similarConfig =
                          findSimilarConfiguration(generator, config);
                      REQUIRE(similarConfig == config);
                    });
    }
  }
  WHEN("I search non base configurations with one true value") {
    auto baseConfig1 =
        baseConfigs.configs[baseConfigs.startIndicesByTrueValueCount[1]];
    THEN("It must find them") {
      for (size_t i = 0; i < cube::VERTEX_COUNT; ++i) {
        uint8_t bitset = static_cast<uint8_t>(1 << i);
        auto similarConfig =
            findSimilarConfiguration(generator, cube::Configuration(bitset));
        REQUIRE(similarConfig == baseConfig1);
        auto similarFlippedConfig = findSimilarConfiguration(
            generator, cube::Configuration(bitset).flip());
        REQUIRE(similarFlippedConfig.flip() == baseConfig1);
      }
    }
  }
  WHEN("I search non base configurations with two true values") {
    auto indexStart = baseConfigs.startIndicesByTrueValueCount[2];
    auto baseConfig2_1 = baseConfigs.configs[indexStart];
    auto baseConfig2_2 = baseConfigs.configs[indexStart + 1];
    auto baseConfig2_3 = baseConfigs.configs[indexStart + 2];
    THEN("It must find them") {
      for (size_t i = 0; i < cube::VERTEX_COUNT; ++i) {
        for (size_t j = i + 1; j < cube::VERTEX_COUNT; ++j) {
          cube::Configuration config(static_cast<uint8_t>(1 << i | 1 << j));
          auto similarConfig = findSimilarConfiguration(generator, config);
          if (similarConfig != baseConfig2_1 &&
              similarConfig != baseConfig2_2 &&
              similarConfig != baseConfig2_3) {
            REQUIRE(false);
          }
          auto similarFlippedConfig =
              findSimilarConfiguration(generator, config.flip());
          REQUIRE(similarFlippedConfig.flip() == similarConfig);
        }
      }
    }
  }
}

SCENARIO("Generate configs") {
  const auto generator = ConfigsGenerator{BaseConfigs()};
  AllConfigs configs = generator.generateConfigs();
  const auto &baseConfigs = generator.baseConfigs;
  WHEN("I retrieve the index and permutation for a base configuration") {
    THEN("It must return the index of the configuration and identity") {
      for (size_t i = 0; i < baseConfigs.configs.size(); ++i) {
        auto config = baseConfigs.configs.at(i);
        std::size_t configIndex = config.toUint8();
        REQUIRE(configs.triangles.at(configIndex) ==
                baseConfigs.triangles.at(i));
      }
    }
  }
  WHEN("retrieve the index and permutation for a configuration with one true "
       "value") {
    auto baseConfig1Index = baseConfigs.startIndicesByTrueValueCount[1];
    auto baseConfig1Triangles = baseConfigs.triangles.at(baseConfig1Index);
    THEN("It must return the index of the base configuration") {
      for (size_t i = 0; i < cube::VERTEX_COUNT; ++i) {
        cube::Configuration config(static_cast<uint8_t>(1 << i));
        REQUIRE(configs.triangles.at(config.toUint8()).size() ==
                baseConfig1Triangles.size());
        REQUIRE(configs.triangles.at(config.flip().toUint8()).size() ==
                baseConfig1Triangles.size());
      }
    }
  }
}

} // namespace marchingcubes::tests
