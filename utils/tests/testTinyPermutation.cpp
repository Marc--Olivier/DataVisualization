/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "utils/TinyPermutation.hpp"

#include <array>
#include <sstream>

#include <catch2/catch.hpp>

namespace Catch {

template <typename TElt, std::size_t oneElementSize, std::size_t size>
struct StringMaker<utils::TinyArray<TElt, oneElementSize, size>> {
  static std::string
  convert(utils::TinyArray<TElt, oneElementSize, size> const &container) {
    std::stringstream acc;
    acc << std::hex << "0x" << static_cast<size_t>(container) << std::dec;
    if (container.size() == 0) {
      return "";
    }
    acc << ": " << std::to_string(container[0]);
    for (size_t i = 1; i < container.size(); ++i) {
      acc << " " << std::to_string(container[i]);
    }
    return acc.str();
  }
};

} // namespace Catch

namespace utils::tests {

using CubePermutationTinyContainer =
    internal::TinyContainer<uint8_t, 3, true, 8>;
using CubePermutationTinyArray = TinyArray<uint8_t, 3, 8>;
using CubePermutation = Permutation<uint8_t, 8>;

TEST_CASE("internal TinyPermutation") {

  SECTION("identity permutation") {
    REQUIRE(CubePermutationTinyArray(internal::identityPermutation<8>()) ==
            CubePermutationTinyArray{{0, 1, 2, 3, 4, 5, 6, 7}});
  }

  SECTION("invert permutation") {
    CubePermutation permutation{{1, 2, 3, 4, 5, 6, 7, 0}};
    REQUIRE(
        CubePermutationTinyArray(internal::invertPermutation(permutation)) ==
        CubePermutationTinyArray{{7, 0, 1, 2, 3, 4, 5, 6}});
  }
}

SCENARIO("Permutation") {
  GIVEN("The identify permutation") {
    auto permutation = CubePermutation::identity();
    WHEN("I retrieve the size of a permutation") {
      THEN("the result is 8") { REQUIRE(sizeof(permutation) == 8); }
    }
    WHEN("I print the permutation") {
      REQUIRE(permutation.to_string() == "0, 1, 2, 3, 4, 5, 6, 7");
    }
    WHEN("I retrieve the target vertex for each index") {
      THEN("Each target vertex is equal to its index") {
        for (std::uint8_t i = 0; i < permutation.size(); ++i) {
          REQUIRE(permutation[i] == i);
        }
      }
    }
  }
  GIVEN("A permutation different from identity") {
    CubePermutation permutation{{1, 2, 3, 4, 5, 6, 7, 0}};
    WHEN("I retrieve the target vertex for each index") {
      THEN("Each target vertex is equal to its index") {
        for (std::uint8_t i = 0; i < permutation.size() - 1; ++i) {
          REQUIRE(permutation[i] == i + 1);
        }
        REQUIRE(permutation[7] == 0);
      }
    }
    WHEN("I create an inverted the permutation") {
      auto inverted = permutation.invert();
      THEN("The permutation is inverted") {
        REQUIRE(inverted[0] == 7);
        for (std::uint8_t i = 1; i < permutation.size(); ++i) {
          REQUIRE(inverted[i] == i - 1);
        }
      }
    }
  }
}

SCENARIO("Apply permutation") {
  GIVEN("A permutation different from identity") {
    CubePermutation permutation{{1, 2, 3, 4, 5, 6, 7, 0}};
    WHEN("I apply this permutation to an array") {
      std::array<std::size_t, CubePermutation::size()> arr = {
          {10, 11, 12, 13, 14, 15, 16, 17}};
      auto permuted = applyPermutation(permutation, arr);
      // std::vector<std::size_t> vec{{10, 11, 12, 13, 14, 15, 16, 17}};
      // applyPermutation(permutation, vec);
      THEN("The elements of the array are permuted") {
        REQUIRE(permuted == std::array<std::size_t, CubePermutation::size()>{
                                {11, 12, 13, 14, 15, 16, 17, 10}});
      }
    }
    WHEN("I apply this permutation to another permutation") {
      const CubePermutation secondPermutation{{1, 0, 3, 2, 5, 4, 7, 6}};
      auto permuted = applyPermutation(permutation, secondPermutation);
      THEN("The elements of the configuration are permuted") {
        REQUIRE(permuted == CubePermutation{{0, 3, 2, 5, 4, 7, 6, 1}});
      }
    }
  }
}

} // namespace utils::tests
