add_library(entities STATIC
    Entity.cpp
)

target_include_directories(entities PUBLIC
    ${CMAKE_SOURCE_DIR}/include/entities
)

target_link_libraries(entities PUBLIC core)