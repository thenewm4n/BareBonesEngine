#include <random>
#include <iostream>

#include "Game.h"

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

                if (!font.loadFromFile(fontPath))
                {
                    std::cerr << "Could not load font." << std::endl;
                    exit(-2);
                }

                m_text.setFont(fontPath);
                m_text.setCharacterSize(textSize);
                m_text.setFillColor(sf::Color(textColour[0], textColour[1], textColour[2]));
            }
            else if (firstElement == "Player")
            {
                // Output to PlayerConfig struct
                lineStream >> m_playerConfig.shapeRadius >> m_playerConfig.collisionRadius >> m_playerConfig.fillR >>
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

    m_window.create(sf::VideoMode(m_resolution.x, m_resolution.y), "Assignment 2");
    m_window.setFramerateLimit(60);

    ImGui::SFML::Init(m_window);

    spawnPlayer();
}

void Game::run()
{
    // TODO: add pause functionality here
    //  rendering system should still function; shapes should still rotate
    //  movement and input systems should stop

    while(m_isRunning)
    {
        m_entities.update();

        ImGui::SFML::Update(m_window, m_deltaClock.restart());

        // Call each systems
        // THIS MAY BE WRONG
        if (!m_isPaused)
        {
            sEnemySpawner();
            sMovement();
            sLifespan();
        }
        sCollision();
        sUserInput();
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
            entity->cTransform.position += entity->cTransform.velocity;
        }
    }
}

void Game::sUserInput()
{
    //TODO: note that you should only be setting the player's input component variables here
    // you should not implement the player's movement logic here
    // the movement system will read the variables you set in this function

    sf::Event;
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
                spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }

            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "Right mouse button clicked at (" << event.MouseButton.x << ", " << event.mouseButton.y << ")" << std::endl;
                // call spawnSpecialWeapon() here
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
        entity->cTransform.angle += 1.f;
        if (entity->cTransform.angle >= 360.f)
        {
            entity->cTransform.angle -= 360.f;
        }
        entity->cShape->shape.setRotation(entity->cTransform.angle);
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
            m_window.draw(entity->cShape.shape);
        }
    }

    // Draw the UI
    ImGui::SFML::Render(m_window);

    // Draw the score text
    m_window.draw(m_text);

    m_window.display()
}

void Game::sCollision()
{
    // TODO: implement all proper collisions between entities

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

    // Collisions between bullet and enemies
    for (const auto& bullet : m_entities.getEntities("Bullet"))
    {
        for (const auto& enemy : m_entities.getEntities("Enemy"))
        {
            /*
            // if distance < bullet->cShape->radius + enement->cShape->radius
                // destoy bullet, destroy enemy, spawn small enemies, update score
            Vec2 diff = bullet->cTransform->position - enemy->cTransform->position;
            float sumOfRadii = bullet->cCollision->radius + enemy->cCollision->radius;
             */

            if (isCollision(bullet, enemy))
            {
                m_score += enemy->cScore->score;
                bullet->destroy();
                enemy->destroy();
                spawnSmallEnemies();
            }
        }

        for (const auto& smallEnemy : m_entities.getEntities("SmallEnemy"))
        {
            if (isCollision(bullet, smallEnemy))
            {
                m_score += smallEnemy->cScore->score;
                bullet.destroy();
                smallEnemy.destroy();
            }
        }
    }

    // If player collides with big or small enemy, destroy enemy, set score to 0 and reset player position to centre
    EntityVector enemies = m_entities.getEntities("Enemy");
    enemies.push_back(m_entities.getEntities("SmallEnemy");
    for (const auto& enemy : enemies)
    {
        if (isCollision(m_player, enemy))
        {
            enemy.destroy();
            spawnSmallEnemies();
            m_score = 0;
            m_player->cTransform->position = m_resolution / 2;
        }
    }
}

void Game::sLifespan()
{
    // TODO

    // for all entities
    // - if entity has no lifespan component, skip
    // - if entity has > 0 remaining lifespan, decrement
    // - if it has lifespan and is alive: scale alpha channel properly
    // - if it has lifespand and time is up: destroy the entity
}

void Game::sEnemySpawner()
{
    // TODO: code implementing enemy spawning
    // if no. frames since last enemy spawned > spawnInterval: spawnEnemy()
    // - i.e. m_currentFrame - m_lastEnemySpawnFrame
}

void Game::sGUI()
{
    ImGui::Begin("Assignment 2");

    ImGui::Text("Stuff goes here.");

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
    std::default_random_engine generator;

    // Generate random position
    std::uniform_int_distribution<int> xDistribution(enemyEntity->m_enemyConfig.shapeRadius, m_resolution.x - enemyEntity->m_enemyConfig.shapeRadius);
    std::uniform_int_distribution<int> yDistribution(enemyEntity->m_enemyConfig.shapeRadius, m_resolution.y - enemyEntity->m_enemyConfig.shapeRadius);
    Vec2 position(xDistribution(generator), yDistribution(generator);

    // Generate random speed
    std::uniform_real_distribution<float> velocityDistribution(m_enemyConfig.speedMin, m_enemyConfig.speedMax);
    Vec2 velocity(velocityDistribution(generator), velocityDistribution(generator));

    // Generate random angle
    std::uniform_real_distribution<float> angleDistribution(0, 360);
    float angle = angleDistribution(generator);

    // Instantiate Transform component with randomly generated position, velocity, angle
    enemyEntity->cTransform = std::make_shared<CTransform>(position, velocity, angle);

    // Generate random fill colour
    std::uniform_int_distribution<int> colourDistribution(0, 255);
    sf::Color fillColour(colourDistribution(generator), colourDistribution(generator), colourDistribution(generator));
    sf::Color outlineColour(m_enemyConfig.outlineR, m_enemyConfig.outlineG, m_enemyConfig.outlineB);

    // Generate random no. vertices
    std::uniform_int_distribution<int> verticesDistribution(3, 8);
    int numVertices = verticesDistrbution(generator);

    // Instantiation Shape sprite component with radius, no. vertices, colours, and outline thickness
    enemyEntity->cShape = std::make_shared<CShape>(m_enemyConfig.shapeRadius, numVertices, fillColour, outlineColour, m_enemyConfig.outlineThick);

    enemyEntity->cCollision = std::make_shared<CCollision>(m_enemyConfig.collisionRadius);

    enemyEntity->cScore = std::make_shared<CScore>(10);

    m_lastEnemySpawnFrame = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
    // TODO: spawn small enemies at location of input enemy e

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to vertices of original enemy
    // - set each small enemy to same colours as original, half the size, set lifespan
    // - small enemies are worth double points of original enemy
}

// Spawn a bullet at the position of entity, firing at target
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
    std::shared_ptr<Entity> bulletEntity = m_entities.addEntity("Bullet");

    // Set velocity according to mouse position and speed from config file
    Vec2 diff = target - entity->cTransform->position;
    Vec2 normDiff = diff.normalise();
    Vec2 velocity = normDiff * m_bulletConfig.speed;

    // Set the bullet position ensuring player and bullet Collision component radii don't overlap
    Vec2 bulletPosition;
    if (entity->cCollision)
    {
        // Set position of bullet in direction of target
        bulletPosition = entity->cTransform->position + (entity->cCollision->radius + m_bulletConfig.collisionRadius) * normDiff;
    }
    else
    {
        bulletPosition = entity->cTransform->position;
    } 
    
    // Instantiate Transform component
    bulletEntity->cTransform = make_shared<CTransform>(bulletPosition, velocity, 0);     // What should the angle be set to?

    // Instantiate Shape component
    sf::Color fillColour(m_bulletConfig.fillR, m_bulletConfig.fillG, m_bulletConfig.fillB);
    sf::Color outlineColour(m_bulletConfig.outlineR, m_bulletConfig.outlineG, m_bulletConfig.outlineB);
    bulletEntity->cShape = make_shared<CShape>(m_bulletConfig.shapeRadius, m_bulletConfig.vertices, fillColour, outlineColour, m_bulletConfig.outlineThick);

    // Instantiate Collision component
    bulletEntity->cCollision = make_shared<CCollision>(m_bulletConfig.collisionRadius);
    
    // Instantiate Lifespan component
    bulletEntity->cLifespan = make_shared<CLifespan>(m_bulletConfig.lifespan);
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
        return 3;
    }

    Vec2 diff = entity1->cTransform->position - entity2->cTransform->position;
    float sumOfRadii = entity1->cCollision->radius + entity2->cCollision->radius;

    // If distance^2 < (r1+r2)^2
    if (diff.x * diff.x + diff.y * diff.y < sumOfRadii * sumOfRadii)
        return true;
    else
        return false;
}