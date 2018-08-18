qt5_add_resources(gui_resources_src gui_resources.qrc)

add_library(gui_resources
    ${gui_resources_src}
)
