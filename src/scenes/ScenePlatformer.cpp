#include "scenes/ScenePlatformer.h"

#include "systems/Action.h"
#include "core/Assets.h"
#include "core/Components.h"
#include "core/GameEngine.h"
#include "systems/Physics.h"
#include "scenes/SceneStartMenu.h"

#include <ui/imgui/imgui.h>
#include <ui/imgui/imgui-SFML.h>
#include <SFML/OpenGL.hpp>

#include <fstream>
#include <iostream>
#include <sstream>


ScenePlatformer::ScenePlatformer(GameEngine* game, const std::string& levelPath)
    : Scene(game), m_levelPath(levelPath)
{
    init();
}

void ScenePlatformer::init()
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURES");
    registerAction(sf::Keyboard::B, "TOGGLE_BBOXES");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    registerAction(sf::Keyboard::Tilde, "TOGGLE_MENU");
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "SHOOT");
    registerAction(sf::Keyboard::Equal, "ZOOM_IN");
    registerAction(sf::Keyboard::Hyphen, "ZOOM_OUT");

    float textScale = m_viewSize.x * 0.0001;
    m_gridText.setFont(m_game->getAssets().getFont("Tech"));
    m_gridText.setCharacterSize(50);
    m_gridText.setScale(textScale, textScale);

	sf::View view(sf::FloatRect(0, -m_viewSize.y, m_viewSize.x, m_viewSize.y));     // Top left corner of view is at (0, 0); view extends in negative y direction (upwards)
    m_game->getWindow().setView(view);

    loadLevel(m_levelPath);
}

void ScenePlatformer::loadLevel(const std::string& filename)
{
    // Overwrites previous EntityManager and parallax layers vector
    m_entityManager = EntityManager();

    std::ifstream levelFile(filename);
    if (!levelFile.is_open())
    {
        std::cerr << "ScenePlatformer.cpp, Line 60: Error opening level file." << std::endl;
        exit(-1);
    }

    std::string line;
    std::string token;
    unsigned int bgLayerIndex = 0;

    while (std::getline(levelFile, line))
    {
        // Enables reading of strings separated by whitespace
        std::istringstream lineStream(line);

        if (lineStream >> token)
        {
            if (token == "Player")
            {
                // Read values into PlayerConfig struct
                lineStream >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.BB_WIDTH
                >> m_playerConfig.BB_HEIGHT >> m_playerConfig.X_SPEED >> m_playerConfig.JUMP_SPEED
                >> m_playerConfig.MAX_SPEED >> m_playerConfig.GRAVITY >> m_playerConfig.WEAPON;

                // Spawn player according to PlayerConfig values
                spawnPlayer();
            }
            else
            {
                std::string animationName;
                Vec2f position;
                std::shared_ptr<Entity> entity;

                lineStream >> animationName >> position.x >> position.y;

                if (token == "Solid")
                {
                    entity = m_entityManager.addEntity("Solid");
                    entity->add<CBody>(m_game->getAssets().getAnimation(animationName).getSize());
                }
                else if (token == "Prop")  
                {
                    entity = m_entityManager.addEntity("Prop");
                }
                else        // i.e. if "Background"   
                {
                    entity = m_entityManager.addEntity("Background");

                    entity->add<CParallax>(bgLayerIndex);
                    bgLayerIndex++;
                }
                
                entity->add<CAnimation>(m_game->getAssets().getAnimation(animationName), true);    // IMPORTANT: add CAnimation first so gridToMidPixel can compute correctly
                entity->add<CTransform>(gridToMidPixel(position, entity));
            }
        }
    }

    levelFile.close();
}

void ScenePlatformer::update()
{
    if (!m_paused)
	{
        m_entityManager.update();    // Adds and removes entities while avoiding iterator invalidation

        sMovement();
        sLifespan();
        sCollision();
        sAnimation();
	}

    sGUI();
    sRender();

    m_currentFrame++;
}

void ScenePlatformer::endScene()
{
    m_game->changeScene("MENU", std::make_shared<SceneStartMenu>(m_game));
}

void ScenePlatformer::sPerformAction(const Action& action)
{
    const std::string& actionName = action.getName();

    if (action.getType() == "START")
    {
        // Check for pausing or quitting
        if (actionName == "PAUSE")
        {
            m_paused = !m_paused;
        }
        else if (actionName == "QUIT")
        {
            endScene();
        }

        // If paused, don't allow player input or toggling
        if (m_paused)
        {
            return;
        }

        if (m_player->has<CInput>())
        {
            // Handle player input and return early to decrease latency
            if (actionName == "UP")
            {
                m_player->get<CInput>().up = true;
                return;
            }
            else if (actionName == "DOWN")
            {
                m_player->get<CInput>().down = true;
                return;
            }
            else if (actionName == "LEFT")
            {
                m_player->get<CInput>().left = true;
                return;
            }
            else if (actionName == "RIGHT")
            {
                m_player->get<CInput>().right = true;
                return;
            }
            else if (actionName == "SHOOT")
            {
                if (m_player->get<CInput>().canShoot)
                {
                    m_player->get<CState>().currentState = PlayerState::Shooting;      // Change state, and hence animation
                    spawnArrow(m_player);
                    m_player->get<CInput>().canShoot = false;                          // Can't shoot again until SHOOT released
                }
                
                return;
            }
        }

        // Toggle actions
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
        else if (actionName == "TOGGLE_MENU")
        {
            m_drawGui = !m_drawGui;
        }
        // Handle view manipulation actions
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
        if (m_player->has<CInput>())
        {
            if (actionName == "UP")
            {
                m_player->get<CInput>().up = false;
            }
            else if (actionName == "DOWN")
            {
                m_player->get<CInput>().down = false;
            }
            else if (actionName == "LEFT")
            {
                m_player->get<CInput>().left = false;
            }
            else if (actionName == "RIGHT")
            {
                m_player->get<CInput>().right = false;
            }
            else if (actionName == "SHOOT")
            {
                m_player->get<CInput>().canShoot = true;
            }
        }
    }
}

void ScenePlatformer::sMovement()
{
    // For each entity in scene, handle movement according to entity's velocity
    for (auto entity : m_entityManager.getEntities())
    {
        auto& transform = entity->get<CTransform>();

        if (entity == m_player)
        {
            auto& input = m_player->get<CInput>();
            auto& state = m_player->get<CState>();

            // If both are pressed or not pressed, x velocity is 0, else set velocity
            if (input.left == input.right)
            {
                transform.velocity.x = 0;
            }
            else
            {
                transform.velocity.x = input.right ? m_playerConfig.X_SPEED : -m_playerConfig.X_SPEED;
            }

            // If player is holding jump & can jump, set y velocity to jump speed
            if (input.up && input.canJump)
            {
                transform.velocity.y = m_playerConfig.JUMP_SPEED;
                input.canJump = false;
            }
            // If player isn't holding jump, set y velocity to 0
            else if (!input.up && transform.velocity.y > 0)
            {
                transform.velocity.y *= 0.75f;
            }
            
            // Update player state if not shooting
            if (state.currentState != PlayerState::Shooting)
            {
                if (transform.velocity.x != 0)
                {
                    state.currentState = PlayerState::Running;
                }
                else if (transform.velocity.y == 0)
                {
                    state.currentState = PlayerState::Idle;
                }

                if (transform.velocity.y > 0)
                {
                    state.currentState = PlayerState::Jumping;
                }
                else if (transform.velocity.y < 0)
                {
                    state.currentState = PlayerState::Falling;
                }
            }

            // Flip animation in X direction if mismatch between velocity and orientation of animation in X
            if ((transform.velocity.x < 0 && transform.scale.x > 0) ||
                (transform.velocity.x > 0 && transform.scale.x < 0))
            {
                transform.scale.x *= -1;
            }
        }

		// Apply gravity if Entity has CGravity
        if (entity->has<CGravity>())
		{
	        transform.velocity.y -= entity->get<CGravity>().acceleration;
		}

        // Cap velocity in both directions; necessary in y direction to ensure no moving through floor
        transform.velocity.clampMax(m_playerConfig.MAX_SPEED);

        // Update Entity's position according to velocity
        transform.previousPosition = transform.position;
        transform.position += transform.velocity;
    }
}

void ScenePlatformer::sLifespan()
{
    for (auto& entity : m_entityManager.getEntities())
    {
        if (entity->has<CLifespan>())
        {
            const auto& lifespanComponent = entity->get<CLifespan>();
            
            if (m_currentFrame - lifespanComponent.frameCreated > lifespanComponent.framesDuration)
            {
                entity->destroy();
            }
        }
    }
}

void ScenePlatformer::sCollision()
{
    EntityVector& entities = m_entityManager.getEntities();
    size_t entityCount = entities.size();

    for (size_t i = 0; i < entityCount; ++i)
    {
        auto& a = entities[i];

        if (!a->has<CBody>())
        {
            continue;
        }

        for (int j = i + 1; j < entityCount; ++j)
        {
            auto& b = entities[j];

            if (!b->has<CBody>())
            {
                continue;
            }

            // If overlap in both x and y directions, resolve collision accordingly
            const Vec2f& overlap = Physics::getOverlap(a, b);
            if (overlap.x > 0 && overlap.y > 0)
            {
                if (a == m_player || b == m_player)
                {
                    handlePlayerCollision((a == m_player) ? b : a);
                }
                // If a combination of arrow and solid, handle collision according
                else if((a->getTag() == "Arrow" || b->getTag() == "Arrow") && 
                        (a->getTag() == "Solid" || b->getTag() == "Solid"))
                {
                    bool aIsArrow = a->getTag() == "Arrow";
                    handleArrowCollision(aIsArrow ? a : b, aIsArrow ? b : a);
                }
            }
        }
    }

    // Restrict player from moving off left of map
    Vec2f& posPlayer = m_player->get<CTransform>().position;
    posPlayer.x = std::max(m_player->get<CBody>().bBox.size.x / 2.0f, posPlayer.x);

    // Restart level if player has fallen down hole
    if (posPlayer.y < 0)
    {
        loadLevel(m_levelPath);
    }
}

void ScenePlatformer::sAnimation()
{
    for (auto& entity : m_entityManager.getEntities())
    {
        // If entity has Animation component, either update it, or remove if has ended
        if (!entity->has<CAnimation>())
        {
            continue;
        }
        
        auto& animationComponent = entity->get<CAnimation>();
        bool hasEnded = animationComponent.update();

        if (hasEnded)
        {
            endAnimation(entity);
        }

        if (entity == m_player)
        {
            // If player state has changed, change animation 
            const CState& stateComponent = m_player->get<CState>();
            if (stateComponent.currentState != stateComponent.previousState)
            {
                changePlayerAnimation();
            }       
        }
    }
}

void ScenePlatformer::sRender()
{
    sf::RenderWindow& window = m_game->getWindow();

    if (m_paused)
    {
        window.clear(sf::Color(50, 50, 150));          // Dark blue
    }
    else
    {
        window.clear(sf::Color(200, 200, 255));        // Light blue
    }

    // Establishes variables for centring view
    sf::View view = window.getView();
    view.setSize(m_viewSize);
    const Vec2f& playerPosition = m_player->get<CTransform>().position;

    // Centres view on player if further to right than middle of screen
	float newViewCentreX = std::max(m_viewSize.x / 2.0f, playerPosition.x);        // Ensures view doesn't exceed left side of level

    // Calculate view horizontal movement for rendering of parallax background layers
    float viewDeltaX = newViewCentreX - view.getCenter().x;
    sParallax(viewDeltaX);

    // Recentre view
	view.setCenter(newViewCentreX, view.getCenter().y);
    window.setView(view);


    // Drawing entities
    // 
    // Move player to end of entity vector
    EntityVector& entities = m_entityManager.getEntities();

    // Check whether player exists
    auto playerIterator = std::find_if(entities.begin(), entities.end(), [](std::shared_ptr<Entity> e)
    {
        return e->getTag() == "Player"; 
    });

    // If player exists, move to end of entity vector, so only a single pass needed
    if (playerIterator != entities.end())
    {
        std::shared_ptr<Entity> player = *playerIterator;
        entities.erase(playerIterator);
        entities.push_back(player);
    }

    // Drawing of textures/animations and bounding boxes
    for (auto entity : entities)
    {
        renderEntity(entity);
    }

	// Render bounding boxes if enabled; in a separate loop to ensure they are drawn on top of textures
    if (m_drawBoundingBoxes)
    {
        for (auto entity : entities)
        {
            if (entity->has<CBody>())
            {
                renderBBox(entity);
            }
        }
    }
    
    if (m_drawGrid)
    {
		renderGrid();
    }

    if (m_drawGui)
    {
        ImGui::SFML::Render(window);
    }

    window.display();
}

void ScenePlatformer::sGUI()
{
    if (!m_drawGui)
    {
        return;
    }

    sf::RenderWindow& window = m_game->getWindow();
    ImGui::SFML::Update(window, m_clock.restart());

    ImGui::ShowDemoWindow();
    
    ImGui::Begin("Scene Properties");

    if (ImGui::BeginTabBar("Tabs"))
    {
        if (ImGui::BeginTabItem("Enity Inspector"))
        {
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Enity Manager"))
        {
            // 2x collapsing headers
                // All entities (include textured button, ID, tag, animation name, position, button to delete entity)
                // Entities by tag (exclude tag)
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Animations"))
        {
            sf::Vector2f buttonSize(32.0f, 32.0f);
            sf::Color bgColour(0, 0, 0, 0);
            sf::Color tintColour(255, 255, 255, 255);

            ImVec2 padding = ImVec2(2.0f, 2.0f);
            ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, padding);

            const TextureMap& sfmlTextureMap = m_game->getAssets().getTextureMap();
            float buttonWidth = buttonSize.x + (padding.x * 2) + itemSpacing.x;
            int buttonsPerRow = static_cast<int>(ImGui::GetWindowWidth() / buttonWidth);
            int buttonCount = 0;

            for (const auto& texturePair : sfmlTextureMap)
            {
                const sf::Texture& texture = texturePair.second;
                std::string buttonID = "button_" + texturePair.first;

                if (buttonCount > 0 && buttonCount % buttonsPerRow != 0)
                {
                    ImGui::SameLine();
                }
                
                if (ImGui::ImageButton(buttonID.c_str(), texture, buttonSize, bgColour, tintColour))
                {
                    // add entity to entity manager with correct animation at (1,1)
                }

                buttonCount++;
            }
        
            ImGui::PopStyleVar();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Performance"))
        {
            // Toggle for FPS overlay
            // Toggle for frame times / frame time graph
            // Toggle memory usage stats(total memory used, no.entities currently)
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Actions"))
        {
            // Buttons for starting and ending each action
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Debug"))
        {
            static bool placeholder = false;

            ImGui::Checkbox("Draw Grid", &m_drawGrid);
            ImGui::Checkbox("Draw Bounding Boxes", &m_drawBoundingBoxes);
            ImGui::Checkbox("Draw AI Patrol/Home Nodes", &placeholder);
            ImGui::Checkbox("Draw Vision Rays", &placeholder);
            ImGui::Separator();
            ImGui::Checkbox("Free Camera", &placeholder);

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void ScenePlatformer::sParallax(float viewDeltaX)
{
    auto parallaxLayers = m_entityManager.getEntities("Background");
    unsigned int numLayers = parallaxLayers.size();

    // Move each background according to layer number
    for (auto& layer : parallaxLayers)
    {
        float& positionX = layer->get<CTransform>().position.x;
        unsigned int layerIndex = layer->get<CParallax>().layerIndex;

        // Calculate speed according to layer: the further back, the more the layer moves in the virtual world
        float speed = static_cast<float>(numLayers - (layerIndex + 1)) / numLayers;

        positionX += viewDeltaX * speed;
    }
}

void ScenePlatformer::spawnPlayer()
{
    // Add Player entity and add components according to playerConfig struct
    m_player = m_entityManager.addEntity("Player");
    
    m_player->add<CInput>();
    m_player->add<CBody>(Vec2f(m_playerConfig.BB_WIDTH, m_playerConfig.BB_HEIGHT), 1.0f);
    m_player->add<CGravity>(m_playerConfig.GRAVITY);
    
    // Add CState, and use state to determine animation
    const PlayerState& state = m_player->add<CState>().currentState;
    m_player->add<CAnimation>(m_game->getAssets().getAnimation(m_stateToAnimationMap[state]), true);

    // Adding CTransform must follow adding CAnimation because gridToMidPixel uses CAnimation
    m_player->add<CTransform>(gridToMidPixel(Vec2f(m_playerConfig.X, m_playerConfig.Y), m_player));
}

void ScenePlatformer::spawnArrow(std::shared_ptr<Entity> entity)
{
    const float arrowSpeed = 10.0f;
    const int framesAlive = 60;

    // Create entity
    std::shared_ptr<Entity> arrow = m_entityManager.addEntity("Arrow");

    // For CAnimation, CBody
    const auto& animationArrow = m_game->getAssets().getAnimation("Arrow");
    
    // For CTransform
    Vec2f positionEntity = entity->get<CTransform>().position;
    bool isFacingLeft = entity->get<CTransform>().scale.x < 0;
    float widthEntity = entity->get<CAnimation>().animation.getSize().x;
    float widthArrow = animationArrow.getSize().x;
    Vec2f velocity = isFacingLeft ? Vec2f(-arrowSpeed, 0.0f) : Vec2f(arrowSpeed, 0.0f);
    float offsetX = isFacingLeft ? (-widthEntity / 2.0f) - (widthArrow / 2.0f) : (widthEntity / 2.0f) + (widthArrow / 2.0f);

    // Add components with pre-calculated parameters
    arrow->add<CAnimation>(animationArrow, true);
    arrow->add<CBody>(animationArrow.getSize());
    arrow->add<CTransform>(positionEntity + Vec2f(offsetX, 8.0f), velocity, 0.0f);
    arrow->get<CTransform>().scale = isFacingLeft ? Vec2f(-1.0f, 1.0f) : Vec2f(1.0f, 1.0f);
    arrow->add<CLifespan>(framesAlive, m_currentFrame);
}

// Used to position of entity so bottom left of sprite is at bottom left of the cell
Vec2f ScenePlatformer::gridToMidPixel(const Vec2f& gridPosition, std::shared_ptr<Entity> entity)
{
    if (!entity->has<CAnimation>())
    {
        std::cerr << "ScenePlatformer.cpp: entity has no CAnimation." << std::endl;
        return Vec2f(0.0f, 0.0f);
    }

    const Vec2f& spriteSize = entity->get<CAnimation>().animation.getSize();
    Vec2f offset = (gridPosition * (float)m_gridCellSize) + (spriteSize / 2.0f);

    return offset;
}

void ScenePlatformer::renderEntity(std::shared_ptr<Entity> e)
{
    // Draw Entity textures/animations
    if (!m_drawTextures || !e->has<CAnimation>())
    {
        return;
    }

    auto& transform = e->get<CTransform>();
    auto& sprite = e->get<CAnimation>().animation.getSprite();

    // If entity has CBody, align bottom of sprite to bottom of bounding box
    float spriteCentreY = -transform.position.y;
    if (e->has<CBody>())
    {
        spriteCentreY += (e->get<CBody>().bBox.size.y - sprite.getGlobalBounds().height) / 2.0f;
    }

    // If entity is background, render copies either side to ensure screen covered
    if (e->getTag() == "Background")
    {
        float layerWidth = sprite.getGlobalBounds().width;

        // Calculate the number of times the layer needs to be drawn to cover the screen width
        int numRepeats = static_cast<int>(std::ceil(m_viewSize.x / layerWidth)) + 1;

        // Draw the layer multiple times
        for (int i = -1; i < numRepeats; ++i)
        {
            sprite.setPosition(transform.position.x + i * layerWidth, -transform.position.y);
            m_game->getWindow().draw(sprite);
        }
    }
    else
    {
        sprite.setPosition(transform.position.x, spriteCentreY);
        sprite.setScale(transform.scale.x, transform.scale.y);
        sprite.setRotation(transform.angle);
        m_game->getWindow().draw(sprite);
    }   
}

void ScenePlatformer::renderGrid()
{
    sf::RenderWindow& window = m_game->getWindow();
    sf::View view = window.getView();

    float leftEdgeX = view.getCenter().x - (m_viewSize.x / 2);                                      // Left edge of viewable area
    float rightEdgeX = leftEdgeX + m_viewSize.x + m_gridCellSize;                                 // Right edge of viewable area - width of a cell is added to ensure full coverage
    float firstVertLineX = leftEdgeX - (static_cast<int>(leftEdgeX) % m_gridCellSize);            // X position of leftmost viewable cell starting just outside of window
    
    sf::Vector2f viewSize = view.getSize();

    float topEdgeY = view.getCenter().y - (m_viewSize.y / 2.0f);                                     // Top of viewable area
    float bottomEdgeY = view.getCenter().y + (m_viewSize.y / 2.0f) + m_gridCellSize;               // Bottom of viewable area; height of a cell added to ensure full coverage
	float firstHorizontalLineY = topEdgeY + (-static_cast<int>(topEdgeY) % m_gridCellSize);        // Y position of topmost cell starting just outside of window

    sf::VertexArray lines(sf::Lines);

    // Add verticle lines to vertex array
    for (float x = firstVertLineX; x < rightEdgeX; x += m_gridCellSize)
    {
        lines.append(sf::Vertex(sf::Vector2f(x, bottomEdgeY)));
        lines.append(sf::Vertex(sf::Vector2f(x, topEdgeY)));
    }

    // Draw horizontal lines
    for (float y = firstHorizontalLineY; y < bottomEdgeY; y += m_gridCellSize)
    {
        lines.append(sf::Vertex(sf::Vector2f(leftEdgeX, y)));
        lines.append(sf::Vertex(sf::Vector2f(rightEdgeX, y)));

        // Draw coordinate text for each cell in row
        for (float x = firstVertLineX; x < rightEdgeX; x += m_gridCellSize)
        {
            int gridX = static_cast<int>(x) / m_gridCellSize;
            int gridY = -static_cast<int>(y) / m_gridCellSize;

            m_gridText.setString("(" + std::to_string(gridX) + ", " + std::to_string(gridY) + ")");
            m_gridText.setPosition(x, y - m_gridCellSize);
            window.draw(m_gridText);
        }
    }

    window.draw(lines);
}

void ScenePlatformer::renderBBox(std::shared_ptr<Entity> entity)
{
    auto& bBox = entity->get<CBody>().bBox;
    auto& transform = entity->get<CTransform>();

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(bBox.size.x - 1, bBox.size.y - 1));  // Only takes sf::Vector2f
    rectangle.setOrigin(bBox.size.x / 2.0f, bBox.size.y / 2.0f);
    rectangle.setPosition(transform.position.x, -transform.position.y);
    rectangle.setFillColor(sf::Color(0, 0, 0, 0));                  // Sets alpha of fill colour to 0 i.e. transparent
    rectangle.setOutlineColor(sf::Color(255, 255, 255, 255));       // Sets alpha of outline to 255 i.e. opaque
    rectangle.setOutlineThickness(1);
    m_game->getWindow().draw(rectangle);
}

void ScenePlatformer::endAnimation(std::shared_ptr<Entity> entity)
{
    if (entity == m_player)
    {
        m_player->get<CState>().currentState = PlayerState::Idle;
    }
    else
    {
        entity->remove<CAnimation>();
    }
}

void ScenePlatformer::changePlayerAnimation()
{
    CState& stateComponent = m_player->get<CState>();

    const std::string& animationName = m_stateToAnimationMap[stateComponent.currentState];
    bool toRepeat = stateComponent.currentState != PlayerState::Shooting;
    m_player->add<CAnimation>(m_game->getAssets().getAnimation(animationName), toRepeat);

    stateComponent.previousState = stateComponent.currentState;
}

void ScenePlatformer::spawnTempAnimation(Vec2f position, std::string animationName)
{
    const Animation& animation = m_game->getAssets().getAnimation(animationName);
                    
    // Create coin entity in grid cell above block
    std::shared_ptr<Entity> animationEntity = m_entityManager.addEntity(animationName);
    animationEntity->add<CAnimation>(m_game->getAssets().getAnimation(animationName), true);
    animationEntity->add<CTransform>(position);
    animationEntity->add<CLifespan>(animation.getFrameCount() * animation.getFrameDuration(), m_currentFrame);
}

void ScenePlatformer::handlePlayerCollision(std::shared_ptr<Entity> object)
{
    // Resolve collision and determine if X or Y direction
    bool isXDirection = Physics::resolveCollision(m_player, object);

    // If collision in y direction, set velocity to 0 and handle according to direction
    if (!isXDirection)
    {
        // Whether collision from above or below, set y velocity to 0
        m_player->get<CTransform>().velocity.y = 0.0f;

        // If player's previous position was less (i.e. closer to top of screen), collision from above
        bool isFromAbove = m_player->get<CTransform>().previousPosition.y > object->get<CTransform>().previousPosition.y;

        // If collision from above, player can now jump
        if (isFromAbove)
        {
            m_player->get<CInput>().canJump = true;
        }
        // If from below, destroy block/change animation accordingly
        else
        {
            destroySolid(object);
        }
    }
}

void ScenePlatformer::handleArrowCollision(std::shared_ptr<Entity> arrow, std::shared_ptr<Entity> object)
{
    destroySolid(object);
    arrow->destroy();
}

void ScenePlatformer::destroySolid(std::shared_ptr<Entity> solid)
{
    std::string animationName = solid->get<CAnimation>().animation.getName();

    // If brick, destroy and spawn explosion animation
    if (animationName == "Brick")
    {
        solid->destroy();
        spawnTempAnimation(solid->get<CTransform>().position, "Explosion");
    }
    // If question mark, change animation and spawn coin
    else if (animationName == "QMark")
    {
        solid->add<CAnimation>(m_game->getAssets().getAnimation("QMarkDead"), true);
        spawnTempAnimation(solid->get<CTransform>().position + Vec2f(0.0f, m_gridCellSize), "Coin");
    }
}