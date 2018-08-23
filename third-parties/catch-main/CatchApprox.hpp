#pragma once

#include <catch2/catch.hpp>

namespace Catch {

template <typename T, typename = void>
struct approx_level : std::integral_constant<int, -1> {};

template <typename T>
struct approx_level<T,
                    std::enable_if_t<std::is_constructible<double, T>::value>>
    : std::integral_constant<int, 0> {};

template <typename T>
struct approx_level<
    T, std::enable_if_t<approx_level<typename T::value_type>::value >= 0>>
    : std::integral_constant<int, approx_level<typename T::value_type>::value +
                                      1> {};

template <typename T, typename = void> struct WithApprox {
  WithApprox(const T &) {
    // Have to use a trick (std::is_same_v in this case) to avoid triggering the
    // static_assert even when the template function is never instantiated.
    static_assert(!std::is_same_v<T, T>, "Cannot create approximation");
  }
};

template <typename T>
struct WithApprox<T, std::enable_if_t<approx_level<T>::value == 0>> : Approx {

  explicit WithApprox(T value) : Approx{value} {}
};

template <typename TContainer>
struct WithApprox<TContainer,
                  std::enable_if_t<approx_level<TContainer>::value >= 1>> {

  using value_type = typename TContainer::value_type;

  explicit WithApprox(const TContainer &container) : container{container} {}

  template <typename T,
            typename = std::enable_if_t<approx_level<T>::value ==
                                        approx_level<TContainer>::value>>
  bool operator==(const T &rhs) const {
    if (rhs.size() != container.size())
      return false;
    for (size_t i = 0; i < container.size(); ++i) {
      if (!(rhs[i] == WithApprox<value_type>(container[i]))) {
        return false;
      }
    }
    return true;
  }

  const TContainer &container;
};

template <typename T> WithApprox<T> approx(const T &value) {
  return WithApprox<T>{value};
}

template <typename T1, typename T2>
bool operator==(const T1 &lhs, const WithApprox<T2> &rhs) {
  static_assert(approx_level<T1>::value == approx_level<T2>::value,
                "Cannot perform approximate comparison because the container "
                "do not have the same number of sub-containers");
  return rhs == lhs;
}

// https://github.com/catchorg/Catch2/blob/master/docs/tostring.md#catchstringmaker-specialisation
template <typename T> struct StringMaker<WithApprox<T>> {
  static std::string convert(WithApprox<T> const &approxContainer) {
    return rangeToString(approxContainer.container);
  }
};

template <> struct StringMaker<WithApprox<double>> {
  static std::string convert(WithApprox<double> const &approx) {
    return approx.toString();
  }
};

/*
template <typename T,
          typename std::enable_if_t<!is_range<T>::value> * = nullptr>
inline std::string makeString(const T &obj) {
  return StringMaker<T>(obj).convert();
}
*/

template <typename TRange,
          typename std::enable_if_t<is_range<TRange>::value> * = nullptr>
inline std::string makeString(const TRange &obj) {
  return rangeToString(obj);
}

} // namespace Catch
