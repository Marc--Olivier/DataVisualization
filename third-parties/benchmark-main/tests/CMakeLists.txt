project(testBenchmark VERSION 1.0 LANGUAGES CXX)

add_executable(testBenchmark
	testBenchmark.cpp
)

target_link_libraries(testBenchmark
	PUBLIC benchmark::main
)

apply_compilation_flags(testBenchmark)
add_test(NAME testBenchmark COMMAND ${CMAKE_CURRENT_BINARY_DIR}/testBenchmark)
