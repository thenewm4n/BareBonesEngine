#pragma once

#include <map>
#include <memory>
// #include <string>

#include <SFML/Graphics.hpp>

#include "Action.h"

class GameEngine;

using ActionMap = std::map<int, std::string>;

// Abstract base class
class Scene
{
protected:
    GameEngine* m_game = nullptr;
    ActionMap m_actionMap;
    bool m_paused = false;
    bool m_hasEnded = false;
    size_t m_currentFrame = 0;

protected:      // Called by derived Scene object
    virtual void sRender() = 0;
    virtual void endScene() = 0;
    void setPaused(bool paused);

public:         // Called by GameEngine object
    Scene();
    Scene(GameEngine* gameEngine = nullptr);

    virtual void update() = 0;
    virtual void sDoAction(const Action &action) = 0;

    void registerAction(int inputKey, const std::string& actionName);

    size_t getCurrentFrame() const;
    const ActionMap& getActionMap() const;
    bool hasEnded() const;
    /*
    size_t getWidth() const;
    size_t getHeight() const;
    */
};
