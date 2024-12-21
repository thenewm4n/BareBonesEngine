#pragma once


#include "core/EntityManager.h"
#include "scenes/Scene.h"

#include <memory>


class SceneLevelEditor : public Scene
{
    struct PlayerConfig
    {
        float X = 0.0f, Y = 0.0f, BB_WIDTH = 1.0f, BB_HEIGHT = 1.0f, X_SPEED = 0.0f, JUMP_SPEED = 0.0f, MAX_SPEED = 10.0f, GRAVITY = 2.0f;
        std::string WEAPON = "BOW";
    };

public:
    SceneLevelEditor(GameEngine* game, const std::string& levelPath);

private:
    std::unordered_map<PlayerState, std::string> m_stateToAnimationMap =
    {
        {PlayerState::Idle, "ArcherIdle"},
        {PlayerState::Running, "ArcherRun"},
        {PlayerState::Jumping, "ArcherJump"},
        {PlayerState::Falling, "ArcherFall"},
        {PlayerState::Shooting, "ArcherShoot"}
    };

    std::shared_ptr<Entity> m_selectedEntity;
    std::shared_ptr<Entity> m_draggedEntity;
    std::string m_levelPath;
    EntityManager m_entityManager;
    std::shared_ptr<Entity> m_player;
    PlayerConfig m_playerConfig;
    sf::Clock m_clock;

    sf::Vector2f m_viewSize{ 1152.0f, 648.0f };             // Was 1536.0f, 864.0f
    const Vec2i m_gridCellSize{ 45, 45 };                   // was 64x64
    sf::Text m_gridText;
    bool m_drawBoundingBoxes = false;
    bool m_drawGrid = false;
    bool m_drawTextures = true;
    bool m_drawGui = true;

    void init();
    void loadLevel(const std::string& filename);
    void update() override;
    void saveToFile();
    void endScene() override;

    // System methods
    void sPerformAction(const Action& action) override;
    void sRender() override;
    void sGUI();
    void sParallax(float viewDeltaX);

    void spawnPlayer();

    // Render helper methods
    Vec2f gridToMidPixel(const Vec2f& gridPosition, std::shared_ptr<Entity> entity);
    void renderEntity(std::shared_ptr<Entity> e);
    void renderGrid();
    void renderBBox(std::shared_ptr<Entity> entity);
};