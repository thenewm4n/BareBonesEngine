#pragma once

#include <vector>

#include "Scene.h"

class SceneStartMenu : public Scene
{
public:
    SceneStartMenu(GameEngine* gameEngine = nullptr);

private:
    std::string m_title;
    std::vector<std::string> m_menuStrings;
    std::vector<std::string> m_levelPaths;
    sf::Text m_menuText;
    uint8_t m_selectedMenuIndex = 0;

private:
    void init();
    void update() override;
    void sRender() override;
    void endScene() override;
    void sDoAction(const Action& action) override;
};