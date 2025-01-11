#include "scenes/SceneLevelEditor.h"

#include "core/Assets.h"
#include "core/Components.h"
#include "core/GameEngine.h"
#include "scenes/SceneStartMenu.h"

#include <ui/imgui/imgui.h>
#include <ui/imgui/imgui-SFML.h>
#include <SFML/OpenGL.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>


SceneLevelEditor::SceneLevelEditor(GameEngine* game, const std::string& levelPath)
    : Scene(game), m_levelPath(levelPath)
{
    init();
}

void SceneLevelEditor::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURES");
    registerAction(sf::Keyboard::B, "TOGGLE_BBOXES");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    registerAction(sf::Keyboard::Tilde, "TOGGLE_MENU");
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

void SceneLevelEditor::loadLevel(const std::string& filename)
{
    // Overwrites previous EntityManager and parallax layers vector
    m_entityManager = EntityManager();

    std::ifstream levelFile(filename);
    if (!levelFile.is_open())
    {
        std::cerr << "SceneLevelEditor.cpp, Line 60: Error opening level file." << std::endl;
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

void SceneLevelEditor::update()
{
    m_entityManager.update();    // Adds and removes entities while avoiding iterator invalidation
    sGUI();
    sRender();

    m_currentFrame++;
}

void SceneLevelEditor::endScene()
{
    m_game->changeScene("MENU", std::make_shared<SceneStartMenu>(m_game));
}

void SceneLevelEditor::sPerformAction(const Action& action)
{
    const std::string& actionName = action.getName();

    if (action.getType() == "START")
    {
        // Check for pausing or quitting
        if (actionName == "QUIT")
        {
            endScene();
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
}

void SceneLevelEditor::sRender()
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

    // Move player to end of entity vector, then draw entities
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
    for (auto entity : entities)
    {
        if (m_drawBoundingBoxes && entity->has<CBody>())
        {
            renderBBox(entity);
        }
    }

    // Draw grid for debugging
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

void SceneLevelEditor::sGUI()
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

void SceneLevelEditor::sParallax(float viewDeltaX)
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

// Wipe the current level txt and write to it all entities in entity manager
void SceneLevelEditor::saveToFile()
{
    // Create a backup of the current save file
    std::string backupPath = m_levelPath + ".bak";
    std::error_code ec;
    std::filesystem::copy_file(m_levelPath, backupPath, std::filesystem::copy_options::overwrite_existing, ec);
    
    // If creating a backup failed, return early
    if (ec)
    {
        std::cerr << "SceneLevelEditor::saveToFile() - Error creating backup: " << ec.message() << std::endl;
        return;
    }

    // Open file at m_levelPath
    bool saveFailed = false;

    std::ofstream levelFile(m_levelPath, std::ios::trunc);
    if (!levelFile.is_open())
    {
        saveFailed = true;
    }

    // Write entities to level file
    for (const auto& entity : m_entityManager.getEntities())
    {
        std::string tag = entity->getTag();
        std::string animationName = entity->get<CAnimation>().animation.getName();

        Vec2f position = entity->get<CTransform>().position;
        float flooredGridPositionX = (position.x - std::fmod(position.x, m_gridCellSize)) / m_gridCellSize;
        float flooredGridPositionY = (position.y - std::fmod(position.y, m_gridCellSize)) / m_gridCellSize;

        levelFile << tag << " " << animationName << " " << flooredGridPositionX << " " << flooredGridPositionY << "\n";       //    wait... should we just make level file positions not grid based? There's less point now?
    }

    // Checks whether IO operations succeeded
    if (levelFile.fail())
    {
        saveFailed = true;
    }

    // If opening level file or IO operations failed, revert to backup
    if (saveFailed)
    {
        std::cerr << "SceneLevelEditor::save() - Error saving." << std::endl;

        // Load backup
        std::filesystem::copy_file(backupPath, m_levelPath, std::filesystem::copy_options::overwrite_existing, ec);

        if (ec)
        {
            std::cerr << "Critical error: Unable to restore from backup. Exiting program." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    levelFile.close();

    // Remove the backup file, regardless of whether saving succeed
    std::filesystem::remove(backupPath, ec);
    if (ec)
    {
        std::cerr << "SceneLevelEditor::saveToFile() - Error deleting backup: " << ec.message() << std::endl;
    }
}

void SceneLevelEditor::spawnPlayer()
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

// Used to position of entity so bottom left of sprite is at bottom left of the cell
Vec2f SceneLevelEditor::gridToMidPixel(const Vec2f& gridPosition, std::shared_ptr<Entity> entity)
{
    if (!entity->has<CAnimation>())
    {
        std::cerr << "SceneLevelEditor.cpp: entity has no CAnimation." << std::endl;
        return Vec2f(0.0f, 0.0f);
    }

    const Vec2f& spriteSize = entity->get<CAnimation>().animation.getSize();
    Vec2f offset = (gridPosition * (float)m_gridCellSize) + (spriteSize / 2.0f);

    return offset;
}

void SceneLevelEditor::renderEntity(std::shared_ptr<Entity> e)
{
    // Draw Entity textures/animations
    if (m_drawTextures && e->has<CAnimation>())
    {
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
}

void SceneLevelEditor::renderGrid()
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

void SceneLevelEditor::renderBBox(std::shared_ptr<Entity> entity)
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