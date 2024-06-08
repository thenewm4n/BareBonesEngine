#pragma once

#include <map>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Action.h"

using ActionMap = std::map<int, std::string>;

class GameEngine;

class Scene
{
protected:
    GameEngine* m_game = nullptr;
    ActionMap m_actionMap;
    bool m_paused = false;
    bool m_hasEnded = false;
    size_t m_currentFrame = 0;

protected:
    virtual void sRender() = 0;
    virtual void endScene() = 0;
    void setPaused(bool paused);        // Necessary in most scenes, hence in Scene base class

public:
    Scene();
    Scene(GameEngine* gameEngine = nullptr);

    virtual void update() = 0;
    virtual void sDoAction(const Action &action) = 0;

    void registerAction(int inputKey, const std::string& actionName);

    size_t getCurrentFrame() const;
    const ActionMap& getActionMap() const;
    bool hasEnded() const;
};
