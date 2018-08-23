project(utils VERSION 1.0 LANGUAGES CXX)

#------- utils -------#

add_library(utils
	internal/TinyContainer.hpp
	internal/TinyPermutation.hpp
	Math.hpp
	TinyArray.hpp
	TinyPermutation.hpp
	TinyVector.hpp
	empty.cpp
)

target_include_directories(utils PUBLIC ..)

apply_compilation_flags(utils)

#------- testUtils -------#

add_executable(testUtils
	tests/testMath.cpp
	tests/testTinyArray.cpp
	tests/testTinyContainer.cpp
	tests/testTinyPermutation.cpp
	tests/testTinyVector.cpp
)

target_link_libraries(testUtils
	PUBLIC utils catch-main
)

apply_compilation_flags(testUtils)

add_catch_test(testUtils)

