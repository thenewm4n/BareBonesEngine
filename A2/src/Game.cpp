#define _USE_MATH_DEFINES

#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>

#include "Game.h"
#include "Components.h"
//#include "imgui_demo.cpp"

Game::Game(const std::string& configFile)
    : m_resolution(1920, 1080)
{
    init(configFile);
}

void Game::init(const std::string& configFile)
{
    // Open file
    std::ifstream file(configFile);
    if (!file.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        exit(-1);
    }
    
    // Read config.txt
    std::string line;
    std::string firstElement;

    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        
        if (lineStream >> firstElement)
        {
            if (firstElement == "Window")
            {
                lineStream >> m_resolution.x >> m_resolution.y;
            }
            else if (firstElement == "Font")
            {
                std::string fontPath;
                int textSize;
                int textColour[3] = { 0, 0, 0 };

                lineStream >> fontPath >> textSize >> textColour[0] >> textColour[1] >> textColour[2];

                if (!m_font.loadFromFile(fontPath))
                {
                    std::cerr << "Could not load font." << std::endl;
                    exit(-2);
                }

                m_text.setFont(m_font);
                m_text.setCharacterSize(textSize);
                m_text.setFillColor(sf::Color(textColour[0], textColour[1], textColour[2]));
            }
            else if (firstElement == "Player")
            {
                // Output to PlayerConfig struct
                lineStream >> m_playerConfig.shapeRadius >> m_playerConfig.collisionRadius >> m_playerConfig.speed >> m_playerConfig.fillR >>
                    m_playerConfig.fillG >> m_playerConfig.fillB >> m_playerConfig.outlineR >> m_playerConfig.outlineG >>
                    m_playerConfig.outlineB >> m_playerConfig.outlineThick >> m_playerConfig.vertices;
            }
            else if (firstElement == "Enemy")
            {
                // Output to EnemyConfig struct
                lineStream >> m_enemyConfig.shapeRadius >> m_enemyConfig.collisionRadius >> m_enemyConfig.speedMin >> 
                    m_enemyConfig.speedMax >> m_enemyConfig.outlineR >> m_enemyConfig.outlineG >> m_enemyConfig.outlineB >> 
                    m_enemyConfig.outlineThick >> m_enemyConfig.verticesMin >> m_enemyConfig.verticesMax >> 
                    m_enemyConfig.smallLifespan >> m_enemyConfig.spawnInterval;
            }
            else    // i.e. if Bullet
            {
                lineStream >> m_bulletConfig.shapeRadius >> m_bulletConfig.collisionRadius >> m_bulletConfig.speed >> m_bulletConfig.fillR >>
                    m_bulletConfig.fillG >> m_bulletConfig.fillB >> m_bulletConfig.outlineR >> m_bulletConfig.outlineG >>
                    m_bulletConfig.outlineB >> m_bulletConfig.outlineThick >> m_bulletConfig.vertices >> m_bulletConfig.lifespan;
            }
        }
    }

    file.close();

    m_window.create(sf::VideoMode(static_cast<unsigned int>(m_resolution.x), static_cast<unsigned int>(m_resolution.y)), "Assignment 2");
    m_window.setFramerateLimit(60);

    ImGui::SFML::Init(m_window);
    ImGui::GetStyle().ScaleAllSizes(3.5f);		// Scales imgui GUI
    ImGui::GetIO().FontGlobalScale = 1.3f;		// Scale imgui text size

    spawnPlayer();
}

void Game::run()
{
    while(m_isRunning)
    {
        m_entities.update();

        // Call each systems
        // THIS MAY BE WRONG
        sCollision();       // sCollision must come before sMovement for collision with walls
        sUserInput();
        if (!m_isPaused)
        {
            sEnemySpawner();
            sMovement();
            sLifespan();

            // Spawn bullet if user left-clicks
            if (m_player->cInput->shoot)
            {
                Vec2 mousePosition = sf::Mouse::getPosition(m_window);
                spawnBullet(m_player, mousePosition);
                m_player->cInput->shoot = false;
            }
        }
        sGUI();
        sRender();

        // increment the current frame
        // may need to be moved when pause implemented (?)
        m_currentFrame++;
    }
}


// Systems

void Game::sMovement()
{
    // Update Y velocity according to player input
    if (m_player->cInput->down)
    {
        m_player->cTransform->velocity.y = m_playerConfig.speed;
    }
    else if (m_player->cInput->up)
    {
        m_player->cTransform->velocity.y = -1 * m_playerConfig.speed;
    }
    else
    {
        m_player->cTransform->velocity.y = 0;
    }

    // Update X velocity according to player input
    if (m_player->cInput->right)
    {
        m_player->cTransform->velocity.x = m_playerConfig.speed;
    }
    else if (m_player->cInput->left)
    {
        m_player->cTransform->velocity.x = -1 * m_playerConfig.speed;
    }
    else
    {
        m_player->cTransform->velocity.x = 0;
    }

    // Move all entities with a Transform component according to their velocity
    for (const auto& entity : m_entities.getEntities())
    {
        if (entity->cTransform)
        {
            entity->cTransform->position += entity->cTransform->velocity;
        }
    }
}

void Game::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(m_window, event);

        if (event.type == sf::Event::Closed)
            m_isRunning = false;

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::Escape:
                    m_isRunning = false;
                    break;
                case sf::Keyboard::P:
                    if (m_isPaused)
                        m_isPaused = false;
                    else
                        m_isPaused = true;
                    break;
                case sf::Keyboard::W:
                    m_player->cInput->up = true;
                    break;
                case sf::Keyboard::S:
                    m_player->cInput->down = true;
                    break;
                case sf::Keyboard::A:
                    m_player->cInput->left = true;
                    break;
                case sf::Keyboard::D:
                    m_player->cInput->right = true;
                    break;
                default:
                    break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                m_player->cInput->up = false;
                break;
            case sf::Keyboard::S:
                m_player->cInput->down = false;
                break;
            case sf::Keyboard::A:
                m_player->cInput->left = false;
                break;
            case sf::Keyboard::D:
                m_player->cInput->right = false;
                break;
            default:
                break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            // If ImGui is being clicked, ignores the mouse event
            if (ImGui::GetIO().WantCaptureMouse) { continue; }

            if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_player->cInput->shoot = true;
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "Right mouse button clicked at (" << event.mouseButton.x << ", " << event.mouseButton.y << ")" << std::endl;
                // call spawnSpecialWeapon() here
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            // If ImGui is being clicked, ignores the mouse event
            if (ImGui::GetIO().WantCaptureMouse) { continue; }

            if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_player->cInput->shoot = false;
            }
        }
    }
}

void Game::sRender()
{
    m_window.clear();

    // Rotate player and all enemies
    EntityVector rotatingEntities = m_entities.getEntities("Enemy");
    rotatingEntities.push_back(m_player);

    for (const auto& entity : rotatingEntities)
    {
        entity->cTransform->angle += 1.f;
        if (entity->cTransform->angle >= 360.f)
        {
            entity->cTransform->angle -= 360.f;
        }
        entity->cShape->shape.setRotation(entity->cTransform->angle);
    }

    // THIS MAY BE WRONG; MAYBE SPLIT SETTING POSITION AND DRAWING
    for (const auto& entity : m_entities.getEntities())
    {
        // If entity has transform component, update entity position
        if (entity->cTransform)
        {
            entity->cShape->shape.setPosition(entity->cTransform->position.x, entity->cTransform->position.y);
        }

        // If entity has shape component, draw it to window
        if (entity->cShape)
        {
            m_window.draw(entity->cShape->shape);
        }
    }

    // Draw the UI
    ImGui::SFML::Render(m_window);

    // Draw the score text
    m_text.setString("Score: " + std::to_string(m_score));
    m_window.draw(m_text);

    m_window.display();
}

void Game::sCollision()
{
     // If entity has Collision component and touches wall, velocity is reversed
    for (const auto& entity : m_entities.getEntities())
    {   
        if (entity->cCollision)
        {
            sf::FloatRect shapeBounds = entity->cShape->shape.getGlobalBounds();

            // If bounds exceed window edge, reverse corresponding velocity
            if (shapeBounds.left < 0 || shapeBounds.left + shapeBounds.width > m_resolution.x)
                entity->cTransform->velocity.x *= -1.f;

            if (shapeBounds.top < 0 || shapeBounds.top + shapeBounds.height > m_resolution.y)
                entity->cTransform->velocity.y *= -1.f;
        }
    }

    EntityVector enemies = m_entities.getEntities("Enemy");
    EntityVector smallEnemies = m_entities.getEntities("SmallEnemy");
    enemies.insert(enemies.end(), smallEnemies.begin(), smallEnemies.end());

    // Checks all enemies for collision with bullets or player
    for (const auto& enemy : enemies)
    {
        // Check collision between bullets and enemy
        for (const auto& bullet : m_entities.getEntities("Bullet"))
        {
            if (isCollision(bullet, enemy))
            {
                // If big enemy destroyed, spawn small enemies
                if (enemy->getTag() == "Enemy")
                {
                    spawnSmallEnemies(enemy);
                }   

                m_score += enemy->cScore->score;
                bullet->destroy();
                enemy->destroy();
            }
        }

        // If player collides with big or small enemy, destroy enemy, set score to 0 and reset player position to centre
        if (isCollision(m_player, enemy))
        {   
            // If collision with big enemy, spawn small enemies
            if (enemy->getTag() == "Enemy")
            {
                spawnSmallEnemies(enemy);
            }
            enemy->destroy();
            m_score = 0;
            m_player->cTransform->position = m_resolution / 2;
        }
    }
}

void Game::sLifespan()
{
    EntityVector entities = m_entities.getEntities();
    for (const auto& entity : entities)
    {
        // If entity has Lifespan component
        if (entity->cLifespan)
        {
            // If has lives remaining and hasn't been destroyed elsewhere in code this loop
            if (entity->cLifespan->remaining > 0)
            {
                entity->cLifespan->remaining--;
                
                // Set transparency to ratio of remaining life to total life
                sf::Uint8 newAlpha = static_cast<sf::Uint8>((static_cast<float>(entity->cLifespan->remaining) / static_cast<float>(entity->cLifespan->total)) * 255);
                
                // Get original colours
                sf::Color fillColour = entity->cShape->shape.getFillColor();
                sf::Color outlineColour = entity->cShape->shape.getOutlineColor();
                
                // Change alpha values
                fillColour.a = newAlpha;
                outlineColour.a = newAlpha;

                // Set new colours with new alphas
                entity->cShape->shape.setFillColor(fillColour);
                entity->cShape->shape.setOutlineColor(outlineColour);
            }
            // If entity has no lives left but is still alive, destroy it
            else if (entity->isAlive())
            {   
                entity->destroy();
            }
        }
    }
}

void Game::sEnemySpawner()
{
    if (m_currentFrame - m_lastEnemySpawnFrame > m_enemyConfig.spawnInterval)
    {
        spawnEnemy();
    }
}

void Game::sGUI()
{   
    ImGui::SFML::Update(m_window, m_deltaClock.restart());

    ImGui::ShowDemoWindow();

    ImGui::Begin("Shape Shooter");

    // Tabs
        // Systems
        // Entity Manager
            // Collapsing headers - all entities; entities by tag
 
    if (ImGui::BeginTabBar("MyTabBar"))
    {
        if (ImGui::BeginTabItem("Systems"))
        {
            // Change the bool refs
            ImGui::Checkbox("Rendering", &check);
            ImGui::Checkbox("Movement", &check);
            ImGui::Checkbox("Enemy Spawner", &check);
            ImGui::Checkbox("User Input", &check);
            ImGui::Checkbox("Collisions", &check);
            ImGui::Checkbox("Lifespan", &check);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Entity Manager"))
        {
            ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}


// Non-System functions

// Spawns the player entity in middle of screen
void Game::spawnPlayer()
{
    // Add player Entity
    std::shared_ptr<Entity> entity = m_entities.addEntity("Player");

    // Update player entity components using data from config file
    entity->cTransform = std::make_shared<CTransform>(Vec2(200.f, 200.f), Vec2(m_playerConfig.speed, m_playerConfig.speed), 0.f);
    
    sf::Color fillColour(m_playerConfig.fillR, m_playerConfig.fillG, m_playerConfig.fillB);
    sf::Color outlineColour(m_playerConfig.outlineR, m_playerConfig.outlineG, m_playerConfig.outlineB);
    entity->cShape = std::make_shared<CShape>(m_playerConfig.shapeRadius, m_playerConfig.vertices, fillColour, outlineColour, m_playerConfig.outlineThick);
    
    entity->cInput = std::make_shared<CInput>();

    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.collisionRadius);

    // Store player entity as attribute of Game object, since it is modified often
    m_player = entity;
}

// Spawn enemy at random position; trigger intermittently automatially
void Game::spawnEnemy()
{
    // Create entity and add to entity manager (like in spawnPlayer())
    std::shared_ptr<Entity> enemyEntity = m_entities.addEntity("Enemy");
    
    // Engine for all RNG
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

    // Generate random position
    std::uniform_int_distribution<int> xDistribution(static_cast<int>(m_enemyConfig.shapeRadius), static_cast<int>(m_resolution.x - m_enemyConfig.shapeRadius));
    std::uniform_int_distribution<int> yDistribution(static_cast<int>(m_enemyConfig.shapeRadius), static_cast<int>(m_resolution.y - m_enemyConfig.shapeRadius));
    Vec2 position(static_cast<float>(xDistribution(generator)), static_cast<float>(yDistribution(generator)));

    // Generate random speed
    std::uniform_real_distribution<float> speedDistribution(m_enemyConfig.speedMin, m_enemyConfig.speedMax);
    float speed = speedDistribution(generator);

    // Generate random angle
    std::uniform_real_distribution<float> angleDistribution(0, 360);
    float angle = angleDistribution(generator);

    Vec2 velocity(speed * cosf(angle), speed * sinf(angle));

    // Instantiate Transform component with randomly generated position, velocity, angle
    enemyEntity->cTransform = std::make_shared<CTransform>(position, velocity, 0);

    // Generate random fill colour
    std::uniform_int_distribution<int> colourDistribution(0, 255);
    sf::Color fillColour(colourDistribution(generator), colourDistribution(generator), colourDistribution(generator));
    sf::Color outlineColour(m_enemyConfig.outlineR, m_enemyConfig.outlineG, m_enemyConfig.outlineB);

    // Generate random no. vertices
    std::uniform_int_distribution<int> verticesDistribution(3, 8);
    int numVertices = verticesDistribution(generator);

    // Instantiation Shape sprite component with radius, no. vertices, colours, and outline thickness
    enemyEntity->cShape = std::make_shared<CShape>(m_enemyConfig.shapeRadius, numVertices, fillColour, outlineColour, m_enemyConfig.outlineThick);

    enemyEntity->cCollision = std::make_shared<CCollision>(m_enemyConfig.collisionRadius);

    enemyEntity->cScore = std::make_shared<CScore>(10);

    m_lastEnemySpawnFrame = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> bigEnemy)
{
    // Error handling
    if (!bigEnemy->cTransform || !bigEnemy->cShape || !bigEnemy->cCollision || !bigEnemy->cScore)
    {
        std::cerr << "Input entity is missing a component." << std::endl;
        exit(-3);
    }
    
    // Calculate angle between small enemies
    size_t numVertices = bigEnemy->cShape->shape.getPointCount();
    float angle = 2 * M_PI / static_cast<float>(numVertices);

     // Set to same colours as original
    sf::Color fillColour = bigEnemy->cShape->shape.getFillColor();
    sf::Color outlineColour = bigEnemy->cShape->shape.getOutlineColor();
    float outlineThickness = bigEnemy->cShape->shape.getOutlineThickness();

    // Set Shape and Collision components to half the scale 
    float shapeRadius = bigEnemy->cShape->shape.getRadius() / 2;
    float collisionRadius = bigEnemy->cCollision->radius / 2;

    // Set position and speed of transform component Transform component
    for (int i = 0; i < numVertices; i++)
    {
        std::shared_ptr<Entity> smallEnemy = m_entities.addEntity("SmallEnemy");

        // Set velocity at angle of i * angle
            // Change X and Y components of velocity components to reflect angle
        float xSquared = bigEnemy->cTransform->velocity.x * bigEnemy->cTransform->velocity.x;
        float ySquared = bigEnemy->cTransform->velocity.y * bigEnemy->cTransform->velocity.y;
        float speed = sqrtf(xSquared + ySquared);
        Vec2 velocity(cosf(angle * i) * speed, sinf(angle * i) * speed);

        // Instantiate components
        smallEnemy->cTransform = std::make_shared<CTransform>(bigEnemy->cTransform->position, velocity, 0);
        smallEnemy->cShape = std::make_shared<CShape>(shapeRadius, numVertices, fillColour, outlineColour, outlineThickness);
        smallEnemy->cCollision = std::make_shared<CCollision>(collisionRadius);
        smallEnemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.smallLifespan);
        smallEnemy->cScore = std::make_shared<CScore>(bigEnemy->cScore->score * 2);
    }
}

// Spawn a bullet at the position of entity, firing at target
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
    std::shared_ptr<Entity> bulletEntity = m_entities.addEntity("Bullet");

    // Set velocity according to mouse position and speed from config file
    Vec2 diff = target - entity->cTransform->position;
    diff.normalise();
    Vec2 velocity = diff * m_bulletConfig.speed;

    // Set the bullet position ensuring player and bullet Collision component radii don't overlap
    Vec2 bulletPosition;
    if (entity->cCollision)
    {
        // Set position of bullet in direction of target
        bulletPosition = entity->cTransform->position + (entity->cCollision->radius + m_bulletConfig.collisionRadius) * diff;
    }
    else
    {
        bulletPosition = entity->cTransform->position;
    } 
    
    // Instantiate Transform component
    bulletEntity->cTransform = std::make_shared<CTransform>(bulletPosition, velocity, 0);     // What should the angle be set to?

    // Instantiate Shape component
    sf::Color fillColour(m_bulletConfig.fillR, m_bulletConfig.fillG, m_bulletConfig.fillB);
    sf::Color outlineColour(m_bulletConfig.outlineR, m_bulletConfig.outlineG, m_bulletConfig.outlineB);
    bulletEntity->cShape = std::make_shared<CShape>(m_bulletConfig.shapeRadius, m_bulletConfig.vertices, fillColour, outlineColour, m_bulletConfig.outlineThick);
    
    // Setting the position of the shape before the sRender call because the shape is spawned at origin -> collision problem
    bulletEntity->cShape->shape.setPosition(sf::Vector2f(bulletPosition.x, bulletPosition.y));

    // Instantiate Collision component
    bulletEntity->cCollision = std::make_shared<CCollision>(m_bulletConfig.collisionRadius);
    
    // Instantiate Lifespan component
    bulletEntity->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.lifespan);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    //TODO
}

// Assumes that both input 
bool Game::isCollision(const std::shared_ptr<Entity>& entity1, const std::shared_ptr<Entity>& entity2)
{
    if (!entity1->cCollision || !entity2->cCollision)
    {
        std::cerr << "One of the input entities has no Collision component." << std::endl;
        exit(-4);
    }

    Vec2 diff = entity1->cTransform->position - entity2->cTransform->position;
    float sumOfRadii = entity1->cCollision->radius + entity2->cCollision->radius;

    // If distance^2 < (r1+r2)^2
    if (diff.x * diff.x + diff.y * diff.y < sumOfRadii * sumOfRadii)
        return true;
    else
        return false;
}