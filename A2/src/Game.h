#pragma once

#include "imgui.h"
#include "imgui-SFML.h"

struct PlayerConfig { int shapeRadius, collisionRadius, fillR, fillG, fillB, outlineR, outlineG, outlineB, outlineThick, vertices; float speed; };
struct EnemyConfig { int shapeRadius, collisionRadius, outlineR, outlineG, outlineB, outlineThick, verticesMax, verticesMin, lifespan, spawnInterval; float speedMax, speedMin;};
struct BulletConfig { int shapeRadius, collisionRadius, fillR, fillG, fillB, outlineR, outlineG, outlineB, outlineThick, vertices, lifespan; float speed; };

class Game
{
private:
    sf::RenderWindow m_window;
    EntityManager m_entities;
    sf::Font m_font;
    sf::Text m_text;
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;
    sf::Clock m_deltaClock;
    int m_score = 0;
    int m_currentFrame = 0;
    bool m_isPaused = false;
    std::shared_ptr<Entity> m_player;

public:
    Game(const std::string& configFilePath);
    void init(const std::string& configFilePath);    
    void run();
    void spawnPlayer();
    // void setPaused(bool isPaused);
};