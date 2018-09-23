/**
 * Copyright Marc-Olivier Andrez 2018.
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 */

#include "marching-cubes/MarchingCubes.hpp"
#include "marching-cubes/Tensor3D.hpp"

#include <benchmark/benchmark.h>

using namespace marchingcubes;

static MarchingCubes algo{};

static void BM_MarchingCubes(benchmark::State &state) {
  auto size = static_cast<size_t>(state.range(0));
  Grid3D grid{equidistantPoints(-1.0, 1.0, size),
              equidistantPoints(-2.0, 2.0, size),
              equidistantPoints(-3.0, 3.0, size)};
  auto sphere = createSphere(grid);
  for (auto _ : state)
    auto isoSurface = algo.isoSurface(grid, sphere, 4.0);
}

BENCHMARK(BM_MarchingCubes)->RangeMultiplier(2)->Range(8, 256);
