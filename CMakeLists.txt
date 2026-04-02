cmake_minimum_required(VERSION 4.0)
project(BareBones LANGUAGES CXX VERSION 1.0)

# Global Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensures flat bin directory with multi-config generators
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<0:>")
if (MSVC)
    set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/debug_info")
endif()

# Dependencies
include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.0.1)
FetchContent_MakeAvailable(SFML)
find_package(OpenGL REQUIRED)

add_subdirectory(src/engine)
add_subdirectory(src/game)