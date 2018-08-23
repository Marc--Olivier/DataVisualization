/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "utils/internal/TinyContainer.hpp"

namespace utils {

/*!
 * \class TinyArray
 * \brief Array that stores all the elements on one single std::size_t.
 *
 * `TElt` is the type of the elements stored within the container.
 *
 * `oneElementSize` is the size of one element. It must be less than
 * `sizeof(TElt)`.
 *
 * `fixedSize` is the number of elements that the container stores.
 * It must satisfy the following condition:
 * oneElementSize*fixedSize <= sizeof(size_t)
 */
template <typename TElt, std::size_t oneElementSize, std::size_t fixedSize>
class TinyArray
    : public internal::TinyContainer<TElt, oneElementSize, true, fixedSize> {

  using Parent = internal::TinyContainer<TElt, oneElementSize, true, fixedSize>;
  using ThisType = TinyArray<TElt, oneElementSize, fixedSize>;
  using value_type = typename Parent::value_type;

public:
  explicit constexpr TinyArray() noexcept : Parent{} {}
  explicit constexpr TinyArray(size_t packedValues) noexcept
      : Parent(packedValues) {}
  explicit constexpr TinyArray(const ThisType &rhs) noexcept : Parent(rhs) {}

  template <typename TContainer,
            std::enable_if_t<internal::has_iterator_v<TContainer>> * = nullptr>
  explicit TinyArray(const TContainer &elements) : Parent(elements) {}

  explicit TinyArray(const std::initializer_list<value_type> &elements)
      : Parent(elements) {}

  static constexpr size_t size() { return fixedSize; }
};

} // namespace utils
