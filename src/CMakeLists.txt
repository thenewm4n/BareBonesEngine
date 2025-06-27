add_subdirectory(core)
add_subdirectory(entities)
add_subdirectory(scenes)
add_subdirectory(systems)
add_subdirectory(ui)

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE core entities scenes systems imgui)