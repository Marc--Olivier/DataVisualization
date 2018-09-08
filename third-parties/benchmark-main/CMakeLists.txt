cmake_minimum_required(VERSION 3.7)

project(GoogleBenchmarkMain)

find_package(benchmark REQUIRED)
get_target_property(benchmark_INTERFACE_INCLUDE_DIRECTORIES benchmark::benchmark INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "Found google-benchmark:\n     benchmark_INTERFACE_INCLUDE_DIRECTORIES=${benchmark_INTERFACE_INCLUDE_DIRECTORIES}\n     "
  "benchmark_INCLUDE_DIRS=${benchmark_INCLUDE_DIRS}\n     benchmark_LIBRARIES=${benchmark_LIBRARIES}")

add_library(GoogleBenchmarkMain
  GoogleBenchmarkMain.cpp
)
add_library(benchmark::main ALIAS GoogleBenchmarkMain)

target_link_libraries(GoogleBenchmarkMain
  PUBLIC benchmark::benchmark
)

apply_compilation_flags(GoogleBenchmarkMain)

add_subdirectory(tests)
