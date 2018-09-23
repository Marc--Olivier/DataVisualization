#pragma once

#include "utils/TinyArray.hpp"

#include <array>
#include <cassert>

namespace utils::internal {

/*!
 * \fn recursiveIsValidPermutation
 * \brief The function recursiveIsValidPermutation checks if the values
 * contained in container represent a permutation or not.
 */
template <typename TElt, TElt M, TElt N, typename TContainer,
          std::enable_if_t<std::is_integral_v<TElt>> * = nullptr>
constexpr bool recursiveIsValidPermutation(const TContainer &container) {
  static_assert(M >= N, "M must be greater than or equal to N");
  if constexpr (M == N) {
    if (M >= container.size()) {
      return false;
    }
  } else {
    if (container[M] == container[N]) {
      return false;
    }
  }
  if constexpr (N > 0) {
    return recursiveIsValidPermutation<TElt, M, N - 1, TContainer>(container);
  } else if constexpr (M > 0) {
    static_assert(N == 0, "N must equal 0");
    static_assert(M > 0, "M must be greater than 0");
    return recursiveIsValidPermutation<TElt, M - 1, M - 1, TContainer>(
        container);
  }
  return true;
}

/*!
 * \fn isValidPermutation
 * \brief The function isValidPermutation checks if the values
 * contained in the TinyArray container represent a permutation or not.
 */
template <typename TElt, std::size_t oneElementSize, std::size_t fixedSize,
          std::enable_if_t<std::is_integral_v<TElt>> * = nullptr>
constexpr bool isValidPermutation(
    const TinyArray<TElt, oneElementSize, fixedSize> &container) {
  return recursiveIsValidPermutation<
      TElt, fixedSize - 1, fixedSize - 1,
      TinyArray<TElt, oneElementSize, fixedSize>>(container);
}

/*!
 * \fn recursiveIdentityPermutation
 * \brief The function recursiveIdentityPermutation recursively creates
 * the identity permutation.
 *
 * Consider to use the function identityPermutation that has a simpler
 * interface instead.
 */
template <std::size_t oneElementSize, std::size_t count, std::size_t acc>
constexpr std::size_t recursiveIdentityPermutation() {
  if constexpr (count < 2) {
    return acc;
  } else {
    return recursiveIdentityPermutation<oneElementSize, count - 1,
                                        (acc | count - 1) << oneElementSize>();
  }
}

/*!
 * \fn identityPermutation
 * \brief The function identityPermutation creates the identity permutation.
 */
template <std::size_t count> constexpr std::size_t identityPermutation() {
  return recursiveIdentityPermutation<ceilLog2(count), count, 0>();
}

/*!
 * \fn recursivelyInvertPermutation
 * \brief The function recursivelyInvertPermutation recursively calculates the
 * inverted permutation of the given src container.
 */
template <std::size_t oneElementSize, std::size_t count, typename TContainer>
constexpr std::size_t recursivelyInvertPermutation(const TContainer &src,
                                                   std::size_t acc) {
  if constexpr (count == 0) {
    return acc;
  } else {
    constexpr auto prevElt = count - 1;
    return recursivelyInvertPermutation<oneElementSize, prevElt>(
        src, acc | (prevElt << (src[prevElt] * oneElementSize)));
  }
}

/*!
 * \fn invertPermutation
 * \brief The function invertPermutation calculates the
 * inverted permutation of the given TinyArray container.
 */
template <typename TElt, std::size_t oneElementSize, std::size_t fixedSize,
          std::enable_if_t<std::is_integral_v<TElt>> * = nullptr>
constexpr std::size_t
invertPermutation(const TinyArray<TElt, oneElementSize, fixedSize> &container) {
  return recursivelyInvertPermutation<oneElementSize, fixedSize>(container, 0);
}

} // namespace utils::internal
