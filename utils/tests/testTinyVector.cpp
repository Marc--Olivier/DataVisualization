/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "utils/TinyArray.hpp"
#include "utils/TinyVector.hpp"

#include <catch2/catch.hpp>

namespace utils::tests {

TEST_CASE("maxCapacity") {
  REQUIRE(maxCapacity<1>() == 58);
  REQUIRE(maxCapacity<2>() == 29);
  REQUIRE(maxCapacity<3>() == 19);
  REQUIRE(maxCapacity<4>() == 15);
}

TEST_CASE("Tiny vector of uint8_t") {

  SECTION("Initialized vector") {
    TinyVector<uint8_t, 4, 10> tinyVectorFromVector{
        std::vector<uint8_t>{{0, 1, 2, 3, 4, 5, 6}}};
    TinyVector<uint8_t, 4> tinyVector{{0, 1, 2, 3, 4, 5, 6}};
    INFO("Hash = " + std::to_string(static_cast<size_t>(tinyVector)));
    REQUIRE(tinyVector.size() == 7);
    for (size_t i = 0; i < tinyVector.size(); ++i) {
      INFO("Index #" + std::to_string(i));
      REQUIRE(tinyVector[i] == i);
      REQUIRE(tinyVectorFromVector[i] == i);
    }
  }
}

TEST_CASE("Tiny vector of TinyArray") {
  using EdgesArray = TinyArray<uint8_t, 4, 2>;

  SECTION("Empty vector") {
    constexpr TinyVector<EdgesArray, 4> tinyVector;
    REQUIRE(sizeof(tinyVector) == sizeof(size_t));
    REQUIRE(tinyVector.size() == 0);
  }

  SECTION("Initialized vector") {
    TinyVector<EdgesArray, 8> tinyVector{
        {EdgesArray{{0, 1}}, EdgesArray{{0, 2}}, EdgesArray{{1, 2}}}};
    INFO("Hash = " + std::to_string(static_cast<size_t>(tinyVector)));
    REQUIRE(tinyVector.size() == 3);
    REQUIRE(tinyVector[0][0] == 0);
    REQUIRE(tinyVector[0][1] == 1);
    REQUIRE(tinyVector[1][0] == 0);
    REQUIRE(tinyVector[1][1] == 2);
    REQUIRE(tinyVector[2][0] == 1);
    REQUIRE(tinyVector[2][1] == 2);

    std::vector<EdgesArray> vector{
        {EdgesArray{{0, 1}}, EdgesArray{{0, 2}}, EdgesArray{{1, 2}}}};
    TinyVector<EdgesArray, 8> tinyVectorFromVector{vector};
    REQUIRE(tinyVectorFromVector.size() == vector.size());
    REQUIRE(tinyVectorFromVector[0][0] == 0);
    REQUIRE(tinyVectorFromVector[0][1] == 1);
    REQUIRE(tinyVectorFromVector[1][0] == 0);
    REQUIRE(tinyVectorFromVector[1][1] == 2);
    REQUIRE(tinyVectorFromVector[2][0] == 1);
    REQUIRE(tinyVectorFromVector[2][1] == 2);
  }
}

} // namespace utils::tests
