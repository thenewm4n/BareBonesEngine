#pragma once

#include "engine/core/GameEngine.h"
#include "engine/scene/Scene.h"
#include "engine/systems/Action.h"

#include <vector>

class SceneStartMenu : public Scene
{
public:
    SceneStartMenu(GameEngine* gameEngine, const std::string& title);

private:
    std::string m_title;
    sf::Text m_menuText;
    std::vector<std::string> m_menuStrings;
    std::vector<std::string> m_levelFiles;
    uint8_t m_selectedMenuIndex = 0;

    void init();
    void update() override;
    void sPerformAction(const Action& action) override;
    void sRender() override;
    void endScene() override;
};