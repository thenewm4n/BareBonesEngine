#pragma once

#include "engine/core/GameEngine.h"
#include "engine/scene/Scene.h"
#include "engine/systems/Action.h"

#include <stack>
#include <vector>


struct Menu
{
    std::string title;
    std::vector<std::string> options;
    uint8_t selectedIndex;

    Menu(const std::string& t, const std::vector<std::string>& opts)
        : title(t), options(opts), selectedIndex(0) {}
};

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
    std::stack<Menu> m_menuStack;
    sf::Text m_menuText;

    void init();
    void update() override;
    void sPerformAction(const Action& action) override;
    void sRender() override;
    void endScene() override;
};