#pragma once

#include <array>

/*!
 * \namespace marchingcubes::triangle
 * \brief The namespace marchingcubes::triangle provides a simple template type
 * triangle.
 */
namespace marchingcubes::triangle {

constexpr std::size_t POINT_COUNT = 3;

template <typename T> using Type = std::array<T, POINT_COUNT>;

} // namespace marchingcubes::triangle
