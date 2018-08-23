/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "utils/TinyArray.hpp"

#include <catch2/catch.hpp>

namespace utils::tests {

TEST_CASE("Tiny array of uint8_t") {

  SECTION("Initialized vector") {
    static_assert(internal::has_iterator_v<std::vector<uint32_t>>, "Really?");
    TinyArray<uint8_t, 3, 7> tinyArrayFromVector{
        std::vector<uint8_t>{{0, 1, 2, 3, 4, 5, 6}}};
    TinyArray<uint8_t, 3, 7> tinyArray{{0, 1, 2, 3, 4, 5, 6}};

    INFO("Hash = " + std::to_string(static_cast<size_t>(tinyArray)));
    REQUIRE(tinyArray.size() == 7);
    REQUIRE(tinyArrayFromVector.size() == 7);
    for (size_t i = 0; i < tinyArray.size(); ++i) {
      INFO("Index #" + std::to_string(i));
      REQUIRE(tinyArray[i] == i);
      REQUIRE(tinyArrayFromVector[i] == i);
    }
  }
}

} // namespace utils::tests
