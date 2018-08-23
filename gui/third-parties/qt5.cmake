# Find the QtWidgets library
# Don't forget to add the path to Qt libraries to CMAKE_PREFIX_PATH variable.
# For example, you can set it when running cmake using:  
#    cmake -DCMAKE_PREFIX_PATH=<path_to_qt>/5.11.1/clang_64
find_package(Qt5Widgets CONFIG REQUIRED)
message(STATUS "Found Qt5Widgets:\n     Qt5Widgets_LIBRARIES=${Qt5Widgets_LIBRARIES}")

find_package(Qt5OpenGL CONFIG REQUIRED)
message(STATUS "Found Qt5OpenGL:\n     Qt5OpenGL_LIBRARIES=${Qt5OpenGL_LIBRARIES}")

find_package(OpenGL REQUIRED)
message(STATUS "Found OpenGL:\n     OpenGL_LIBRARIES=${OPENGL_LIBRARIES}")
