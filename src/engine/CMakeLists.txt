# Gather source files
set(ENGINE_SOURCES
    core/GameEngine.cpp 
    core/ConfigManager.cpp
    core/Assets.cpp
    entity/Entity.cpp
    entity/EntityManager.cpp
    scene/Scene.cpp
    systems/Action.cpp 
    systems/Animation.cpp 
    systems/Physics.cpp
)

set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui-SFML.cpp
)

# Add sources into the Engine library
add_library(Engine STATIC
    ${ENGINE_SOURCES}
    ${IMGUI_SOURCES}
)

# Include headers
target_include_directories(Engine PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/vendor/imgui
)

# Link dependencies
target_link_libraries(Engine PUBLIC
    SFML::Graphics
    SFML::Window
    SFML::System
    OpenGL::GL
)