/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "utils/TinyArray.hpp"
#include "utils/internal/TinyPermutation.hpp"

namespace utils {

template <typename TElt, size_t TSize>
struct Permutation : TinyArray<TElt, ceilLog2(TSize), TSize> {

  constexpr Permutation() noexcept : TinyArrayType{} {}

  Permutation(const Permutation &rhs) noexcept
      : TinyArrayType{static_cast<TinyArrayType>(rhs)} {}

  explicit constexpr Permutation(size_t packedValues) noexcept
      : TinyArrayType(checkPermutation(packedValues)) {}

  explicit Permutation(
      const std::initializer_list<std::uint8_t> &elements) noexcept
      : TinyArrayType{elements} {
    checkPermutation();
  }

  constexpr Permutation invert() const {
    return Permutation(internal::invertPermutation(*this));
  }

  static constexpr Permutation invalid() noexcept { return Permutation(); }
  static constexpr Permutation identity() noexcept {
    return Permutation(internal::identityPermutation<TSize>());
  }

private:
  static constexpr size_t elementSize = ceilLog2(TSize);
  using TinyArrayType = TinyArray<TElt, elementSize, TSize>;

  static constexpr size_t checkPermutation(size_t packedValues) {
    assert(internal::isValidPermutation(TinyArrayType(packedValues)));
    return packedValues;
  }

  constexpr void checkPermutation() const {
    assert(internal::isValidPermutation(*this));
  }
};

template <typename T, typename TBase, size_t TSize>
T applyPermutation(Permutation<TBase, TSize>, const T &);

template <typename T, typename TBase, size_t TSize>
inline std::array<T, TSize>
applyPermutation(Permutation<TBase, TSize> permutation,
                 const std::array<T, TSize> &arr) {
  std::array<T, TSize> dest;
  for (size_t i = 0; i < permutation.size(); ++i) {
    dest[i] = arr[permutation[i]];
  }
  return dest;
}

template <typename TBase, size_t TSize>
inline Permutation<TBase, TSize>
applyPermutation(Permutation<TBase, TSize> permutation,
                 Permutation<TBase, TSize> other) {
  return Permutation<TBase, TSize>{
      {other[permutation[0]], other[permutation[1]], other[permutation[2]],
       other[permutation[3]], other[permutation[4]], other[permutation[5]],
       other[permutation[6]], other[permutation[7]]}};
}

} // namespace utils
