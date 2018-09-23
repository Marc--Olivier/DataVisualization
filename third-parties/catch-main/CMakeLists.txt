project(CatchMain VERSION 1.0 LANGUAGES CXX)

find_package(Catch2 REQUIRED)
get_target_property(catch2_INTERFACE_INCLUDE_DIRECTORIES Catch2::Catch2 INTERFACE_INCLUDE_DIRECTORIES)

message(STATUS "Found Catch2:\n     catch2_INTERFACE_INCLUDE_DIRECTORIES=${catch2_INTERFACE_INCLUDE_DIRECTORIES}\n     "
  "Catch2_INCLUDE_DIRS=${Catch2_INCLUDE_DIRS}\n     Catch2_LIBRARIES=${Catch2_LIBRARIES}")

add_library(catch-main
    CatchApprox.hpp
    CatchMain.cpp
)

target_link_libraries(catch-main PUBLIC Catch2::Catch2)
apply_compilation_flags(catch-main)

function(add_catch_test testName)
    add_test(NAME ${testName} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${testName})
endfunction()

