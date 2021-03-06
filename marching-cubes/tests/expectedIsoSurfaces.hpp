/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once

#include "marching-cubes/MarchingCubes.hpp"

namespace marchingcubes::tests::expected {

// Replacements used from results:
// 1. "\n  "   ===>  ""
// 2. ",([^ ])"   ===>   ", \1"
// 3. "{[ ]*{[ ]*([-\d.]*), ([-\d.]*), ([-\d.]*)[ ]*},\ {[ ]*([-\d.]*),
// ([-\d.]*), ([-\d.]*)[ ]*},\ {[ ]*([-\d.]*), ([-\d.]*), ([-\d.]*)[ ]*}[ ]*}"
// ==>
// "Triangle3D{{ Point3D{{ \1, \2, \3 }}, Point3D{{ \4, \5, \6 }}, Point3D{{ \7,
// \8, \9}} }}"

inline std::vector<Triangle3D> sphereIsoSurface4() {
  return std::vector<Triangle3D>{
      {Triangle3D{{Point3D{{-0.8333333333, -1.0, -1.5}},
                   Point3D{{-0.5, -1.1666666667, -1.5}},
                   Point3D{{-0.5, -1.0, -1.6111111111}}}}, // 0
       Triangle3D{{Point3D{{-0.5, -1.1666666667, -1.5}},
                   Point3D{{-0.5, -1.0, -1.6111111111}},
                   Point3D{{0.0, -1.0, -1.6666666667}}}}, // 1
       Triangle3D{{Point3D{{-0.5, -1.1666666667, -1.5}},
                   Point3D{{0.0, -1.25, -1.5}},
                   Point3D{{0.0, -1.0, -1.6666666667}}}}, // 2
       Triangle3D{{Point3D{{0.0, -1.25, -1.5}},
                   Point3D{{0.0, -1.0, -1.6666666667}},
                   Point3D{{0.5, -1.0, -1.6111111111}}}}, // 3
       Triangle3D{{Point3D{{0.0, -1.25, -1.5}},
                   Point3D{{0.5, -1.1666666667, -1.5}},
                   Point3D{{0.5, -1.0, -1.6111111111}}}}, // 4
       Triangle3D{{Point3D{{0.5, -1.0, -1.6111111111}},
                   Point3D{{0.5, -1.1666666667, -1.5}},
                   Point3D{{0.8333333333, -1.0, -1.5}}}}, // 5
       Triangle3D{{Point3D{{-0.8333333333, -1.0, -1.5}},
                   Point3D{{-1.0, -0.75, -1.5}},
                   Point3D{{-0.5, -1.0, -1.6111111111}}}}, // 6
       Triangle3D{{Point3D{{-1.0, -0.75, -1.5}},
                   Point3D{{-0.5, -1.0, -1.6111111111}},
                   Point3D{{-1.0, 0.0, -1.6666666667}}}},
       Triangle3D{{Point3D{{-0.5, -1.0, -1.6111111111}},
                   Point3D{{-0.5, 0.0, -1.8333333333}},
                   Point3D{{-1.0, 0.0, -1.6666666667}}}},
       Triangle3D{{Point3D{{-0.5, -1.0, -1.6111111111}},
                   Point3D{{0.0, -1.0, -1.6666666667}},
                   Point3D{{-0.5, 0.0, -1.8333333333}}}},
       Triangle3D{{Point3D{{0.0, -1.0, -1.6666666667}},
                   Point3D{{0.0, 0.0, -1.8888888889}},
                   Point3D{{-0.5, 0.0, -1.8333333333}}}},
       Triangle3D{{Point3D{{0.0, -1.0, -1.6666666667}},
                   Point3D{{0.5, -1.0, -1.6111111111}},
                   Point3D{{0.0, 0.0, -1.8888888889}}}},
       Triangle3D{{Point3D{{0.5, -1.0, -1.6111111111}},
                   Point3D{{0.5, 0.0, -1.8333333333}},
                   Point3D{{0.0, 0.0, -1.8888888889}}}},
       Triangle3D{{Point3D{{0.8333333333, -1.0, -1.5}},
                   Point3D{{1.0, -0.75, -1.5}},
                   Point3D{{0.5, -1.0, -1.6111111111}}}},
       Triangle3D{{Point3D{{1.0, -0.75, -1.5}},
                   Point3D{{0.5, -1.0, -1.6111111111}},
                   Point3D{{1.0, 0.0, -1.6666666667}}}},
       Triangle3D{{Point3D{{0.5, -1.0, -1.6111111111}},
                   Point3D{{0.5, 0.0, -1.8333333333}},
                   Point3D{{1.0, 0.0, -1.6666666667}}}},
       Triangle3D{{Point3D{{-1.0, 0.75, -1.5}},
                   Point3D{{-0.8333333333, 1.0, -1.5}},
                   Point3D{{-1.0, 0.0, -1.6666666667}}}},
       Triangle3D{{Point3D{{-0.8333333333, 1.0, -1.5}},
                   Point3D{{-1.0, 0.0, -1.6666666667}},
                   Point3D{{-0.5, 1.0, -1.6111111111}}}},
       Triangle3D{{Point3D{{-1.0, 0.0, -1.6666666667}},
                   Point3D{{-0.5, 0.0, -1.8333333333}},
                   Point3D{{-0.5, 1.0, -1.6111111111}}}},
       Triangle3D{{Point3D{{-0.5, 0.0, -1.8333333333}},
                   Point3D{{0.0, 0.0, -1.8888888889}},
                   Point3D{{-0.5, 1.0, -1.6111111111}}}},
       Triangle3D{{Point3D{{0.0, 0.0, -1.8888888889}},
                   Point3D{{0.0, 1.0, -1.6666666667}},
                   Point3D{{-0.5, 1.0, -1.6111111111}}}},
       Triangle3D{{Point3D{{0.0, 0.0, -1.8888888889}},
                   Point3D{{0.5, 0.0, -1.8333333333}},
                   Point3D{{0.0, 1.0, -1.6666666667}}}},
       Triangle3D{{Point3D{{0.5, 0.0, -1.8333333333}},
                   Point3D{{0.5, 1.0, -1.6111111111}},
                   Point3D{{0.0, 1.0, -1.6666666667}}}},
       Triangle3D{{Point3D{{0.8333333333, 1.0, -1.5}},
                   Point3D{{1.0, 0.75, -1.5}},
                   Point3D{{0.5, 1.0, -1.6111111111}}}},
       Triangle3D{{Point3D{{1.0, 0.75, -1.5}},
                   Point3D{{0.5, 1.0, -1.6111111111}},
                   Point3D{{1.0, 0.0, -1.6666666667}}}},
       Triangle3D{{Point3D{{0.5, 1.0, -1.6111111111}},
                   Point3D{{0.5, 0.0, -1.8333333333}},
                   Point3D{{1.0, 0.0, -1.6666666667}}}},
       Triangle3D{{Point3D{{-0.8333333333, 1.0, -1.5}},
                   Point3D{{-0.5, 1.0, -1.6111111111}},
                   Point3D{{-0.5, 1.1666666667, -1.5}}}},
       Triangle3D{{Point3D{{-0.5, 1.0, -1.6111111111}},
                   Point3D{{-0.5, 1.1666666667, -1.5}},
                   Point3D{{0.0, 1.25, -1.5}}}},
       Triangle3D{{Point3D{{-0.5, 1.0, -1.6111111111}},
                   Point3D{{0.0, 1.0, -1.6666666667}},
                   Point3D{{0.0, 1.25, -1.5}}}},
       Triangle3D{{Point3D{{0.0, 1.0, -1.6666666667}},
                   Point3D{{0.0, 1.25, -1.5}},
                   Point3D{{0.5, 1.1666666667, -1.5}}}},
       Triangle3D{{Point3D{{0.0, 1.0, -1.6666666667}},
                   Point3D{{0.5, 1.0, -1.6111111111}},
                   Point3D{{0.5, 1.1666666667, -1.5}}}},
       Triangle3D{{Point3D{{0.5, 1.0, -1.6111111111}},
                   Point3D{{0.8333333333, 1.0, -1.5}},
                   Point3D{{0.5, 1.1666666667, -1.5}}}},
       Triangle3D{{Point3D{{-0.8333333333, -1.0, -1.5}},
                   Point3D{{-1.0, -1.0, -1.3333333333}},
                   Point3D{{-0.5, -1.1666666667, -1.5}}}},
       Triangle3D{{Point3D{{-1.0, -1.0, -1.3333333333}},
                   Point3D{{-0.5, -1.1666666667, -1.5}},
                   Point3D{{-1.0, -1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-0.5, -1.1666666667, -1.5}},
                   Point3D{{-0.5, -1.9166666667, 0.0}},
                   Point3D{{-1.0, -1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-0.5, -1.1666666667, -1.5}},
                   Point3D{{0.0, -1.25, -1.5}},
                   Point3D{{-0.5, -1.9166666667, 0.0}}}},
       Triangle3D{{Point3D{{0.0, -1.25, -1.5}}, Point3D{{0.0, -2.0, 0.0}},
                   Point3D{{-0.5, -1.9166666667, 0.0}}}},
       Triangle3D{{Point3D{{0.0, -1.25, -1.5}},
                   Point3D{{0.5, -1.1666666667, -1.5}},
                   Point3D{{0.0, -2.0, 0.0}}}},
       Triangle3D{{Point3D{{0.5, -1.1666666667, -1.5}},
                   Point3D{{0.5, -1.9166666667, 0.0}},
                   Point3D{{0.0, -2.0, 0.0}}}},
       Triangle3D{{Point3D{{0.8333333333, -1.0, -1.5}},
                   Point3D{{1.0, -1.0, -1.3333333333}},
                   Point3D{{0.5, -1.1666666667, -1.5}}}},
       Triangle3D{{Point3D{{1.0, -1.0, -1.3333333333}},
                   Point3D{{0.5, -1.1666666667, -1.5}},
                   Point3D{{1.0, -1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{0.5, -1.1666666667, -1.5}},
                   Point3D{{0.5, -1.9166666667, 0.0}},
                   Point3D{{1.0, -1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-0.8333333333, -1.0, -1.5}},
                   Point3D{{-1.0, -0.75, -1.5}},
                   Point3D{{-1.0, -1.0, -1.3333333333}}}},
       Triangle3D{{Point3D{{0.8333333333, -1.0, -1.5}},
                   Point3D{{1.0, -0.75, -1.5}},
                   Point3D{{1.0, -1.0, -1.3333333333}}}},
       Triangle3D{{Point3D{{-1.0, 0.75, -1.5}},
                   Point3D{{-0.8333333333, 1.0, -1.5}},
                   Point3D{{-1.0, 1.0, -1.3333333333}}}},
       Triangle3D{{Point3D{{0.8333333333, 1.0, -1.5}},
                   Point3D{{1.0, 0.75, -1.5}},
                   Point3D{{1.0, 1.0, -1.3333333333}}}},
       Triangle3D{{Point3D{{-0.8333333333, 1.0, -1.5}},
                   Point3D{{-1.0, 1.0, -1.3333333333}},
                   Point3D{{-0.5, 1.1666666667, -1.5}}}},
       Triangle3D{{Point3D{{-1.0, 1.0, -1.3333333333}},
                   Point3D{{-0.5, 1.1666666667, -1.5}},
                   Point3D{{-1.0, 1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-0.5, 1.1666666667, -1.5}},
                   Point3D{{-0.5, 1.9166666667, 0.0}},
                   Point3D{{-1.0, 1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-0.5, 1.1666666667, -1.5}},
                   Point3D{{0.0, 1.25, -1.5}},
                   Point3D{{-0.5, 1.9166666667, 0.0}}}},
       Triangle3D{{Point3D{{0.0, 1.25, -1.5}}, Point3D{{0.0, 2.0, 0.0}},
                   Point3D{{-0.5, 1.9166666667, 0.0}}}},
       Triangle3D{{Point3D{{0.0, 1.25, -1.5}},
                   Point3D{{0.5, 1.1666666667, -1.5}},
                   Point3D{{0.0, 2.0, 0.0}}}},
       Triangle3D{{Point3D{{0.5, 1.1666666667, -1.5}},
                   Point3D{{0.5, 1.9166666667, 0.0}},
                   Point3D{{0.0, 2.0, 0.0}}}},
       Triangle3D{{Point3D{{0.8333333333, 1.0, -1.5}},
                   Point3D{{1.0, 1.0, -1.3333333333}},
                   Point3D{{0.5, 1.1666666667, -1.5}}}},
       Triangle3D{{Point3D{{1.0, 1.0, -1.3333333333}},
                   Point3D{{0.5, 1.1666666667, -1.5}},
                   Point3D{{1.0, 1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{0.5, 1.1666666667, -1.5}},
                   Point3D{{0.5, 1.9166666667, 0.0}},
                   Point3D{{1.0, 1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-1.0, -1.0, 1.3333333333}},
                   Point3D{{-0.8333333333, -1.0, 1.5}},
                   Point3D{{-1.0, -1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-0.8333333333, -1.0, 1.5}},
                   Point3D{{-1.0, -1.6666666667, 0.0}},
                   Point3D{{-0.5, -1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{-1.0, -1.6666666667, 0.0}},
                   Point3D{{-0.5, -1.9166666667, 0.0}},
                   Point3D{{-0.5, -1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{-0.5, -1.9166666667, 0.0}},
                   Point3D{{0.0, -2.0, 0.0}},
                   Point3D{{-0.5, -1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{0.0, -2.0, 0.0}}, Point3D{{0.0, -1.25, 1.5}},
                   Point3D{{-0.5, -1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{0.0, -2.0, 0.0}},
                   Point3D{{0.5, -1.9166666667, 0.0}},
                   Point3D{{0.0, -1.25, 1.5}}}},
       Triangle3D{{Point3D{{0.5, -1.9166666667, 0.0}},
                   Point3D{{0.5, -1.1666666667, 1.5}},
                   Point3D{{0.0, -1.25, 1.5}}}},
       Triangle3D{{Point3D{{0.8333333333, -1.0, 1.5}},
                   Point3D{{1.0, -1.0, 1.3333333333}},
                   Point3D{{0.5, -1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{1.0, -1.0, 1.3333333333}},
                   Point3D{{0.5, -1.1666666667, 1.5}},
                   Point3D{{1.0, -1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{0.5, -1.1666666667, 1.5}},
                   Point3D{{0.5, -1.9166666667, 0.0}},
                   Point3D{{1.0, -1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-1.0, -1.0, 1.3333333333}},
                   Point3D{{-0.8333333333, -1.0, 1.5}},
                   Point3D{{-1.0, -0.75, 1.5}}}},
       Triangle3D{{Point3D{{0.8333333333, -1.0, 1.5}},
                   Point3D{{1.0, -1.0, 1.3333333333}},
                   Point3D{{1.0, -0.75, 1.5}}}},
       Triangle3D{{Point3D{{-1.0, 1.0, 1.3333333333}},
                   Point3D{{-1.0, 0.75, 1.5}},
                   Point3D{{-0.8333333333, 1.0, 1.5}}}},
       Triangle3D{{Point3D{{0.8333333333, 1.0, 1.5}}, Point3D{{1.0, 0.75, 1.5}},
                   Point3D{{1.0, 1.0, 1.3333333333}}}},
       Triangle3D{{Point3D{{-1.0, 1.0, 1.3333333333}},
                   Point3D{{-0.8333333333, 1.0, 1.5}},
                   Point3D{{-1.0, 1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-0.8333333333, 1.0, 1.5}},
                   Point3D{{-1.0, 1.6666666667, 0.0}},
                   Point3D{{-0.5, 1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{-1.0, 1.6666666667, 0.0}},
                   Point3D{{-0.5, 1.9166666667, 0.0}},
                   Point3D{{-0.5, 1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{-0.5, 1.9166666667, 0.0}}, Point3D{{0.0, 2.0, 0.0}},
                   Point3D{{-0.5, 1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{0.0, 2.0, 0.0}}, Point3D{{0.0, 1.25, 1.5}},
                   Point3D{{-0.5, 1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{0.0, 2.0, 0.0}}, Point3D{{0.5, 1.9166666667, 0.0}},
                   Point3D{{0.0, 1.25, 1.5}}}},
       Triangle3D{{Point3D{{0.5, 1.9166666667, 0.0}},
                   Point3D{{0.5, 1.1666666667, 1.5}},
                   Point3D{{0.0, 1.25, 1.5}}}},
       Triangle3D{{Point3D{{0.8333333333, 1.0, 1.5}},
                   Point3D{{1.0, 1.0, 1.3333333333}},
                   Point3D{{0.5, 1.1666666667, 1.5}}}},
       Triangle3D{{Point3D{{1.0, 1.0, 1.3333333333}},
                   Point3D{{0.5, 1.1666666667, 1.5}},
                   Point3D{{1.0, 1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{0.5, 1.1666666667, 1.5}},
                   Point3D{{0.5, 1.9166666667, 0.0}},
                   Point3D{{1.0, 1.6666666667, 0.0}}}},
       Triangle3D{{Point3D{{-0.8333333333, -1.0, 1.5}},
                   Point3D{{-0.5, -1.1666666667, 1.5}},
                   Point3D{{-0.5, -1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{-0.5, -1.1666666667, 1.5}},
                   Point3D{{-0.5, -1.0, 1.6111111111}},
                   Point3D{{0.0, -1.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{-0.5, -1.1666666667, 1.5}},
                   Point3D{{0.0, -1.25, 1.5}},
                   Point3D{{0.0, -1.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{0.0, -1.25, 1.5}},
                   Point3D{{0.0, -1.0, 1.6666666667}},
                   Point3D{{0.5, -1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{0.0, -1.25, 1.5}},
                   Point3D{{0.5, -1.1666666667, 1.5}},
                   Point3D{{0.5, -1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{0.5, -1.1666666667, 1.5}},
                   Point3D{{0.8333333333, -1.0, 1.5}},
                   Point3D{{0.5, -1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{-0.8333333333, -1.0, 1.5}},
                   Point3D{{-1.0, -0.75, 1.5}},
                   Point3D{{-0.5, -1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{-1.0, -0.75, 1.5}},
                   Point3D{{-0.5, -1.0, 1.6111111111}},
                   Point3D{{-1.0, 0.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{-0.5, -1.0, 1.6111111111}},
                   Point3D{{-0.5, 0.0, 1.8333333333}},
                   Point3D{{-1.0, 0.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{-0.5, -1.0, 1.6111111111}},
                   Point3D{{0.0, -1.0, 1.6666666667}},
                   Point3D{{-0.5, 0.0, 1.8333333333}}}},
       Triangle3D{{Point3D{{0.0, -1.0, 1.6666666667}},
                   Point3D{{0.0, 0.0, 1.8888888889}},
                   Point3D{{-0.5, 0.0, 1.8333333333}}}},
       Triangle3D{{Point3D{{0.0, -1.0, 1.6666666667}},
                   Point3D{{0.5, -1.0, 1.6111111111}},
                   Point3D{{0.0, 0.0, 1.8888888889}}}},
       Triangle3D{{Point3D{{0.5, -1.0, 1.6111111111}},
                   Point3D{{0.5, 0.0, 1.8333333333}},
                   Point3D{{0.0, 0.0, 1.8888888889}}}},
       Triangle3D{{Point3D{{0.8333333333, -1.0, 1.5}},
                   Point3D{{1.0, -0.75, 1.5}},
                   Point3D{{0.5, -1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{1.0, -0.75, 1.5}},
                   Point3D{{0.5, -1.0, 1.6111111111}},
                   Point3D{{1.0, 0.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{0.5, -1.0, 1.6111111111}},
                   Point3D{{0.5, 0.0, 1.8333333333}},
                   Point3D{{1.0, 0.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{-1.0, 0.75, 1.5}},
                   Point3D{{-0.8333333333, 1.0, 1.5}},
                   Point3D{{-1.0, 0.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{-0.8333333333, 1.0, 1.5}},
                   Point3D{{-1.0, 0.0, 1.6666666667}},
                   Point3D{{-0.5, 1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{-1.0, 0.0, 1.6666666667}},
                   Point3D{{-0.5, 0.0, 1.8333333333}},
                   Point3D{{-0.5, 1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{-0.5, 0.0, 1.8333333333}},
                   Point3D{{0.0, 0.0, 1.8888888889}},
                   Point3D{{-0.5, 1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{0.0, 0.0, 1.8888888889}},
                   Point3D{{0.0, 1.0, 1.6666666667}},
                   Point3D{{-0.5, 1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{0.0, 0.0, 1.8888888889}},
                   Point3D{{0.5, 0.0, 1.8333333333}},
                   Point3D{{0.0, 1.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{0.5, 0.0, 1.8333333333}},
                   Point3D{{0.5, 1.0, 1.6111111111}},
                   Point3D{{0.0, 1.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{0.8333333333, 1.0, 1.5}}, Point3D{{1.0, 0.75, 1.5}},
                   Point3D{{0.5, 1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{1.0, 0.75, 1.5}}, Point3D{{0.5, 1.0, 1.6111111111}},
                   Point3D{{1.0, 0.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{0.5, 1.0, 1.6111111111}},
                   Point3D{{0.5, 0.0, 1.8333333333}},
                   Point3D{{1.0, 0.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{-0.8333333333, 1.0, 1.5}},
                   Point3D{{-0.5, 1.1666666667, 1.5}},
                   Point3D{{-0.5, 1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{-0.5, 1.1666666667, 1.5}},
                   Point3D{{-0.5, 1.0, 1.6111111111}},
                   Point3D{{0.0, 1.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{-0.5, 1.1666666667, 1.5}},
                   Point3D{{0.0, 1.25, 1.5}},
                   Point3D{{0.0, 1.0, 1.6666666667}}}},
       Triangle3D{{Point3D{{0.0, 1.25, 1.5}}, Point3D{{0.0, 1.0, 1.6666666667}},
                   Point3D{{0.5, 1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{0.0, 1.25, 1.5}}, Point3D{{0.5, 1.1666666667, 1.5}},
                   Point3D{{0.5, 1.0, 1.6111111111}}}},
       Triangle3D{{Point3D{{0.8333333333, 1.0, 1.5}},
                   Point3D{{0.5, 1.1666666667, 1.5}},
                   Point3D{{0.5, 1.0, 1.6111111111}}}}}};
}

} // namespace marchingcubes::tests::expected
