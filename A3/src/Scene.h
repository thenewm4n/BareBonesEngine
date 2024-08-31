#pragma once

#include "Action.h"

#include <map>

using ActionMap = std::map<int, std::string>;

class GameEngine;

class Scene
{
public:
    Scene();
    Scene(GameEngine* gameEngine);

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;

    void registerAction(int inputKey, const std::string& actionName);
    void simulate(const size_t frames);

    size_t getCurrentFrame() const;
    const ActionMap& getActionMap() const;
    bool hasEnded() const;

protected:
    GameEngine* m_game = nullptr;
    ActionMap m_actionMap;
    bool m_paused = false;
    bool m_hasEnded = false;
    size_t m_currentFrame = 0;

    virtual void sRender() = 0;
    virtual void endScene() = 0;
};
