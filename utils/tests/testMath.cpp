/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "utils/Math.hpp"

#include <catch2/catch.hpp>

namespace utils::tests {

TEST_CASE("ceilLog2") {
  // REQUIRE(ceilLog2(1) == 0);
  REQUIRE(ceilLog2(2) == 1);
  REQUIRE(ceilLog2(3) == 2);
  REQUIRE(ceilLog2(4) == 2);
  REQUIRE(ceilLog2(5) == 3);
  REQUIRE(ceilLog2(8) == 3);
  REQUIRE(ceilLog2(9) == 4);
}

} // namespace utils::tests
