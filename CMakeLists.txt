cmake_minimum_required(VERSION 4.0)
project(BareBones LANGUAGES CXX VERSION 1.0)

# Configures global (i.e. for all targets) output directories for binary and object files for single-configuration generators
# (I'm using Makefile on Mac and Ninja on Windows)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
)
FetchContent_MakeAvailable(SFML)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX "_d"
    RELWITHDEBINFO_POSTFIX "_rd"
    RELEASE_POSTFIX ""
)

# Flattening bin directory is only required in MSVC as it's a multi-config generator
if (MSVC)
    # Define single variable for the debug info folder
    set(DEBUG_INFO_DIR "${CMAKE_SOURCE_DIR}/bin/debug_info")

    set_target_properties(BareBones PROPERTIES 
        # All executables go to /bin (not e.g. /bin/Debug)
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin"

        # All PDB files go to /bin/debug_info
        PDB_OUTPUT_DIRECTORY_DEBUG "${DEBUG_INFO_DIR}"
        PDB_OUTPUT_DIRECTORY_RELEASE "${DEBUG_INFO_DIR}"
        PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO "${DEBUG_INFO_DIR}"
    )
endif()

add_subdirectory(src)