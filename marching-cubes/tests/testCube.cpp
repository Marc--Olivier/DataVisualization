/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "marching-cubes/Cube.hpp"

#include <catch2/catch.hpp>

namespace marchingcubes::cube::tests {

SCENARIO("Vertex") {
  GIVEN("A vertex") {
    WHEN("I retrieve the name") {
      THEN("The name is returned") { REQUIRE(std::to_string(v0) == "v0"); }
    }
  }
}

SCENARIO("Edge") {
  GIVEN("An edge") {
    auto startVertex = v3;
    auto endVertex = v7;
    auto edge = cube::edge(startVertex, endVertex);
    WHEN("I retrieve the size of the edge") {
      THEN("the result is 1") { REQUIRE(sizeof(edge) == 1); }
    }
    WHEN("I retrieve the start vertex") {
      auto vertex = cube::startVertex(edge);
      THEN("The index of the start vertex is returned") {
        REQUIRE(vertex == startVertex);
      }
    }
    WHEN("I retrieve the end vertex") {
      auto vertex = cube::endVertex(edge);
      THEN("The index of the end vertex is returned") {
        REQUIRE(vertex == endVertex);
      }
    }
    WHEN("I print the edge") { REQUIRE(to_string(edge) == "3->7"); }
    WHEN("I retrieve the Edge after having permuted the vertices of the cube") {
      cube::Permutation permutation{{1, 2, 3, 4, 5, 6, 7, 0}};
      auto edge = e26;
      auto permuted = toPermutatedCube(edge, permutation);
      THEN("The start and end vertices are transformed") {
        REQUIRE(permuted == e37);
      }
    }
  }
}

SCENARIO("Cube3D") {
  GIVEN("A cube defined by its min and max values per axis") {
    Cube3D cube{{{std::make_pair(-1.0, 3.0), std::make_pair(-2.0, 6.0),
                  std::make_pair(-3.0, 9.0)}}};
    /*
    WHEN("I access one of the point") {
      THEN("The correct 3D coordinates are returned") {
        REQUIRE(cube.point(0) == Point3D{{-1.0, -2.0, -3.0}});
        REQUIRE(cube.point(1) == Point3D{{3.0, -2.0, -3.0}});
        REQUIRE(cube.point(2) == Point3D{{-1.0, 6.0, -3.0}});
        REQUIRE(cube.point(3) == Point3D{{3.0, 6.0, -3.0}});
        REQUIRE(cube.point(4) == Point3D{{-1.0, -2.0, 9.0}});
        REQUIRE(cube.point(5) == Point3D{{3.0, -2.0, 9.0}});
        REQUIRE(cube.point(6) == Point3D{{-1.0, 6.0, 9.0}});
        REQUIRE(cube.point(7) == Point3D{{3.0, 6.0, 9.0}});
      }
    }
    */
    WHEN("I interpolate a point between two vertices on the same edge") {
      THEN("The 3D coordinates of the point are returned") {
        REQUIRE(cube.interpolatedPoint(e01, 0.75) ==
                Point3D{{2.0, -2.0, -3.0}});
        REQUIRE(cube.interpolatedPoint(e13, 0.75) == Point3D{{3.0, 4.0, -3.0}});
        REQUIRE(cube.interpolatedPoint(e04, 0.75) ==
                Point3D{{-1.0, -2.0, 6.0}});
      }
    }
  }
}

SCENARIO("Configuration") {
  GIVEN("A configuration") {
    uint8_t bitset = 0b00111001;
    const Configuration config{bitset};
    WHEN("I ask for its size") {
      THEN("The result is 8") { REQUIRE(sizeof(config) == 8); }
    }
    WHEN("I ask for its index") {
      auto index = config.toUint8();
      THEN("The configuration index is returned") { REQUIRE(index == bitset); }
    }
    WHEN("I count the number of true values") {
      Configuration config3{0b01001001};
      Configuration config2{0b00010100};
      Configuration config1{0b00000010};
      Configuration config0{0b00000000};
      REQUIRE(config.trueValueCount() == 4);
      REQUIRE(config3.trueValueCount() == 3);
      REQUIRE(config2.trueValueCount() == 2);
      REQUIRE(config1.trueValueCount() == 1);
      REQUIRE(config0.trueValueCount() == 0);
      REQUIRE(config.flip().trueValueCount() == 4);
      REQUIRE(config3.flip().trueValueCount() == 5);
      REQUIRE(config2.flip().trueValueCount() == 6);
      REQUIRE(config1.flip().trueValueCount() == 7);
      REQUIRE(config0.flip().trueValueCount() == 8);
    }
    WHEN("I retrieve the values by index") {
      THEN("It provides the correct value") {
        REQUIRE(config[0] == true);
        REQUIRE(config[1] == false);
        REQUIRE(config[2] == false);
        REQUIRE(config[3] == true);
        REQUIRE(config[4] == true);
        REQUIRE(config[5] == true);
        REQUIRE(config[6] == false);
        REQUIRE(config[7] == false);
      }
    }
    WHEN("I set the value at a given index") {
      Configuration copyConfig = config;
      copyConfig[4] = true;
      copyConfig[5] = false;
      copyConfig[7] = true;
      THEN("The value is correctly set") {
        REQUIRE(copyConfig[4] == true);
        REQUIRE(copyConfig[5] == false);
        REQUIRE(copyConfig[7] == true);
      }
    }
    WHEN("I flip the configuration") {
      Configuration flippedConfig = config.flip();
      THEN("The flipped configuration is returned") {
        REQUIRE(flippedConfig.toUint8() == 0b11000110);
      }
    }
    WHEN("I compare two configurations") {
      REQUIRE(config == Configuration(0b00111001));
      REQUIRE(config != Configuration(0b00111000));
    }
    WHEN("I convert it to a string") {
      REQUIRE(config.to_string() == "00111001");
    }
  }
  GIVEN("Positive vertices") {
    WHEN("I create a configuration from these vertices") {
      Configuration config{configurationFromPositiveVertices(1, 3, 4)};
      THEN("A configuration that contains true values at these vertices is "
           "returned") {
        REQUIRE(config == Configuration{0b00011010});
      }
    }
  }
}

SCENARIO("Apply permutation") {
  GIVEN("A permutation different from identity") {
    cube::Permutation permutation{{1, 2, 3, 4, 5, 6, 7, 0}};
    WHEN("I apply this permutation to a configuration") {
      uint8_t bitset = 0b00111001;
      const Configuration config{bitset};
      auto permuted = utils::applyPermutation(permutation, config);
      THEN("The elements of the configuration are permuted") {
        REQUIRE(permuted.toUint8() == 0b10011100);
      }
    }
  }
}

} // namespace marchingcubes::cube::tests
