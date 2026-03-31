add_library(scenes STATIC
    Scene.cpp
    SceneLevelEditor.cpp
    ScenePlatformer.cpp
    SceneStartMenu.cpp
)

target_include_directories(scenes PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(scenes PUBLIC core systems)