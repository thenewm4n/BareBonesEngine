#pragma once

#include <memory>

#include "Assets.h"
#include "Scene.h"

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

/**
 * Main game engine class that handles the core game loop, scene management, and rendering
 * 
 * The GameEngine class is responsible for:
 * - Initializing and managing the game window
 * - Managing the game loop
 * - Handling scene transitions
 * - Managing game assets
 * - Processing user input
 * - Maintaining core game state
 * 
 * The engine uses SFML for rendering and window management, and supports a scene-based
 * architecture where different game states are represented as Scene objects.
 */
class GameEngine
{
public:
    /**
     * Constructs a new GameEngine instance
     * @param configFilePath Path to the configuration file containing initial engine settings
     */
    GameEngine(const std::string& configFilePath);

    /**
     * Starts the main game loop
     * 
     * This function runs the core game loop which:
     * - Processes input
     * - Updates game logic
     * - Renders the current scene
     * The loop continues until quit() is called or the window is closed.
     */
    void run();

    /**
     * Signals the engine to stop running and exit the game loop
     */
    void quit();

    /**
     * Changes the currently active scene
     * @param sceneName Unique identifier for the scene
     * @param scene Shared pointer to the new scene instance
     */
    void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene);

    /**
     * Checks if the game engine is currently running
     * @return true if the engine is running, false otherwise
     */
    bool isRunning();

    /**
     * Gets the main render window
     * @return Reference to the SFML render window
     */
    sf::RenderWindow& getWindow();

    /**
     * Gets the current window resolution
     * @return Vector containing the window width and height
     */
    sf::Vector2u getResolution();

    /**
     * Gets the current aspect ratio of the window
     * @return The window's width divided by height
     * @note This is primarily used for camera zooming in the platformer scene
     */
    float getAspectRatio();

    /**
     * Gets the asset manager containing all game resources
     * @return Const reference to the Assets instance
     */
    const Assets& getAssets() const;

private:
    void init(const std::string& assetsFilePath);
    void update();
    void sUserInput();
    void takeScreenshot();
    std::shared_ptr<Scene> getCurrentScene();

    sf::RenderWindow m_window;
    sf::Vector2u m_resolution;
    float m_aspectRatio;
    Assets m_assets;
    std::string m_currentScene;
    SceneMap m_sceneMap;
    size_t m_simulationSpeed = 1;
    bool m_running = true;
};