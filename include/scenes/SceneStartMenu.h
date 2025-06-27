#pragma once

#include "GameEngine.h"
#include "Scene.h"

#include <vector>

class SceneStartMenu : public Scene
{
public:
    SceneStartMenu(GameEngine* gameEngine);

private:
    std::string m_title;
    sf::Text m_menuText;
    std::vector<std::string> m_menuStrings;
    std::vector<std::string> m_levelPaths;
    uint8_t m_selectedMenuIndex = 0;

    void init();
    void update() override;
    void sPerformAction(const Action& action) override;
    void sRender() override;
    void endScene() override;
};