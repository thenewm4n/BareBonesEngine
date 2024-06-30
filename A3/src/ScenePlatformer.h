#pragma once

#include "EntityManager.h"
#include "Scene.h"

#include <memory>


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
    std::string m_levelPath;
    std::shared_ptr<Entity> m_player;
    PlayerConfig m_playerConfig;
    EntityManager m_entityManager;
    sf::Vector2f m_viewSize{384.f, 216.f};          // Was 320.f, 180.f on Mac
    const Vec2i m_gridCellSize{16, 16};
    sf::Text m_gridText;
    bool m_drawBoundingBoxes = false;
    bool m_drawGrid = false;
    bool m_drawTextures = true;

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