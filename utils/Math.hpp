/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include <cassert>
#include <type_traits>

namespace utils {
namespace internal {

template <typename T, std::enable_if_t<std::is_integral_v<T>> * = nullptr>
constexpr T ceilLog2WithAcc(T val, T acc, T exponent) {
  return acc >= val ? exponent : ceilLog2WithAcc(val, acc * 2, exponent + 1);
}

} // namespace internal

/*!
 * \fn pow
 * \brief Calculates the power of an integer at compile time. For example,
 * pow(2, 3) returns 8.
 */
template <typename T, std::enable_if_t<std::is_integral_v<T>> * = nullptr>
constexpr T pow(T base, size_t exponent) {
  return exponent == 0 ? 1 : base * pow(base, exponent - 1);
}

/*!
 * \fn ceilLog2
 * \brief Calculates the logarithm to the base 2 of an integer at compile time,
 * and rounds the result to the upper integer. For example, ceilLog2(2) returns
 * 1, ceilLog2(3) returns 2, ceilLog2(4) returns 2, ceilLog2(5) returns 3,
 * ceilLog2(8) returns 3.
 */
template <typename T, std::enable_if_t<std::is_integral_v<T>> * = nullptr>
constexpr T ceilLog2(T val) {
  assert(val >= 2);
  return internal::ceilLog2WithAcc(val, static_cast<T>(1), static_cast<T>(0));
}

/*!
 * \fn fillWithOnes
 * \brief Creates a size_t with the `count` first bits equal to 1 at compile
 * time. For example, fillWithOnes(3) returns 0b111.
 */
constexpr size_t fillWithOnes(size_t count) {
  return count == 0 ? 0 : fillWithOnes(count - 1) << 1 | 1;
}

} // namespace utils
