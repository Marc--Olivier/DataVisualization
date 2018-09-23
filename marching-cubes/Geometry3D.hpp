/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include <array>

/*!
 * \file Geometry3D.hpp
 * \brief The file Geometry3D.hpp contains a couple of base 3D elements such as
 * dimensions and Point3D.
 */
namespace marchingcubes {

constexpr std::size_t X = 0;
constexpr std::size_t Y = X + 1;
constexpr std::size_t Z = Y + 1;
constexpr std::size_t DIM_COUNT = Z + 1;

using Point3D = std::array<double, DIM_COUNT>;

} // namespace marchingcubes
