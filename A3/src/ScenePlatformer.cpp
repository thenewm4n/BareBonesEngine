#include <iostream>

#include "Action.h"
#include "Assets.h"
#include "Components.h"
#include "GameEngine.h"
#include "Physics.h"
#include "ScenePlatformer.h"
#include "SceneStartMenu.h"

ScenePlatformer::ScenePlatformer(GameEngine* game, const std::string& levelPath)
    : Scene(game), m_levelPath(levelPath)
{
    init(m_levelPath);
}

void ScenePlatformer::init(const std::string& levelPath)
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURES");
    registerAction(sf::Keyboard::C, "TOGGLE_BBOXES");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "SHOOT");

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->getAssets().getFont("Pixel"));

    loadLevel(levelPath);
}

void ScenePlatformer::update()
{
    if (m_paused)
	{
		return;
	}

    // Adds and removes entities while avoiding iterator invalidation
    m_entityManager.update();

    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sRender();
}

void ScenePlatformer::loadLevel(const std::string& filename)
{
    // Overwrites previous EntityManager
    m_entityManager = EntityManager();

    // TODO: read in level file and add appropriate entities to entity manager -> will automatically be drawn to screen
        // Add correct bounding boxes to Tile entities, no bounding boxes to Dec entities
    // Use playerConfig struct m_playerConfig to store player properties
    // This struct is defined at top of ScenePlatformer.h

    // NOTE: all of code below is sample code which shows you how to set up and use entities with the new syntax -> should be removed

    spawnPlayer();

    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT: always add CAnimatio component first so gridToMidPixel can compute correctly
    brick->addComponent<CAnimation>(m_game->getAssets().getAnimation("Brick"), true);
    brick->addComponent<CTransform>(Vec2f(96.f, 480.f));
    // NOTE: final code should position entity with grid x,y position read from file:
    // brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));

    if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
        std::cout << "This could be good way of identifying if a tile is a brick" << std::endl;
    }

    auto questionMarkBlock = m_entityManager.addEntity("tile");
    questionMarkBlock->addComponent<CAnimation>(m_game->getAssets().getAnimation("QuestionMark"), true);
    questionMarkBlock->addComponent<CTransform>(Vec2f(352.f, 480.f));

    // NOTE - THIS IS IMPORTANT, READ THIS SAMPLE
        // Components are now returned as references, not pointers.
        // If you do not specify a reference variable type, it will only copy.
        //The follow is an example:
            // auto transform1 = entity->getComponent<CTransform>()
            // transform one is a copy
            // auto& transform2 = entity->getComponent<CTransform>()
            // This is correct
}

void ScenePlatformer::endScene()
{
    m_game->changeScene("MENU", std::make_shared<SceneStartMenu>(m_game));
}

void ScenePlatformer::sMovement()
{
    // TODO: Implement player movemement/jumping based on its CInput component
        // Use scales of -1/1 to look left/right
    // TODO: Implement gravity's effect on player
    // TODO: Implement max player speed in both X and Y directions

    // Update player velocity according to input
    Vec2f velocity(0, m_player->getComponent<CTransform>().velocity.y);
    if (m_player->getComponent<CInput>().up)
    {
        // m_player.getComponent<CState>().state = "jumping";      // Used for changing player animation
        velocity.y = -3;
    }
    m_player->getComponent<CTransform>().velocity = velocity;

    // For each entity in scene, handle movement according to velocities
    for (auto entity : m_entityManager.getEntities())   // Why isn't this auto&? Are they already references?
    {
        /*
        auto& velocity = entity->getComponent<CTransform>().velocity;

        if (entity->hasComponent<CGravity>())
        {
            velocity.y += entity->getComponent<CGravity>().acceleration;
        }

        // Should cap velocity in all directions to ensure no moving throughfloors
        if (velocity.y > m_playerConfig.MAX_SPEED)
        {
            velocity.y = m_playerConfig.MAX_SPEED;
        }
        if (velocity.x > m_playerConfig.MAX_SPEED)
        {
            velocity.x = m_playerConfig.MAX_SPEED;
        }
        */

        // Update Entity's position according to velocity
        entity->getComponent<CTransform>().position += velocity;
    }
}

void ScenePlatformer::sLifespan()
{
    // TODO: Check lifespan of entities that have them, and destroy if they go over
}

void ScenePlatformer::sCollision()
{
    // TODO: Implement Physics::getOverlap() to use inside this function

    // TODO: Implement bullet/tile collisions
        // Destroy tile if it has Brick animation
    // TODO: Implement player/tile collisions and resolutions
        // Update the CState component of player to store whether it's currently on ground or in air; this will be used by Animation system
    // TODO: Check to see if player has fallen down hole i.e. y > height
    // TODO: Don't let player walk off left side of map
}

void ScenePlatformer::sDoAction(const Action& action)
{
    const std::string& actionName = action.getName();

    if (action.getType() == "START")
    {
        if (actionName == "TOGGLE_TEXTURE")
        {
            m_drawTextures = !m_drawTextures;
        }
        else if (actionName == "TOGGLE_COLLISION")
        {
            m_drawBoundingBoxes = !m_drawBoundingBoxes;
        }
        else if (actionName == "TOGGLE_GRID")
        {
            m_drawGrid = !m_drawGrid;
        }
        else if (actionName == "PAUSE")
        {
            m_paused = !m_paused;
        }
        else if (actionName == "QUIT")
        {
            endScene();
        }
        else if (actionName == "UP")
        {
            m_player->getComponent<CInput>().up = true;
        }
        else if (actionName == "DOWN")
        {
            m_player->getComponent<CInput>().down = true;
        }
        else if (actionName == "LEFT")
        {
            m_player->getComponent<CInput>().left = true;
        }
        else if (actionName == "RIGHT")
        {
            m_player->getComponent<CInput>().right = true;
        }
    }
    else if (action.getType() == "END")
    {
        if (actionName == "UP")
        {
            m_player->getComponent<CInput>().up = false;
        }
        else if (actionName == "DOWN")
        {
            m_player->getComponent<CInput>().down = false;
        }
        else if (actionName == "LEFT")
        {
            m_player->getComponent<CInput>().left = false;
        }
        else if (actionName == "RIGHT")
        {
            m_player->getComponent<CInput>().right = false;
        }
    }
}

void ScenePlatformer::sAnimation()
{
    // Set player animation based on state
    for (auto& entity : m_entityManager.getEntities())
    {
        // Change player Animation according to player CState
        const auto& stateComponent = m_player->getComponent<CState>();

        if (stateComponent.state == "running")
        {
            m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Run"), true);
        }
        else if (stateComponent.state == "standing")
        {
            m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Stand"), true);
        }
        else if (stateComponent.state == "inAir")
        {
            m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Air"), true);
        }
        else if (stateComponent.state == "shooting")
        {
            m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Shoot"), false);
        }

        // If entity has Animation component, call update()
        if (entity->hasComponent<CAnimation>())
        {
            CAnimation& animationComponent = entity->getComponent<CAnimation>();

            if (animationComponent.animation.hasEnded() && !animationComponent.toRepeat)
            {
                entity->removeComponent<CAnimation>();
            }
            else
            {
                animationComponent.animation.update();
            }
        }
    }
}

void ScenePlatformer::sRender()
{
    sf::RenderWindow& window = m_game->getWindow();
    Vec2f viewSize(426.f, 240.f);   // 240p in 16:9

    // Different colour background to indicate game paused
    if (m_paused)
    {
        window.clear(sf::Color(50, 50, 150));          // Dark blue
    }
    else
    {
        window.clear(sf::Color(100, 100, 255));        // Indigo 
    }

    // Centres view on player if further to right than middle of screen
    auto& playerPosition = m_player->getComponent<CTransform>().position;
    // float newViewCentreX = std::max(resolution.x / 2.f, playerPosition.x);
    float newViewCentreX = std::max(viewSize.x / 2.f, playerPosition.x);
    sf::View view = window.getView();
    view.setCenter(newViewCentreX, view.getCenter().y);    // This was m_game->getWindow().getSize().y - view.getCenter().y -> used to mirror the view in the vertical midline
    view.setSize(viewSize.x, viewSize.y);
    window.setView(view);

    // Drawing of textures/animations and bounding boxes
    for (auto entity : m_entityManager.getEntities())
    {
        // Draw Entity textures/animations
        if (m_drawTextures && entity->hasComponent<CAnimation>())
        {
            auto& transform = entity->getComponent<CTransform>();
            auto& animation = entity->getComponent<CAnimation>().animation;
            animation.getSprite().setPosition(transform.position.x, transform.position.y);
            animation.getSprite().setScale(transform.scale.x, transform.scale.y);
            animation.getSprite().setRotation(transform.angle);
            window.draw(animation.getSprite());
        }

        // Draw Entity bounding boxes
        if (m_drawBoundingBoxes && entity->hasComponent<CBoundingBox>())
        {
            auto& bBox = entity->getComponent<CBoundingBox>();
            auto& transform = entity->getComponent<CTransform>();
            
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(bBox.size.x - 1, bBox.size.y - 1));  // Only takes sf::Vector2f
            rectangle.setOrigin(bBox.halfSize.x, bBox.halfSize.y);
            rectangle.setPosition(transform.position.x, transform.position.y);      
            rectangle.setFillColor(sf::Color(0, 0, 0, 0));                  // Sets alpha of fill colour to 0 i.e. transparent
            rectangle.setOutlineColor(sf::Color(255, 255, 255, 255));       // Sets alpha of outline to 255 i.e. opaque
            rectangle.setOutlineThickness(1);
            window.draw(rectangle);
        }
    }

    // Draw grid for debugging
    if (m_drawGrid)
    {
        float leftEdgeX = window.getView().getCenter().x - (viewSize.x / 2);              // Left edge of viewable area
        float rightEdgeX = leftEdgeX + viewSize.x + m_gridCellSize.x;                     // Right edge of viewable area - width of a cell is added to ensure full coverage
        float firstCellX = leftEdgeX - (static_cast<int>(leftEdgeX) % m_gridCellSize.x);    // X position of leftmost cell starting just outside of window

        float topEdgeY = window.getView().getCenter().y - (viewSize.y / 2);               // Top of viewable area
        float bottomEdgeY = topEdgeY + viewSize.y + m_gridCellSize.y;                     // Bottom of viewable area - height of cell added to ensure full coverage
        float firstCellY = topEdgeY - (static_cast<int>(topEdgeY) % m_gridCellSize.y);      // Y position of top cell starting just outside of window

        sf::VertexArray lines(sf::Lines);

        // Adds vertical lines to VertexArray
        for (float x = firstCellX; x < rightEdgeX; x += m_gridCellSize.x)
        {
            lines.append(sf::Vertex(sf::Vector2f(x, topEdgeY)));
            lines.append(sf::Vertex(sf::Vector2f(x, bottomEdgeY)));
        }

        // Adds horizontal line to VertexArray and draws coordinate text for each cell
        for (float y = firstCellY; y < bottomEdgeY; y += m_gridCellSize.y)
        {
            lines.append(sf::Vertex(sf::Vector2f(leftEdgeX, y)));
            lines.append(sf::Vertex(sf::Vector2f(rightEdgeX, y)));

            // For each cell, adds coordinate text
            for (float x = firstCellX; x < rightEdgeX; x += m_gridCellSize.x)
            {
                std::string xCellCoord = std::to_string(static_cast<int>(x) / m_gridCellSize.x);
                std::string yCellCoord = std::to_string(static_cast<int>(y) / m_gridCellSize.y);
                m_gridText.setString("(" + xCellCoord + ", " + yCellCoord + ")");
                m_gridText.setPosition(x + 3, bottomEdgeY - y - m_gridCellSize.y + 2);      // OR m_gridText.setPosition(x + 3, y + 2); ?
                window.draw(m_gridText);
            }
        }

        window.draw(lines);
    }

    window.display();
}

void ScenePlatformer::spawnPlayer()
{
    // Read player config from level file and spawn player (this is where player should restart after death)

    // sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation("Stand"), true);
    m_player->addComponent<CTransform>(Vec2f(224.f, 352.f));
    m_player->addComponent<CBoundingBox>(Vec2f(48.f, 48.f));
    m_player->addComponent<CGravity>(0.1f);

    // TODO: add remaining components to player
}

void ScenePlatformer::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at a given entity, going in direction entity is facing
}

Vec2f ScenePlatformer::gridToMidPixel(float gridPositionX, float gridPositionY, std::shared_ptr<Entity> entity)
{
    // TODO: Takes in a grid position where the bottom left of the entity is, and an Entity object
    // Returns Vec2 indicating the actual centre position of the entity (not a grid position)
    // Must use Entity's animation size
    // Grid width and height is stored in m_gridCellSize
    // Bottom left corner of animation should align with the bottom left of the grid cell

    return Vec2f(0, 0);
}