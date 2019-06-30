project(gui VERSION 1.0 LANGUAGES CXX)

include("third-parties/dcmtk.cmake")
include("third-parties/qt5.cmake")

add_subdirectory(resources)

set(CMAKE_AUTOMOC ON)

add_library(gui
	DicomReader.cpp
	DicomReader.h
	MCubesRenderer.cpp
	MCubesRenderer.h
	MCubesTools.cpp
	MCubesTools.h
	MCubesWindow.cpp
	MCubesWindow.h
)

target_include_directories(gui PUBLIC ${DCMTK_INCLUDE_DIRS})
target_link_libraries(gui PUBLIC marching-cubes Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} dcmimage gui_resources)
apply_compilation_flags(gui)

add_executable(datavisualization ${GUI_APP} MACOSX_BUNDLE
    main.cpp
#    images.qrc
#    mylogo.rc
)

target_link_libraries(datavisualization gui)
