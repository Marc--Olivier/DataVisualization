cmake_minimum_required(VERSION 3.7)

# Project to compile all third parties
project(third_parties)
enable_testing(true)

# Be careful to correctly escapes whitespace in ${CMAKE_CURRENT_BINARY_DIR} using double quotes
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_BINARY_DIR}/install")
set(external_cmake_cxx_standard_options "\"-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install\" -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON")

# - Function to install external libraries
# 
# It uses the file `${name}.cmake.in` to download, compile, install,
# and run tests for the given module.
# 
# The following folders/files are generated:
#   * `${CMAKE_CURRENT_BINARY_DIR}/cmake/${name}/CMakeLists.txt` is generated from `${name}.cmake.in`
#   * `${CMAKE_CURRENT_BINARY_DIR}/cmake/${name}-build` contains the result of running cmake
#     on `${CMAKE_CURRENT_BINARY_DIR}/cmake/${name}/CMakeLists.txt`
# 
# In `${name}.cmake.in`, ensure that the library is installed into `${CMAKE_CURRENT_BINARY_DIR}/install`.
# For example, for cmake projects, define `CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install`.
# The easiest solution consists in using `CMAKE_ARGS ${external_cmake_cxx_standard_options}`.
# 
function(install_external name)
  message("============================\nInstalling ${name}\n============================")
  configure_file(${name}.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/${name}/CMakeLists.txt)
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cmake/${name}-build")
  execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" ${CMAKE_CURRENT_BINARY_DIR}/cmake/${name}
       WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cmake/${name}-build" )
  execute_process(COMMAND "${CMAKE_COMMAND}" --build .
      WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cmake/${name}-build" )
endfunction()

install_external(catch2)

add_subdirectory(catch-main)
