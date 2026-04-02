#pragma once

#include "engine/core/Assets.h"
#include "engine/scene/Scene.h"

#include <memory>


using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

class GameEngine
{
public:
    // executableDir is used to locate config and asset files relative to the .exe
    GameEngine(const std::filesystem::path& executableDir);

    void run();
    void quit();

    // Switches to a new scene. If the scene already exists in the map, it will transition to it.
    void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene);

    bool isRunning();
    const std::filesystem::path& getExecutableDir() const;
    sf::RenderWindow& getWindow();
    sf::Vector2u getResolution();
    float getAspectRatio();
    const Assets& getAssets() const;

    static constexpr const char* GAME_TITLE = "Game Engine: The Game";
    static constexpr const char* LEVELS[] = {"Platformer", "Mario"};    // This currently does nothing

private:
    void init();
    void update();
    void sUserInput();
    void handleSceneAction(const sf::Keyboard::Key& keyCode, bool isKeyPressed);
    void takeScreenshot();
    void resize(const sf::Vector2u& newSize);
    std::shared_ptr<Scene> getCurrentScene();

    std::filesystem::path m_executableDir;
    sf::RenderWindow m_window;
    sf::Vector2u m_resolution;
    float m_aspectRatio;
    std::uint16_t m_framerateCap = 60;
    bool m_running = true;
    size_t m_simulationSpeed = 1;

    std::string m_currentScene;
    Assets m_assets;
    SceneMap m_sceneMap;
};