#include <iostream>

#include "Scene.h"

Scene::Scene()
	: m_game(nullptr)
{
}

Scene::Scene(GameEngine* gameEngine)
    : m_game(gameEngine)
{
    if (!m_game)
    {
        std::cerr << "Scene.cpp, Line 9: Game engine pointer is null." << std::endl;
        exit(-7);
    }
}

void Scene::registerAction(int inputKey, const std::string& actionName)
{
    m_actionMap[inputKey] = actionName;
}

void Scene::simulate(const size_t frames)
{
	for (size_t i = 0; i < frames; i++)
	{
		update();
	}
}

size_t Scene::getCurrentFrame() const
{
    return m_currentFrame;
}

const ActionMap& Scene::getActionMap() const
{
    return m_actionMap;
}

bool Scene::hasEnded() const
{
    return m_hasEnded;
}