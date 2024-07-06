#include "ScenePlatformer.h"

#include "Action.h"
#include "Assets.h"
#include "Components.h"
#include "GameEngine.h"
#include "Physics.h"
#include "SceneStartMenu.h"

#include <iostream>


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
    registerAction(sf::Keyboard::B, "TOGGLE_BBOXES");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "SHOOT");
    registerAction(sf::Keyboard::Equal, "ZOOM_IN");
    registerAction(sf::Keyboard::Hyphen, "ZOOM_OUT");

    m_gridText.setFont(m_game->getAssets().getFont("Tech"));
    m_gridText.setCharacterSize(50);
    m_gridText.setScale(0.05, 0.05);

    sf::View view(sf::FloatRect(0, 0, m_viewSize.x, m_viewSize.y));
    m_game->getWindow().setView(view);

    loadLevel(levelPath);
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

    // Change PlayerConfig struct according to level.txt, then spawn player with this struct
    spawnPlayer();

    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT: always add CAnimatio component first so gridToMidPixel can compute correctly
    brick->addComponent<CAnimation>(m_game->getAssets().getAnimation("Brick"), true);
    // brick->addComponent<CTransform>(Vec2f(96.f, 480.f));
    brick->addComponent<CTransform>(Vec2f(0.f, 0.f));
    // NOTE: final code should position entity with grid x,y position read from file:
    // brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));

    if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
        std::cout << "This could be good way of identifying if a tile is a brick" << std::endl;
    }

    auto questionMarkBlock = m_entityManager.addEntity("tile");
    questionMarkBlock->addComponent<CAnimation>(m_game->getAssets().getAnimation("QuestionMark"), true);
    questionMarkBlock->addComponent<CTransform>(Vec2f(24.f, 24.f));

    // NOTE - THIS IS IMPORTANT, READ THIS SAMPLE
        // Components are now returned as references, not pointers.
        // If you do not specify a reference variable type, it will only copy.
        //The follow is an example:
            // auto transform1 = entity->getComponent<CTransform>()
            // transform one is a copy
            // auto& transform2 = entity->getComponent<CTransform>()
            // This is correct
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

void ScenePlatformer::endScene()
{
    m_game->changeScene("MENU", std::make_shared<SceneStartMenu>(m_game));
}

void ScenePlatformer::sDoAction(const Action& action)
{
    const std::string& actionName = action.getName();

    if (action.getType() == "START")
    {
        if (actionName == "TOGGLE_TEXTURES")
        {
            m_drawTextures = !m_drawTextures;
        }
        else if (actionName == "TOGGLE_BBOXES")
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
        else if (actionName == "ZOOM_IN")
        {
            m_viewSize.y -= 20;
            m_viewSize.x = m_viewSize.y * m_game->getAspectRatio();
        }
        else if (actionName == "ZOOM_OUT")
        {
            m_viewSize.y += 20;
            m_viewSize.x = m_viewSize.y * m_game->getAspectRatio();
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

void ScenePlatformer::sMovement()
{
    // TODO: Implement max player speed in both X and Y directions

    // For each entity in scene, handle movement according to entity's velocity
    for (auto entity : m_entityManager.getEntities())
    {
        auto& transform = entity->getComponent<CTransform>();

        if (entity == m_player)
        {
            auto& input = m_player->getComponent<CInput>();
            auto& state = m_player->getComponent<CState>();

            // Change velocity and animation direction according to input
            if (input.right && !input.left)
            {
                // state.isFacingRight = true;           // Used in sAnimation()   
                transform.velocity.x = 2;
            }
            else if (input.left && !input.right)
            {
                //state.isFacingRight = false;
                transform.velocity.x = -2;
            }
            else
            {
                transform.velocity.x = 0;
            }

            // Jump if on ground and W pressed
            if (input.up && input.canJump)
			{
				transform.velocity.y = -3;
				input.canJump = false;
			}

            // Set CState according to horizontal velocity -> changes animation set in sAnimation()
            if (transform.velocity.x != 0)
            {
                state.currentState = PlayerState::Running;

                bool movingLeftMismatch = transform.velocity.x < 0 && transform.scale.x < 0 ? true : false;
                bool movingRightMismatch = transform.velocity.x > 0 && transform.scale.x > 0 ? true : false;

                // Change direction animation faces depending on velocity
                if (movingLeftMismatch || movingRightMismatch)           // if moving right while animation looks left or moving left while animations looks right
                {
                    transform.scale.x *= -1;                    // flip animation in x direction
                }

            }
            else
            {
                state.currentState = PlayerState::Standing;     // No change to direction when velocity is 0
            }
            
            // Falling animation overrides running or standing animations
            if (transform.velocity.y != 0)
            {
                state.currentState = PlayerState::InAir;
            }
        }

        if (entity->hasComponent<CGravity>())
		{
	        // transform.velocity.y += entity->getComponent<CGravity>().acceleration;
		}

        /*
        // Should cap velocity in all directions to ensure no moving throughfloors     DOESN'T WORK
        if (transform.velocity.y > m_playerConfig.MAX_SPEED)
        {
            transform.velocity.y = m_playerConfig.MAX_SPEED;
        }
        if (transform.velocity.x > m_playerConfig.MAX_SPEED)
        {
           transform.velocity.x = m_playerConfig.MAX_SPEED;
        }
        */

        // Update Entity's position according to velocity
        transform.position += transform.velocity;
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


    // If collision in y axis and coming from above, set y velocity to 0 and set player state to standing, and set CInput.canJump to true
}


void ScenePlatformer::sAnimation()
{
    // Set player animation based on state
    for (auto& entity : m_entityManager.getEntities())
    {
        if (entity == m_player)
		{
            CState& stateComponent = m_player->getComponent<CState>();
            CAnimation& animComponent = m_player->getComponent<CAnimation>();

            // If state has changed, replace animation according to state
            if (stateComponent.currentState != stateComponent.previousState)
            { 
                // Determine whether animation will be repeated or played once
                bool toRepeat = stateComponent.currentState == PlayerState::Shooting ? false : true;

                // Replace animation according to new state
                const std::string& animationName = m_stateToAnimationMap[stateComponent.currentState];
                animComponent = m_player->addComponent<CAnimation>(m_game->getAssets().getAnimation(animationName), toRepeat);
            
                // Update previous state to current state
                stateComponent.previousState = stateComponent.currentState;
            }
		}
      
        // If entity has Animation component, either update it, or remove if has ended
        if (entity->hasComponent<CAnimation>())
        {
            CAnimation& animComponent = entity->getComponent<CAnimation>();

            if (animComponent.animation.hasEnded() && !animComponent.toRepeat)
            {
                if (entity == m_player)
                {
                    const Animation& standingAnimation = m_game->getAssets().getAnimation(m_stateToAnimationMap[PlayerState::Standing]);
                    entity->addComponent<CAnimation>(standingAnimation, true);
                }
                else
                {
                    entity->removeComponent<CAnimation>();
                }
            }
            else
            {   
                animComponent.animation.update();       // Animation cycles by default
            }
        }
    }
}

void ScenePlatformer::sRender()
{
    sf::RenderWindow& window = m_game->getWindow();

    // Different colour background to indicate game paused
    if (m_paused)
    {
        window.clear(sf::Color(50, 50, 150));          // Dark blue
    }
    else
    {
        window.clear(sf::Color(100, 100, 255));        // Indigo 
    }

    // Establishes variables for centring view
    sf::View view = window.getView();
    view.setSize(m_viewSize);
    Vec2f playerPosition = m_player->getComponent<CTransform>().position;

    // Centres view on player if further to right than middle of screen
    float newViewCentreX = std::max(m_viewSize.x / 2.f, playerPosition.x);        // Ensures view doesn't exceed left side of level
    view.setCenter(newViewCentreX, view.getCenter().y);    // y value was window.getSize().y - view.getCenter().y
    window.setView(view);

    // Drawing of textures/animations and bounding boxes
    for (auto entity : m_entityManager.getEntities())
    {
        // Draw Entity textures/animations
        if (m_drawTextures && entity->hasComponent<CAnimation>())
        {
            auto& transform = entity->getComponent<CTransform>();
            auto& animationSprite = entity->getComponent<CAnimation>().animation.getSprite();
            animationSprite.setPosition(transform.position.x, transform.position.y);
            animationSprite.setScale(transform.scale.x, transform.scale.y);
            animationSprite.setRotation(transform.angle);
            window.draw(animationSprite);
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
        float leftEdgeX = view.getCenter().x - (m_viewSize.x / 2);                                  // Left edge of viewable area
        float rightEdgeX = leftEdgeX + m_viewSize.x + m_gridCellSize.x;                             // Right edge of viewable area - width of a cell is added to ensure full coverage
        float firstVertLineX = leftEdgeX - (static_cast<int>(leftEdgeX) % m_gridCellSize.x);        // X position of leftmost cell starting just outside of window
        float bottomEdgeY = m_viewSize.y;                                                           // Bottom of viewable area - height of cell added to ensure full coverage; top edge is 0

        sf::VertexArray lines(sf::Lines);

        // Add verticle lines to vertex array
        for (float x = firstVertLineX; x < rightEdgeX; x += m_gridCellSize.x)
        {
            lines.append(sf::Vertex(sf::Vector2f(x, 0)));
            lines.append(sf::Vertex(sf::Vector2f(x, bottomEdgeY)));
        }

        // Draw horizontal lines
        for (float y = bottomEdgeY; y > 0; y -= m_gridCellSize.y)
        {
            lines.append(sf::Vertex(sf::Vector2f(leftEdgeX, y)));
            lines.append(sf::Vertex(sf::Vector2f(rightEdgeX, y)));

            // Draw coordinate text for each cell
            for (float x = firstVertLineX; x < rightEdgeX; x += m_gridCellSize.x)
            {
                int gridX = static_cast<int>(x) / m_gridCellSize.x;
                int gridY = (m_viewSize.y - static_cast<int>(y)) / m_gridCellSize.y;

                m_gridText.setString("(" + std::to_string(gridX) + ", " + std::to_string(gridY) + ")");
                m_gridText.setPosition(x, y - m_gridCellSize.y);
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
    
    Animation standAnim = m_game->getAssets().getAnimation("Stand");
    m_player->addComponent<CAnimation>(standAnim, true);

    // Adding CTransform must follow adding CAnimation because gridToMidPixel uses CAnimation
    m_player->addComponent<CTransform>(gridToMidPixel(1.f, 1.f, m_player));
    
    const Vec2f& spriteSize = standAnim.getSize();
    m_player->addComponent<CBoundingBox>(Vec2f(spriteSize.x, spriteSize.y));

    m_player->addComponent<CGravity>(0.1f);
    
    // TODO: add remaining components to player
}

void ScenePlatformer::spawnBullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at a given entity, going in direction entity is facing
}

// Used to position of entity so bottom left of sprite is at bottom left of the cell
Vec2f ScenePlatformer::gridToMidPixel(float gridPositionX, float gridPositionY, std::shared_ptr<Entity> entity)
{
    if (!entity->hasComponent<CAnimation>())
    {
        std::cerr << "ScenePlatformer.cpp, Line 419: entity has no CAnimation." << std::endl;
        return Vec2f(0.f, 0.f);
    }

    const Vec2f& spriteSize = entity->getComponent<CAnimation>().animation.getSize();
    
    float x = (gridPositionX * m_gridCellSize.x) + (spriteSize.x / 2.f);
    float y = m_viewSize.y - (gridPositionY * m_gridCellSize.y) - spriteSize.y / 2.f;
        
    return Vec2f(x, y);
}