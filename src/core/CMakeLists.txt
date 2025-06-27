add_library(core STATIC
    Assets.cpp
    EntityManager.cpp
    GameEngine.cpp
    #ConfigManager.cpp
)

target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/utils
)

target_link_libraries(core PUBLIC
    entities
    scenes
    systems
    imgui
    SFML::Graphics
)