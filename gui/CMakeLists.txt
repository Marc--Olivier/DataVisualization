project(gui VERSION 1.0 LANGUAGES CXX)

include("third-parties/dcmtk.cmake")
include("third-parties/qt5.cmake")

add_subdirectory(resources)

set(CMAKE_AUTOMOC ON)

add_library(gui
	DicomReader.cpp
	DicomReader.h
	MCubes.h
	MCubesAlgorithm.cpp
	MCubesAlgorithm.h
	MCubesAllConfigurations.cpp
	MCubesAllConfigurations.h
	MCubesAllConfigurations_init.cpp
	MCubesConfigurationArray.cpp
	MCubesConfigurationArray.h
	MCubesConfigurationGenerator.cpp
	MCubesCube.cpp
	MCubesCube.h
	MCubesData.cpp
	MCubesData.h
	MCubesGrid.cpp
	MCubesGrid.h
	MCubesRenderer.cpp
	MCubesRenderer.h
	MCubesSurface.cpp
	MCubesSurface.h
	MCubesTools.cpp
	MCubesTools.h
	MCubesWindow.cpp
	MCubesWindow.h
)

target_link_libraries(gui PUBLIC Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} dcmimage gui_resources)
set_property(TARGET gui PROPERTY CXX_STANDARD 17)

add_executable(datavisualization ${GUI_APP} MACOSX_BUNDLE
    main.cpp
#    images.qrc
#    mylogo.rc
)

target_link_libraries(datavisualization gui)
