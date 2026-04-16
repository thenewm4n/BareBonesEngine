#pragma once

#include "engine/core/GameEngine.h"
#include "engine/scene/Scene.h"
#include "engine/systems/Action.h"

#include <vector>


struct LevelData
{
    std::string name;
    std::string path;
};


class SceneStartMenu : public Scene
{
public:
    SceneStartMenu(GameEngine* gameEngine);

private:
    std::string m_title;
    std::vector<LevelData> m_levels;
    sf::Text m_menuText;
    std::vector<std::string> m_menuStrings;
    uint8_t m_selectedMenuIndex = 0;

    void init();
    void update() override;
    void sPerformAction(const Action& action) override;
    void sRender() override;
    void endScene() override;
};