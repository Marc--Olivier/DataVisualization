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
 * \namespace internal
 * \brief Contains auxiliary classes/functions you should not use directly.
 */
namespace internal {

/*!
 * \fn recMaxCapacity
 * \brief Auxiliary function to calculate the maximum capacity of a container
 * for a given element size.
 */
template <size_t elementSize, size_t currentCapacity>
constexpr size_t recMaxCapacity() {
  static_assert(elementSize < sizeTSize,
                "elementSize must be less than sizeof(size_t)");
  static_assert(elementSize > 0, "elementSize must be greater than 0");
  if constexpr ((sizeTSize - ceilLog2(currentCapacity)) / elementSize >=
                currentCapacity) {
    return currentCapacity;
  } else {
    return recMaxCapacity<elementSize, currentCapacity - 1>();
  }
}

} // namespace internal

/*!
 * \fn maxCapacity
 * \brief Calculates the maximum capacity of a container for a given element
 * size.
 */
template <size_t elementSize> constexpr size_t maxCapacity() {
  return internal::recMaxCapacity<elementSize, internal::sizeTSize>();
}

/*!
 * \class TinyVector
 * \brief Vector that stores all the elements on one single std::size_t.
 *
 * `TElt` is the type of the elements stored within the container.
 *
 * `oneElementSize` is the size of one element. It must be less than
 * `sizeof(TElt)`.
 *
 * `TCapacity` is the maximum number of elements that the container can store.
 * It must satisfy the following condition:
 * oneElementSize*TCapacity + ceilLog2(TCapacity) <= sizeof(size_t)
 *
 * The function `maxCapacity` takes care of calculating the maximum capacity
 * possible for a given elementSize.
 */
template <typename TElt, std::size_t oneElementSize,
          std::size_t capacity = maxCapacity<oneElementSize>()>
class TinyVector
    : public internal::TinyContainer<TElt, oneElementSize, false, capacity> {

  using Parent = internal::TinyContainer<TElt, oneElementSize, false, capacity>;
  using ThisType = TinyVector<TElt, oneElementSize, capacity>;

public:
  using value_type = typename Parent::value_type;

public:
  explicit constexpr TinyVector() : Parent{} {}
  constexpr TinyVector(const ThisType &rhs) : Parent{rhs} {}

  template <typename TContainer,
            std::enable_if_t<internal::has_iterator_v<TContainer>> * = nullptr>
  explicit TinyVector(const TContainer &elements) : Parent(elements) {}

  explicit TinyVector(const std::initializer_list<value_type> &elements)
      : Parent(elements) {}

  std::string to_string() const { return Parent::to_string(Parent::size()); }
};

} // namespace utils
