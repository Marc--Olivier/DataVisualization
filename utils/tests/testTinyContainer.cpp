/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "utils/internal/TinyContainer.hpp"

#include <catch2/catch.hpp>

namespace utils::internal::tests {

TEST_CASE("Tiny container of uint8_t") {
  SECTION("Empty container with size of size equals to 3") {
    REQUIRE(sizeTSize == 64);
    constexpr size_t capacity = 8;
    constexpr TinyContainer<uint8_t, 4, false, capacity> tinyContainer;
    // REQUIRE(sizeof(tinyContainer) == 8);
    REQUIRE(tinyContainer.elementSize == 4);
    REQUIRE(tinyContainer.capacity == 8);
    REQUIRE(tinyContainer.sizeMask == 0b111);
    REQUIRE(tinyContainer.elementMask == 0b1111);
  }

  SECTION("Empty container with size of size equals to 5") {
    REQUIRE(sizeTSize == 64);
    constexpr size_t capacity = 15;
    constexpr TinyContainer<uint8_t, 4, false, capacity> tinyContainer;
    // REQUIRE(sizeof(tinyContainer) == 8);
    REQUIRE(tinyContainer.elementSize == 4);
    REQUIRE(tinyContainer.capacity == 15);
    REQUIRE(tinyContainer.sizeMask == 0b1111);
    REQUIRE(tinyContainer.elementMask == 0b1111);
  }
}

} // namespace utils::internal::tests
