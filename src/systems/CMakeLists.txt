add_library(systems STATIC
    Action.cpp
    Animation.cpp
    Physics.cpp
)

target_include_directories(systems PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(systems PUBLIC core entities SFML::Graphics)