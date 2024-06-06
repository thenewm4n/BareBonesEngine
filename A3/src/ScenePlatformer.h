#pragma once

#include <map>
#include <memory>

#include "EntityManager.h"
#include "Scene.h"

class ScenePlatformer : public Scene
{
    struct PlayerConfig
    {
        float X, Y, BB_WIDTH, BB_HEIGHT, X_SPEED, MAX_SPEED, JUMP_SPEED, GRAVITY
        std::string WEAPON;
    };

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

    void sDoAction() override;
    void sMovement();
    void sLifepan();
    void sCollision();
    void sAnimation();
    void sRender() override;

    void spawnPlayer();
    void spawnBullet(std::shared_ptr<Entity> entity);
    
    Vec2 gridToMidPixel(float gridPositionX, float gridPositionY, std::shared_ptr<Entity> entity);
    void simulate(const size_t frames);
    void drawLine(const Vec2& point1, const Vec2& point2);
};