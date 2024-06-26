#pragma once

#include <memory>

#include "Assets.h"
#include "Scene.h"

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

class GameEngine
{
protected:
    sf::RenderWindow m_window;
    sf::Vector2u m_resolution;
    float m_aspectRatio;
    Assets m_assets;
    std::string m_currentScene;
    SceneMap m_sceneMap;
    size_t m_simulationSpeed = 1;
    bool m_running = true;

protected:
    void init(const std::string& assetsFilePath);
    void update();
    void sUserInput();

    std::shared_ptr<Scene> getCurrentScene();

public:
    GameEngine(const std::string& configFilePath);

    void run();
    void quit();
    void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene);

    bool isRunning();
    sf::RenderWindow& getWindow();
    sf::Vector2u getResolution();
    const Assets& getAssets() const;
};