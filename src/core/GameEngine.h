#pragma once

#include <memory>

#include "core/Assets.h"
#include "scenes/Scene.h"

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

/**
 * The core class responsible for initialising the game, managing the core game 
 * loop, managing scenes and providing access to assets and the render window.
 * 
 * Thread Safety:
 * - This class is not thread-safe. All methods should be called from the main thread.
 *
 * Usage Example:
 * @code
 * int main() {
 *     GameEngine engine("config.txt");
 *     engine.run();
 *     return 0;
 * }
 * @endcode
 *
 * Dependencies:
 * - SFML (Simple and Fast Multimedia Library) for rendering, window management and sound.
 * - A valid configuration file for initializing the game engine.
 */
class GameEngine
{
public:
    /**
     * Initializes the game engine with the provided configuration file path.
     * Sets up resources, loads assets, and prepares the initial game state.
     *
     * Parameters:
     * @param configFilePath Path to the configuration file used to initialize the game engine.
     *
     * Side Effects:
     * - Initializes the game engine's internal state.
     * - Loads game assets from the specified configuration file.
     * - Sets up the initial scene and render window.
     *
     * Dependencies:
     * - Requires a valid configuration file at the specified path.
     */
    GameEngine(const std::string& configFilePath);

    /**
     * Runs the main game loop, updating the game state and rendering frames until the game is quit.
     *
     * Side Effects:
     * - Continuously updates the game state and renders frames.
     * - Processes user input and handles scene transitions.
     */
    void run();

    /**
     * Quits the game loop and performs necessary cleanup before shutting down the game engine.
     *
     * Side Effects:
     * - Sets the running state to false, causing the main game loop to exit.
     */
    void quit();

    /**
     * Either creates new scene or transitions to pre-existing scene; allows for different game states or levels.
     *
     * Parameters:
     * @param sceneName The name of the new scene to switch to.
     * @param scene A shared pointer to the new scene object.
     *
     * Side Effects:
     * - Updates the current scene to the new scene.
     * - May trigger loading of new assets or resources.
     */
    void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene);

    /**
     * Checks if the game engine is currently running.
     *
     * Return Values:
     * @return True if the game engine is running, false otherwise.
     */
    bool isRunning();

    /**
     * Provides access to the render window used by the engine, mostly for rendering operations.
     *
     * Return Values:
     * @return A reference to the sf::RenderWindow object used by the game engine.
     *
     * Dependencies:
     * - Requires the game engine to be initialized and the render window to be created.
     */
    sf::RenderWindow& getWindow();

    /**
     * Gets the resolution of the render window.
     *
     * Return Values:
     * @return An sf::Vector2u object representing the resolution of the render window.
     */
    sf::Vector2u getResolution();

    /**
     * Gets the aspect ratio of the render window.
     *
     * Return Values:
     * @return A float representing the aspect ratio of the render window.
     */
    float getAspectRatio();

    /**
     * Gets the assets instance holding all loaded textures, animations, fonts and sounds.
     *
     * Return Values:
     * @return A constant reference to the Assets object managed by the game engine.
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