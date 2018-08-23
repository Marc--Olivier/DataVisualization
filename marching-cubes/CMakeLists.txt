project(marching-cubes VERSION 1.0 LANGUAGES CXX)

#-------  marching-cubes  -------#
add_library(marching-cubes
	AllConfigs.cpp
	AllConfigs.hpp
	ConfigsGenerator.cpp
	ConfigsGenerator.hpp
	Cube.hpp
	Geometry3D.hpp
	MarchingCubes.cpp
	MarchingCubes.hpp
	Tensor3D.cpp
	Tensor3D.hpp
	Triangle.hpp
)

target_include_directories(marching-cubes PUBLIC ..)

target_link_libraries(marching-cubes PUBLIC utils)

apply_compilation_flags(marching-cubes)

#-------  testMarchingCubes  -------#

add_executable(testMarchingCubes
	tests/expectedIsoSurfaces.hpp
	tests/testConfigsGenerator.cpp
	tests/testCube.cpp
	tests/testMarchingCubes.cpp
	tests/testTensor3D.cpp
)

target_link_libraries(testMarchingCubes
	PUBLIC marching-cubes catch-main
)

apply_compilation_flags(testMarchingCubes)

add_catch_test(testMarchingCubes)
