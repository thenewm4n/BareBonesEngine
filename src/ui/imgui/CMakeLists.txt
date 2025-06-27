add_library(imgui STATIC
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui-SFML.cpp
    imgui.cpp
)

target_include_directories(imgui PUBLIC
    ${CMAKE_SOURCE_DIR}/include/ui/imgui
)

target_link_libraries(imgui PUBLIC SFML::Graphics OpenGL::GL)