#include "Scene.h"

Scene::Scene(GameEngine* gameEngine = nullptr)
    : m_game(gameEngine)
{
    if (!m_game)
    {
        std::cerr << "Scene.cpp, Line 9: Game engine pointer is null." << std::endl;
        exit(-7);
    }
}

Scene::registerAction(int inputKey, const std::string& actionName)
{
    m_actionMap[inputKey] = actionName;
}

void simulate(const size_t frames);

void drawLine(const Vec2& point1, const Vec2& point2)
{
    sf::Vertex line[] = { sf::Vertex(point1), sf::Vertex(point2) };
    window.draw(line, 2, sf::Lines);
}

size_t getWidth() const;
size_t getHeight() const;

size_t getCurrentFrame() const;
{
    return m_currentFrame;
}

const ActionMap& getActionMap() const
{
    return m_actionMap;
}

bool hasEnded() const
{
    return m_hasEnded;
}