/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "marching-cubes/ConfigsGenerator.hpp"

#include <list>

namespace marchingcubes {

// https://en.wikipedia.org/wiki/Marching_cubes
auto retrieveBaseConfigurations() {
  using namespace cube;
  return std::array<cube::Configuration, 15>{
      {// 0 element: from 0 until 1
       cube::Configuration{0b00000000},
       // 1 element: from 1 until 2
       configurationFromPositiveVertices(0),
       // 2 elements: from 2 until 5
       configurationFromPositiveVertices(0, 1),
       configurationFromPositiveVertices(0, 5),
       configurationFromPositiveVertices(0, 7),
       // 3 elements: from 5 until 8
       configurationFromPositiveVertices(1, 2, 3),
       configurationFromPositiveVertices(0, 1, 7),
       configurationFromPositiveVertices(1, 4, 7),
       // 4 elements: from 8 until 15
       configurationFromPositiveVertices(0, 1, 2, 3),
       configurationFromPositiveVertices(1, 2, 3, 4),
       configurationFromPositiveVertices(0, 3, 5, 6),
       configurationFromPositiveVertices(0, 2, 3, 6),
       configurationFromPositiveVertices(1, 2, 3, 6),
       configurationFromPositiveVertices(0, 3, 4, 7),
       configurationFromPositiveVertices(0, 2, 3, 7)}};
}

/**
 * @brief Retrieve the intersecting triangles for each marching cube base
 *        configuration.
 *
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
std::array<TrianglesOnCubeEdges, 15> retrieveBaseConfigurationTriangles() {
  using namespace cube;
  constexpr auto t = [](Edge e0, Edge e1, Edge e2) {
    return TriangleOnCubeEdges{{e0, e1, e2}};
  };
  using ts = TrianglesOnCubeEdges;

  return std::array<TrianglesOnCubeEdges, 15>{{
      ts{},                                                       // None
      ts{{t(e01, e02, e04)}},                                     // 0
      ts{{t(e02, e04, e15), t(e02, e13, e15)}},                   // 0, 1
      ts{{t(e01, e02, e04), t(e45, e57, e15)}},                   // 0, 5
      ts{{t(e01, e02, e04), t(e37, e57, e67)}},                   // 0, 7
      ts{{t(e01, e02, e15), t(e02, e15, e26), t(e15, e37, e26)}}, // 1, 2 ,3
      ts{{t(e02, e04, e13), t(e04, e13, e15), t(e37, e57, e67)}}, // 0, 1, 7
      ts{{t(e01, e13, e15), t(e04, e45, e46), t(e37, e57, e67)}}, // 1, 4, 7
      ts{{t(e04, e15, e26), t(e15, e37, e26)}},                   // 0, 1, 2, 3
      ts{{t(e04, e45, e46), t(e01, e02, e15), t(e02, e15, e37),   //
          t(e15, e26, e37)}},                                     // 1, 2, 3, 4
      ts{{t(e01, e02, e04), t(e13, e23, e37), t(e15, e45, e57),   //
          t(e26, e46, e67)}},                                     // 0, 3, 5, 6
      ts{{t(e04, e46, e67), t(e01, e04, e67), t(e01, e37, e67),   //
          t(e01, e13, e37)}},                                     // 0, 2, 3, 6
      ts{{t(e01, e02, e46), t(e01, e37, e46), t(e37, e46, e67),   //
          t(e01, e15, e37)}},                                     // 1, 2, 3, 6
      ts{{t(e01, e02, e46), t(e01, e45, e46), t(e13, e23, e67),   //
          t(e13, e57, e67)}},                                     // 0, 3, 4, 7
      ts{{t(e01, e04, e26), t(e01, e26, e57), t(e26, e57, e67),   //
          t(e01, e13, e57)}}                                      // 0, 2, 3, 7
  }};
}

BaseConfigs::BaseConfigs()
    : configs{retrieveBaseConfigurations()},
      triangles{retrieveBaseConfigurationTriangles()},
      startIndicesByTrueValueCount{{0, 1, 2, 5, 8, 15}} {}

//// Test all configurations
//
//           D2                  D2
//      A6________A7         a6________a7
//   D7 /|       /|D6     d7 /|       /|d6
//     / |  D3  / |         / |  d3  / |
//   A4________A5 |D10    a4________a5 |d10
//    |  |     |  |        |  |     |  |
//    |  |D11  |D9|        |  |d11  |d9|
// D8 |  |  D1 |  |     d8 |  |  d1 |  |
//    | A2_____|__A3       | a2_____|__a3
//    | /      | /         | /      | /
//    |/ D4    |/ D5       |/ d4    |/ d5
//   A0________A1         a0________a1
//        D0                   d0
//
// A configuration a0,a1,...,a7 is a permutation P of A0,A1,...,A7
// checking the condition:
//    for each i in 0..7, there is one and only one j in 0..7
//          so that "a_i=P(A_i)=A_j"
//    for each i in 0..11, there is one and only one j in 0..11
//          so that "d_i=Pd(D_i)=D_j"
//
// As a consequence, if A_i1, A_i2, A_i3, A_i4 are on the same cube face
// then a_j1=P(A_i1), a_j2=P(A_i2), a_j3=P(A_i3), a_j4=P(A_i4) are on the
// same cube face The points a0, a1, a3, a4 are defining a configuration:
//   - a2 is the unique point on the same face as a0, a1, a3
//   - a5 is the unique point on the same face as a0, a1, a4
//   - a7 is the unique point on the same face as a0, a3, a4
//   - a6 is the unique point on the same face as a4, a5, a7
//
// Testing all configurations mean testing all permutations
// giving different a0, a1, a2, a4
// (remembering that d0, d4, d5, d8 are edges from D0,...,D11 )
// Only three edges are starting from a given a0 = A_i0:
// D_j1, D_j2 and D_j3. The other extremities of these edges
// are a1, a3 and a4, but we do not know if a1 is at the end
// of edge D_j1, D_j2 or D_j3.
// => there are 6=3*2*1 permutations without repeting a number
// possible for a given a0
// a0 can be one of the eight vertex of the cube
// => 6*8 = 48 possibilities
//
// Algorithm:
// ----------
// For a0 in A0..A7
//    For a1, a2, a4 extremities of edges starting with a0
//       generate permutation
//

// Helper structs to apply symmetries
template <std::uint8_t N> struct ApplySymmetries {
  std::array<cube::Permutation, N> symmetries;

  template <typename TFun> void apply(cube::Permutation initial, TFun fun) {
    fun(initial);
    std::for_each(symmetries.cbegin(), symmetries.cend(), [&](auto symmetry) {
      fun(utils::applyPermutation(initial, symmetry));
    });
  }
};

struct ApplySymmetry : public ApplySymmetries<1> {
  ApplySymmetry(cube::Permutation symmetry)
      : ApplySymmetries<1>{{{symmetry}}} {}
};

auto createAllowedPermutations() {
  using namespace cube;

  std::array<Permutation, 48> permutations;
  std::size_t currentPermutation = 0;

  auto identity = Permutation::identity();

  /**
   * Vertex indices of a cube
   *
   *      6_____7        z
   *     /|    /|        |  y
   *    4_____5 |        | /
   *    | |   | |        |/
   *    | |   | |        #----- x
   *    | |   | |
   *    | 2___|_3
   *    |/    |/
   *    0_____1
   */
  // (1, 3, 4, 6)  -->  base: (0, 3, 4, 7)?
  ApplySymmetry symmetryXY(Permutation{{4, 5, 6, 7, 0, 1, 2, 3}});
  ApplySymmetry symmetryXZ(Permutation{{2, 3, 0, 1, 6, 7, 4, 5}});
  ApplySymmetry symmetryYZ(Permutation{{1, 0, 3, 2, 5, 4, 7, 6}});

  Permutation symmetry0347{{0, 2, 1, 3, 4, 6, 5, 7}};
  Permutation symmetry0257{{0, 4, 2, 6, 1, 5, 3, 7}};
  ApplySymmetries<2> symmetriesForA1{{{symmetry0347, symmetry0257}}};

  ApplySymmetry symmetry0167(Permutation{{0, 1, 4, 5, 2, 3, 6, 7}});

  // Change a0 origin
  symmetryXY.apply(identity, [&](Permutation xySymmetry) {
    symmetryXZ.apply(xySymmetry, [&](Permutation xyXzSymmetry) {
      symmetryYZ.apply(xyXzSymmetry, [&](Permutation xyXzYzSymmetry) {
        // Change a1 origin
        symmetriesForA1.apply(
            xyXzYzSymmetry, [&](Permutation permutationForA1) {
              // Interchange a2 and a4
              symmetry0167.apply(
                  permutationForA1, [&](Permutation permutationForA2) {
                    permutations.at(currentPermutation) = permutationForA2;
                    ++currentPermutation;
                  });
            });
      });
    });
  });
  assert(currentPermutation == 48);
  assert(std::find(permutations.cbegin(), permutations.cend(),
                   Permutation::invalid()) == permutations.cend());
  return permutations;
}

ConfigsGenerator::ConfigsGenerator(const BaseConfigs baseConfigs)
    : baseConfigs{baseConfigs}, allowedPermutations{
                                    createAllowedPermutations()} {}

cube::Permutation
ConfigsGenerator::findSimilarConfiguration(cube::Configuration config) const {
  auto trueValueCount = config.trueValueCount();
  if (trueValueCount > 4) {
    return findSimilarConfiguration(config.flip());
  }
  auto fromIndex = baseConfigs.startIndicesByTrueValueCount[trueValueCount];
  auto untilIndex =
      baseConfigs.startIndicesByTrueValueCount[trueValueCount + 1];
  for (auto permutation = allowedPermutations.cbegin();
       permutation != allowedPermutations.cend(); ++permutation) {
    auto permutedConfig = utils::applyPermutation(*permutation, config);
    for (auto i = fromIndex; i < untilIndex; ++i) {
      if (permutedConfig == baseConfigs.configs[i]) {
        return *permutation;
      }
    }
  }
  throw std::runtime_error("Configuration not found: " + config.to_string());
}

AllConfigs ConfigsGenerator::generateConfigs() const {
  std::bitset<CONFIGS_COUNT> triangleSet{0};
  std::array<TrianglesOnCubeEdges, CONFIGS_COUNT> triangles;

  auto setConfig = [&](std::uint8_t configIndex,
                       TrianglesOnCubeEdges baseTriangles,
                       cube::Permutation permutation) {
    assert(!triangleSet[configIndex]);
    triangleSet[configIndex] = true;
    std::list<TriangleOnCubeEdges> permutedTriangles;
    for (size_t iTriangle = 0; iTriangle < baseTriangles.size(); ++iTriangle) {
      auto baseEdges = baseTriangles[iTriangle];
      triangle::Type<cube::Edge> permutedEdges;
      for (size_t iEdge = 0; iEdge < baseEdges.size(); ++iEdge) {
        permutedEdges.at(iEdge) =
            toPermutatedCube(baseEdges[iEdge], permutation);
      }
      permutedTriangles.push_back(TriangleOnCubeEdges{permutedEdges});
    }
    triangles.at(configIndex) = TrianglesOnCubeEdges{permutedTriangles};
  };

  const auto identity = cube::Permutation::identity();
  for (uint8_t i = 0; i < baseConfigs.configs.size(); ++i) {
    auto config = baseConfigs.configs.at(i);
    auto configIndex = config.toUint8();
    setConfig(configIndex, baseConfigs.triangles.at(i), identity);
    setConfig(config.flip().toUint8(), baseConfigs.triangles.at(i), identity);
  }

  assert(CONFIGS_COUNT == std::numeric_limits<std::uint8_t>::max() + 1);
  for (std::uint32_t srcConfigIndex = 0; srcConfigIndex < CONFIGS_COUNT;
       ++srcConfigIndex) {
    if (triangleSet[srcConfigIndex]) {
      continue;
    }
    auto configIndex = static_cast<std::uint8_t>(srcConfigIndex);
    cube::Configuration config(configIndex);
    if (config.trueValueCount() > 4) {
      continue;
    }
    auto permutation = findSimilarConfiguration(config);
    auto baseConfig = utils::applyPermutation(permutation, config);
    assert(baseConfig.trueValueCount() <= 4);
    auto baseConfigTriangles = triangles.at(baseConfig.toUint8());
    setConfig(configIndex, baseConfigTriangles, permutation);
    setConfig(config.flip().toUint8(), baseConfigTriangles, permutation);
  }
  assert(triangleSet.all());
  return AllConfigs{triangles};
}

} // namespace marchingcubes
