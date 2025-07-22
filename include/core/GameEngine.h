#pragma once

#include <memory>

#include "Assets.h"
#include "Scene.h"

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

/**
 * @brief Core game engine managing the main loop, scenes, and assets.
 * 
 * Not thread-safe - use from main thread only.
 * 
 * @code
 * GameEngine engine("/path/to/executable");
 * engine.run();
 * @endcode
 */
class GameEngine
{
public:
    /**
     * @brief Initialises game with executable directory for config and asset loading.
     * @param executableDir Path used to locate config and asset files
     */
    GameEngine(const std::filesystem::path& executableDir);

    /**
     * @brief Starts the main game loop.
     */
    void run();

    void quit();

    /**
     * @brief Either creates new scene or transitions to pre-existing scene; allows for different game states or levels.
     * @param sceneName The name of the new scene to switch to.
     * @param scene A shared pointer to the new scene object.
     */
    void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene);

    bool isRunning();
    const std::filesystem::path& getExecutableDir();
    sf::RenderWindow& getWindow();
    sf::Vector2u getResolution();
    float getAspectRatio();
    const Assets& getAssets() const;

private:
    void init();
    void update();
    void sUserInput();
    void handleSceneAction(const sf::Keyboard::Key& keyCode, bool isKeyPressed);
    void takeScreenshot();
    void resize(const sf::Vector2u& newSize);
    std::shared_ptr<Scene> getCurrentScene();

    static constexpr const char* GAME_TITLE = "Game Engine: The Game!";
    static constexpr const char* LEVELS[] = {"Platformer", "Mario"};    // This currently does nothing

    std::filesystem::path m_executableDir;
    sf::RenderWindow m_window;
    sf::Vector2u m_resolution;
    float m_aspectRatio;
    bool m_running = true;
    size_t m_simulationSpeed = 1;

    std::string m_currentScene;
    Assets m_assets;
    SceneMap m_sceneMap;
};