#include "Game.h"

Game::Game(const std::string& config)
{
    init(config);
}

void Game::init(const std::string& path)
{
    // TODO: read in config file

    m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
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
        }
        sCollision();
        sUserInput();
        sLifespan();
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
    // TODO: implement all entity movement within this function
    // - should read the m_player->cInput component to determine if player is moving

// for (auto entity : m_entities.getEntities())

    m_player->cTransform->position += m_player->cTransform->velocity;
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
        
        /* if (event.type == escape key)
            quit game
        */

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
                    std::cout << "W key pressed." << std::endl;
                    // TODO: set player's input component "up" to true
                    break;
                default: break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                std::cout << "W key released." << std::endl;
                // TODO: set player's input component "up" to false
                break;
            default: break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            // If ImGui is being clicked, ignores the mouse event
            if (ImGui::GetIO().WantCaptureMouse) { continue; }

            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "Left mouse button clicked at (" << event.MouseButton.x << ", " << event.mouseButton.y << ")" << std::endl;
                // Call spawnBullet() here
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
    // TODO: change code to draw ALL entities
    m_window.clear();
    
    // Set position of player sprite
    m_player->cShape->circle.setPosition(m_player->cTransform->position.x, m_player->cTransform->position.y);

    // Set rotation of player sprite
    m_player->cTransform->angle += 1.f;
    m_player->cShape->circle.setRotation(m_player->cTransform->angle);

    // Draw the player's sf::CircleShape
    m_window.draw(m_player->cShape->circle);

    // Draw the UI
    ImGui::SFML::Render(m_window);

    m_window.display()
}

void Game::sCollision()
{
    // Don't use square root: use if (distance^2 < (r1+r2)^2)

    // TODO: implement all proper collisions between entities
    // - use COLLISION RADIUS, not shape radius

    // Collisions between bullet and enemies
    for (auto bullet : m_entities.getEntities("Bullet"))
    {
        for (auto enemy : m_entities.getEntities("Enemy"))
        {
            // if distance < bullet->cShape->radius + enement->cShape->radius
                // destoy bullet, destroy enemy, spawn small enemies, update score
        }

        for (auto smallEnemy : m_entities.getEntities("SmallEnemy"))
        {

        }
    }

    // Collisions with edge of screen
        // 

    // Collisions between player and enemy
        // set player to dead
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
    // - i.e. m_currentFrame - m_lastEnemySpawnTime
}

void Game::sGUI()
{
    ImGui::Begin("Assignment 2");

    ImGui::Text("Stuff goes here.");

    ImGui::End();
}


// Other functions

// Spawns the player entity in middle of screen
void Game::spawnPlayer()
{
    // Attributes to set - LOOK AT PLAYERCONFIG STRUCT
        // Shape radius (cShape)
        // Collision radius (cCollision)
        // Speed
        // Fill colour & outline colour, outline thickness (cShape)
        // Shape vertices (cShape)


    // TODO: Finish adding all properties of the player with the correct values from the config file
    std::shared_ptr<Entity> entity = m_entities.addEntity("Player");
    
    // TODO: entity->cTransform = std::make_shared<CTransform>(Vec2(200.f, 200.f), Vec2(1.f, 1.f), 0.f);    ACCORDING TO CONFIG
    
    // TODO: entity->cShape = std::make_shared<CShape>(32.f, 8, sf::Color(255, 0, 0), 4.f);
    
    entity->cInput = std::make_shared<CInput>();   

    // Stores the player entity in Game object, since it is modified often
    m_player = entity;
}

// Spawn enemy at random position; trigger intermittently automatially
void Game::spawnEnemy()
{
    // TODO: make sure enemy is spawned properly with the m_enemyConfig variables
        // the enemy must be spawned completely withiin bounds of window (check whether origin is radius away from border)

    // Create entity and add to entity manager (like in spawnPlayer())

    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
    // TODO: spawn small enemies at location of input enemy e

    // when we create the smaller enemy, we have to read the values of the original enemy
    // - spawn a number of small enemies equal to vertices of original enemy
    // - set each small enemy to same colours as original, half the size, set lifespan
    // - small enemies are worth double points of original enemy
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)  // takes entity that fired bullet i.e. player and target based in mouse position
{
    // TODO: implement spawning of the bullet which travels toward target
    // - bullet speed is given as a scalar speed
    // - must set velocity using formula in notes (?)
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    //TODO
}