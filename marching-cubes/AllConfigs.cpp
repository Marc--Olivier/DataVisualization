/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "marching-cubes/AllConfigs.hpp"

namespace marchingcubes {

AllConfigs::AllConfigs(
    const std::array<TrianglesOnCubeEdges, CONFIGS_COUNT> &triangles)
    : triangles{triangles} {}

} // namespace marchingcubes
