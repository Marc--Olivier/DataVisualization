/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "utils/Math.hpp"

#include <string>

namespace utils::internal {

constexpr std::size_t oneByteSize = 8;
constexpr std::size_t sizeTSize = sizeof(std::size_t) * oneByteSize;

/*!
 * \class has_iterator
 * \brief Trait that indicates if a given type T has an iterator type defined.
 */
template <typename T, typename = void> struct has_iterator : std::false_type {};
template <typename T>
struct has_iterator<T, std::void_t<typename T::iterator>> : std::true_type {};
template <typename T> constexpr bool has_iterator_v = has_iterator<T>::value;

/*!
 * \class size_manager
 * \brief Class that contains either a fixed size or a variable size
 *        whose limit is TCapacity.
 *
 */
template <bool TIsFixed, size_t TCapacity> struct size_manager {
public:
  static constexpr size_t fixedSize() {
    static_assert(
        isFixed(),
        "TIsFixed must be true if size_manager is used in fixed size mode");
    return TCapacity;
  }
  static constexpr size_t sizeOfSize() {
    if constexpr (isFixed()) {
      return 0;
    } else {
      return ceilLog2(TCapacity);
    }
  }
  static constexpr bool isFixed() { return TIsFixed; }

  static constexpr size_t capacity() { return TCapacity; }
};

/*!
 * \class TinyContainer
 * \brief Base class for tiny containers that store all the elements on one
 *        single std::size_t.
 *
 * `TElt` is the type of the elements stored within the container.
 * `oneElementSize` is the size of one element. It must be less than
 * `sizeof(TElt)`.
 * `hasFixedSize` indicates if the container has a fixed size of not.
 * `TCapacity` is the number of elements that the container can store.
 */
template <typename TElt, std::size_t oneElementSize, bool hasFixedSize,
          std::size_t TCapacity>
struct TinyContainer {

  using ThisType = TinyContainer<TElt, oneElementSize, hasFixedSize, TCapacity>;

public:
  using value_type = TElt;
  using SizeManager = size_manager<hasFixedSize, TCapacity>;
  static constexpr size_t elementSize = oneElementSize;
  static constexpr size_t capacity = SizeManager::capacity();
  static constexpr size_t sizeMask = fillWithOnes(SizeManager::sizeOfSize());
  static constexpr size_t elementMask = fillWithOnes(elementSize);
  static constexpr value_type toT(size_t i) {
    return static_cast<value_type>(i);
  }

public:
  explicit constexpr TinyContainer() : sizeAndValues{0} { checkSizes(); }
  explicit constexpr TinyContainer(size_t packedSizeAndValues) noexcept
      : sizeAndValues{packedSizeAndValues} {
    checkSizes();
  }

  constexpr TinyContainer(const ThisType &rhs)
      : sizeAndValues{rhs.sizeAndValues} {}

  template <typename TContainer,
            std::enable_if_t<internal::has_iterator_v<TContainer>> * = nullptr>
  explicit TinyContainer(const TContainer &elements) : sizeAndValues{0} {
    static_assert(std::is_same_v<typename std::iterator_traits<
                                     typename TContainer::iterator>::value_type,
                                 value_type>,
                  "Types shouldn't be different");
    initializeFrom(elements.size(), elements.begin(), elements.end());
  }

  explicit TinyContainer(const std::initializer_list<value_type> &elements)
      : sizeAndValues{0} {
    initializeFrom(elements.size(), elements.begin(), elements.end());
  }

public:
  auto &operator=(const ThisType &rhs) {
    sizeAndValues = rhs.sizeAndValues;
    return *this;
  }

  constexpr bool operator==(const ThisType &rhs) const {
    return sizeAndValues == rhs.sizeAndValues;
  }

  constexpr bool operator!=(const ThisType &rhs) const {
    return sizeAndValues != rhs.sizeAndValues;
  }

  value_type operator[](size_t index) const {
    assert(index < size());
    return toT(
        (sizeAndValues >> (SizeManager::sizeOfSize() + index * elementSize)) &
        elementMask);
  }

  constexpr explicit operator size_t() const { return sizeAndValues; }

  std::string to_string() const {
    const auto &this_ = *this;
    auto str = std::to_string(static_cast<size_t>(this_[0]));
    for (size_t i = 1; i < size(); ++i) {
      str += ", " + std::to_string(static_cast<size_t>(this_[i]));
    }
    return str;
  }

public:
  static constexpr void checkSizes() {
    static_assert(oneElementSize <= sizeof(value_type) * internal::oneByteSize,
                  "Provided element size bigger than what it can be");
    static_assert((sizeTSize - SizeManager::sizeOfSize()) / elementSize >=
                      capacity,
                  "Element size is too big for the container to store "
                  "`capacity` elements");
  }

  template <typename TIt>
  void initializeFrom(size_t srcSize, TIt start, TIt end) {
    checkSizes();
    if (srcSize > capacity) {
      throw std::invalid_argument(
          "Expected a source container size less than " +
          std::to_string(capacity) + ", but got " + std::to_string(srcSize));
    }
    if constexpr (!SizeManager::isFixed()) {
      sizeAndValues = srcSize;
    }
    size_t shift = SizeManager::sizeOfSize();
    for (auto it = start; it != end; ++it) {
      assert(static_cast<size_t>(*it) <
             pow(static_cast<size_t>(2), elementSize));
      sizeAndValues |= static_cast<size_t>(*it) << shift;
      shift += elementSize;
    }
  }

  constexpr size_t size() const {
    if constexpr (SizeManager::isFixed()) {
      return SizeManager::fixedSize();
    } else {
      return sizeAndValues & sizeMask;
    }
  }

private:
  size_t sizeAndValues;
};

} // namespace utils::internal
