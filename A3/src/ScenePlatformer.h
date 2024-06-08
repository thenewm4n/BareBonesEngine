#pragma once

#include <map>
#include <memory>

#include "EntityManager.h"
#include "Scene.h"

class ScenePlatformer : public Scene
{
    struct PlayerConfig
    {
        float X, Y, BB_WIDTH, BB_HEIGHT, X_SPEED, MAX_SPEED, JUMP_SPEED, GRAVITY;
        std::string WEAPON;
    };

public:
    ScenePlatformer(GameEngine* game, const std::string& levelPath);

private:
    std::shared_ptr<Entity> m_player;
    PlayerConfig m_playerConfig;
    std::string m_levelPath;
    EntityManager m_entityManager;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    bool m_drawTextures = true;
    const Vec2i m_gridCellSize = { 64, 64 };
    sf::Text m_gridText;

private:
    void init(const std::string& levelPath);
    void loadLevel(const std::string& filename);
    void update() override;
    void endScene() override;

    void sDoAction(const Action& action) override;
    void sMovement();
    void sLifespan();
    void sCollision();
    void sAnimation();
    void sRender() override;

    void spawnPlayer();
    void spawnBullet(std::shared_ptr<Entity> entity);
    
    Vec2f gridToMidPixel(float gridPositionX, float gridPositionY, std::shared_ptr<Entity> entity);
};