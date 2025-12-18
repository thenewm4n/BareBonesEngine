add_library(core STATIC
    Assets.cpp
    EntityManager.cpp
    GameEngine.cpp
    #ConfigManager.cpp
)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(core PUBLIC
    entities
    scenes
    systems
    imgui
    SFML::Graphics
)