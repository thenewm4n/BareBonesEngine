cmake_minimum_required(VERSION 4.0)
project(BareBones LANGUAGES CXX VERSION 1.0)

# Configures global (i.e. for all targets) output directories for binary and object files for single-configuration generators
# (I'm using Makefile on Mac and Ninja on Windows)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Window)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "_d"
    RELWITHDEBINFO_POSTFIX "_rd"
    RELEASE_POSTFIX ""
)

add_subdirectory(src)