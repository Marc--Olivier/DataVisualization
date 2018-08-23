/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "marching-cubes/MarchingCubes.hpp"

#include "marching-cubes/Cube.hpp"
#include "marching-cubes/Tensor3D.hpp"
#include "marching-cubes/tests/expectedIsoSurfaces.hpp"
#include "third-parties/catch-main/CatchApprox.hpp"

#include <iostream>
#include <sstream>

namespace marchingcubes::tests {

static MarchingCubes algo{};

namespace internal {

static Point3D P(double v1, double v2, double v3) {
  return Point3D{{v1, v2, v3}};
}

static Triangle3D T(const Point3D &p1, const Point3D &p2, const Point3D &p3) {
  return Triangle3D{{p1, p2, p3}};
}

static Tensor3D unitCubeTensor(std::initializer_list<size_t> indices) {
  std::vector<double> values(cube::VERTEX_COUNT, 0.0);
  const auto unitValue = 1.0;
  for (auto index : indices) {
    values.at(index) = unitValue;
  }
  return Tensor3D{2, 2, 2, values};
}

template <typename TContainer>
static std::string join(const TContainer &container,
                        const std::string &delimiter) {
  auto it = container.begin();
  const auto last = container.end();
  if (it == last) {
    return std::string();
  }
  std::stringstream acc;
  acc << *(it++);
  while (it != last) {
    acc << delimiter << *(it++);
  }
  return acc.str();
}

} // namespace internal

TEST_CASE("isoSurface for basic configurations on one cube") {

  using namespace internal;

  /*
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
  */

  Grid3D cube{equidistantPoints(-1.0, 1.0, 2), equidistantPoints(-1.0, 1.0, 2),
              equidistantPoints(-1.0, 1.0, 2)};
  REQUIRE(cube.values[X][0] == Approx(-1.0));

  auto test = [&](std::initializer_list<size_t> indicesOfPositiveValues,
                  double isoValue, const std::vector<Triangle3D> &expected) {
    auto msg = "Positive values: " + join(indicesOfPositiveValues, ", ") +
               ". Iso value: " + std::to_string(isoValue);
    SECTION(msg) {
      auto valuesOnCube = unitCubeTensor(indicesOfPositiveValues);
      const auto intersectingTriangles =
          algo.isoSurface(cube, valuesOnCube, isoValue);
      INFO("intersectingTriangles: " +
           Catch::makeString(intersectingTriangles));
      INFO("expected: " + Catch::makeString(expected));
      if (intersectingTriangles.size() != expected.size()) {
        // CHECK(intersectingTriangles == Catch::approx(expected));
        REQUIRE(intersectingTriangles.size() == expected.size());
      }
      for (size_t i = 0; i < intersectingTriangles.size(); ++i) {
        INFO("error at triangle #" + std::to_string(i));
        REQUIRE(intersectingTriangles[i] == Catch::approx(expected[i]));
      }
    }
  };

  auto expected = [](std::initializer_list<Triangle3D> triangles) {
    return std::vector<Triangle3D>{triangles};
  };

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
  SECTION("One positive value on the cube") {
    test({0}, 0.25,
         expected(
             {T(P(0.5, -1.0, -1.0), P(-1.0, 0.5, -1.0), P(-1.0, -1.0, 0.5))}));

    test({0}, 1.0,
         expected({T(P(-1.0, -1.0, -1.0), P(-1.0, -1.0, -1.0),
                     P(-1.0, -1.0, -1.0))}));
  }

  SECTION("Two positive values on the cube") {
    test({0, 1}, 0.25,
         expected(
             {T(P(-1.0, 0.5, -1.0), P(-1.0, -1.0, 0.5), P(1.0, -1.0, 0.5)),
              T(P(-1.0, 0.5, -1.0), P(1.0, 0.5, -1.0), P(1.0, -1.0, 0.5))}));

    test({0, 5}, 0.25,
         expected(
             {T(P(0.5, -1.0, -1.0), P(-1.0, 0.5, -1.0), P(-1.0, -1.0, 0.5)),
              T(P(-0.5, -1.0, 1.0), P(1.0, 0.5, 1.0), P(1.0, -1.0, -0.5))}));

    test(
        {0, 7}, 0.25,
        expected({T(P(0.5, -1.0, -1.0), P(-1.0, 0.5, -1.0), P(-1.0, -1.0, 0.5)),
                  T(P(1.0, 1.0, -0.5), P(1.0, -0.5, 1.0), P(-0.5, 1.0, 1.0))}));
  }
}

SCENARIO("isoSurface for one cube") {
  GIVEN("A cube and values on this cube") {
    Grid3D cube{equidistantPoints(-1.0, 3.0, 2),
                equidistantPoints(-2.0, 6.0, 2),
                equidistantPoints(-3.0, 9.0, 2)};

    Tensor3D valuesOnCube{
        2, 2, 2, std::vector<double>{{1.0, 2.0, 4.0, 3.0, 5.0, 6.0, 8.0, 7.0}}};

    WHEN("I calculate the intersecting triangles for a value below the min") {
      const std::vector<Triangle3D> intersectingTriangles =
          algo.isoSurface(cube, valuesOnCube, 0.0);
      THEN("No triangle is returned") {
        REQUIRE(intersectingTriangles == std::vector<Triangle3D>{});
      }
    }

    WHEN("I calculate the intersecting triangles for a value between the min "
         "and the second min value") {
      const auto intersectingTriangles =
          algo.isoSurface(cube, valuesOnCube, 1.75);
      THEN("One triangle is returned") {
        std::vector<Triangle3D> expected{
            {{Point3D{{2.0, -2.0, -3.0}}, Point3D{{-1.0, 0.0, -3.0}},
              Point3D{{-1.0, -2.0, -0.75}}}}};
        REQUIRE(intersectingTriangles == expected);
      }
    }

    WHEN("I calculate the intersecting triangles for a value between the max "
         "and the second max value") {
      const auto intersectingTriangles =
          algo.isoSurface(cube, valuesOnCube, 7.25);
      THEN("One triangle is returned") {
        std::vector<Triangle3D> expected{
            {{Point3D{{-1.0, 6.0, 6.75}}, Point3D{{-1.0, 4.0, 9.0}},
              Point3D{{2.0, 6.0, 9.0}}}}};
        REQUIRE(intersectingTriangles == expected);
      }
    }
  }
}

SCENARIO("isoSurface sphere" /*, "[!hide]" */) {
  GIVEN("A sphere tensor 3D") {
    Grid3D grid{equidistantPoints(-1.0, 1.0, 5),
                equidistantPoints(-2.0, 2.0, 5),
                equidistantPoints(-3.0, 3.0, 5)};
    auto sphere = createSphere(grid);
    WHEN("I calculate an iso-surface") {
      auto isoSurface = algo.isoSurface(grid, sphere, 4.0);
      THEN("Triangles representing the iso-surface are returned") {
        auto expected = expected::sphereIsoSurface4();
        INFO("Number of triangles: " + std::to_string(isoSurface.size()));
        for (size_t i = 0; i < isoSurface.size(); ++i) {
          INFO("Triangle #" + std::to_string(i));
          REQUIRE(isoSurface.at(i) == Catch::approx(expected.at(i)));
        }
      }
    }
  }
}

} // namespace marchingcubes::tests
