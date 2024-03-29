#pragma once

#include "imgui.h"
#include "imgui-SFML.h"

#include "Entity.h"
#include "EntityManager.h"

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
    bool m_isRunning = true;
    bool m_isPaused = false;
    std::shared_ptr<Entity> m_player;
    int m_lastEnemySpawnTime = 0;

public:
    Game(const std::string& configFilePath);
    void init(const std::string& configFilePath);    
    void run();
    
    void sMovement();
    void sUserInput();
    void sRender();
    void sCollision();
    void sLifespan();
    void sEnemySpawner();
    void sGUI();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies();
    void spawnBullet();
    void spawnSpecialWeapon();
};