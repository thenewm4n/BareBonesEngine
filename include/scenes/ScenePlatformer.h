#pragma once

#include "core/EntityManager.h"
#include "scenes/Scene.h"

#include <memory>


class ScenePlatformer : public Scene
{
    struct PlayerConfig
    {
        float X = 0.0f, Y = 0.0f, BB_WIDTH = 1.0f, BB_HEIGHT = 1.0f, X_SPEED = 0.0f, JUMP_SPEED = 0.0f, MAX_SPEED = 10.0f, GRAVITY = 2.0f;
        std::string WEAPON = "BOW";
    };

public:
    ScenePlatformer(GameEngine* game, const std::filesystem::path& levelPath); //const std::string& levelPath);

protected:
    std::unordered_map<PlayerState, std::string> m_stateToAnimationMap =
    {
        {PlayerState::Idle, "ArcherIdle"},
        {PlayerState::Running, "ArcherRun"},
        {PlayerState::Jumping, "ArcherJump"},
        {PlayerState::Falling, "ArcherFall"},
        {PlayerState::Shooting, "ArcherShoot"}
    };

    std::filesystem::path m_levelPath;
    std::shared_ptr<Entity> m_player;
    PlayerConfig m_playerConfig;
    EntityManager m_entityManager;
    sf::Clock m_clock;

    sf::Vector2f m_viewSize{ 1152.0f, 648.0f };             // Was 1536.0f, 864.0f
    const int m_gridCellSize = 45;                   // was 64x64; changed to fit new assets
    sf::Text m_gridText;
    bool m_drawBoundingBoxes = false;
    bool m_drawGrid = false;
    bool m_drawTextures = true;
    bool m_drawGui = true;

    void init();
    void loadEntities(const std::filesystem::path& levelPath);
    void update() override;
    void endScene() override;

    // System methods
    void sPerformAction(const Action& action) override;
    void sMovement();
    void sLifespan();
    void sCollision();
    void sAnimation();
    void sRender() override;
    void sGUI();
    void sParallax(float viewDeltaX);

    // Spawning methods
    void spawnPlayer();
    void spawnArrow(std::shared_ptr<Entity> entity);
    
    // Render helper methods
    Vec2f gridToMidPixel(const Vec2f& gridPosition, std::shared_ptr<Entity> entity);
    void renderEntity(std::shared_ptr<Entity> e);
    void renderGrid();
    void renderBBox(std::shared_ptr<Entity> entity);

    // Animation helper methods
    void endAnimation(std::shared_ptr<Entity> entity);
    void changePlayerAnimation();
    void spawnTempAnimation(Vec2f position, std::string animationName);

    // Collision helper methods
    void handlePlayerCollision(std::shared_ptr<Entity> object);
    void handleArrowCollision(std::shared_ptr<Entity> arrow, std::shared_ptr<Entity> object);
    void destroySolid(std::shared_ptr<Entity> solid);
};