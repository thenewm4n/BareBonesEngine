# Gather game-specific source files
set(GAME_SOURCES 
    main.cpp
    SceneStartMenu.cpp
    SceneLevelEditor.cpp
    ScenePlatformer.cpp 
)

# Make executable
add_executable(BareBones ${GAME_SOURCES})

# Configure executable naming
set_target_properties(BareBones PROPERTIES
    DEBUG_POSTFIX "_d"
    RELWITHDEBINFO_POSTFIX "_rd"
    RELEASE_POSTFIX ""
)

# Set include directory
target_include_directories(BareBones PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add Engine as a library
target_link_libraries(BareBones PRIVATE Engine)

# Copy assets folder to bin folder
add_custom_command(TARGET BareBones POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:BareBones>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config
        $<TARGET_FILE_DIR:BareBones>/config
)