cmake_minimum_required(VERSION 3.8)

project(datavisualization_root)

enable_testing()

include("build-tools/compilation-flags.cmake")
add_subdirectory(third-parties)
add_subdirectory(gui)
