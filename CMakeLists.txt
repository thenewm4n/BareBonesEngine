cmake_minimum_required(VERSION 3.14)
project(BareBones VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_OBJECTS_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/obj)

# Per-configuration paths for object and binary files
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_OBJECTS_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/obj)
endforeach()

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
find_package(OpenGL REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES "src/*/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Makes src the root for includes
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio OpenGL::GL)

# ImGui files
file(GLOB IMGUI_SOURCES "src/*/imgui/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src/ui/imgui)