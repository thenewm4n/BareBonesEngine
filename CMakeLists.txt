cmake_minimum_required(VERSION 3.14)
project(BareBones VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio OpenGL::GL)

# ImGui files
file(GLOB IMGUI_SOURCES "src/imgui/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src/imgui)