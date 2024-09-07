#pragma once

#include "EntityManager.h"
#include "Scene.h"

#include <memory>


class ScenePlatformer : public Scene
{
    struct PlayerConfig
    {
        float X = 0.0f, Y = 0.0f, BB_WIDTH = 1.0f, BB_HEIGHT = 1.0f, X_SPEED = 0.0f, JUMP_SPEED = 0.0f, MAX_SPEED = 10.0f, GRAVITY = 2.0f;
        std::string WEAPON = "BOW";
    };

public:
    ScenePlatformer(GameEngine* game, const std::string& levelPath);

private:
    std::unordered_map<PlayerState, std::string> m_stateToAnimationMap =
    {
        {PlayerState::Idle, "ArcherIdle"},
        {PlayerState::Running, "ArcherRun"},
        {PlayerState::Jumping, "ArcherJump"},
        {PlayerState::Falling, "ArcherFall"},
        {PlayerState::Shooting, "ArcherShoot"}
    };

    std::string m_levelPath;
    std::shared_ptr<Entity> m_player;
    PlayerConfig m_playerConfig;
    EntityManager m_entityManager;
    sf::Vector2f m_viewSize{1536.f, 864.f};          // Was 320.f, 180.f on Mac, 384.f, 216.f on PC
    const Vec2i m_gridCellSize{64, 64};
    sf::Text m_gridText;
    bool m_drawBoundingBoxes = false;
    bool m_drawGrid = false;
    bool m_drawTextures = true;

    void init();
    void loadLevel(const std::string& filename);
    void update() override;
    void endScene() override;

    // System methods
    void sDoAction(const Action& action) override;
    void sMovement();
    void sLifespan();
    void sCollision();
    void sAnimation();
    void sRender() override;

    // Spawning methods
    void spawnPlayer();
    void spawnArrow(std::shared_ptr<Entity> entity);
    
    // Render helper methods
    Vec2f gridToMidPixel(const Vec2f& gridPosition, std::shared_ptr<Entity> entity);
    void renderEntity(std::shared_ptr<Entity> e);
    void renderGrid(sf::RenderWindow& window, const sf::View& view);
    void renderBBox(std::shared_ptr<Entity> entity);

    // Animation helper methods
    void updatePlayerAnimation(CAnimation& animComponent);
    void updateEntityAnimation(std::shared_ptr<Entity> entity, CAnimation& animComponent);
    void spawnTempAnimation(Vec2f position, std::string animationName);

    // Collision helper methods
    void handlePlayerCollision(std::shared_ptr<Entity> object);
    void handleArrowSolidCollision(std::shared_ptr<Entity> arrow, std::shared_ptr<Entity> object);
    void destroySolid(std::shared_ptr<Entity> solid);
};