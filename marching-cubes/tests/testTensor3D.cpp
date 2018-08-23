/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "marching-cubes/Tensor3D.hpp"

#include <numeric>

#include <catch2/catch.hpp>

namespace marchingcubes::tests {

SCENARIO("equidistantPoints") {
  REQUIRE(equidistantPoints(0, 6, 4) ==
          std::vector<double>{{0.0, 2.0, 4.0, 6.0}});
}

static std::vector<double> iota(size_t size, double startValue) {
  std::vector<double> values(size);
  assert(values.size() == size);
  std::iota(values.begin(), values.end(), startValue);
  return values;
}

SCENARIO("Tensor3D") {
  GIVEN("A 3D tensor") {
    Tensor3D tensor{2, 3, 4, iota(2 * 3 * 4, 10.0)};
    WHEN("I access the value at a given index") {
      THEN("The value at this index is returned") {
        REQUIRE(tensor.value(0, 0, 0) == 10.0);
        REQUIRE(tensor.value(1, 2, 3) == 33.0);
        REQUIRE(tensor.value(1, 1, 2) == 25.0);
      }
    }
    WHEN("I retrieve the min and max values") {
      THEN("The min and max values are returned") {
        auto [min, max] = tensor.minMax();
        REQUIRE(min == 10.0);
        REQUIRE(max == 33.0);
      }
    }
  }
}

SCENARIO("sphere") {
  GIVEN("A 3D grid") {
    Grid3D grid{equidistantPoints(-3.0, 3.0, 7),
                equidistantPoints(-4.0, 4.0, 9),
                equidistantPoints(-5.0, 5.0, 11)};
    WHEN("I create a sphere") {
      auto sphere = createSphere(grid);
      THEN("A tensor 3D with values equal to x²+y²+z² is returned") {
        auto [min, max] = sphere.minMax();
        REQUIRE(min == 0.0);
        REQUIRE(max == 50.0);
        REQUIRE(sphere.value(0, 0, 0) == 50.0);
        REQUIRE(sphere.value(3, 4, 5) == 0.0);
        REQUIRE(sphere.value(4, 4, 5) == 1.0);
        REQUIRE(sphere.value(3, 5, 5) == 1.0);
        REQUIRE(sphere.value(3, 4, 6) == 1.0);
        REQUIRE(sphere.value(5, 4, 5) == 4.0);
        REQUIRE(sphere.value(3, 6, 5) == 4.0);
        REQUIRE(sphere.value(3, 4, 7) == 4.0);
      }
    }
  }
}

} // namespace marchingcubes::tests
