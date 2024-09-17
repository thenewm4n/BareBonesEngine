#include "GameEngine.h"

#include "Action.h"
#include "SceneStartMenu.h"

#include <fstream>
#include <iostream>
#include <sstream>


// Public methods

GameEngine::GameEngine(const std::string& configFilePath) { init(configFilePath); }

void GameEngine::run()
{
    while (isRunning())
    {
        update();
    }

    m_window.close();
}

void GameEngine::quit() { m_running = false; }

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene)
{
    m_sceneMap[sceneName] = scene;  // Automatically adds new scene to map, or reassigns an already existing scene
    m_currentScene = sceneName;
}

bool GameEngine::isRunning() { return m_running && m_window.isOpen(); }   // true if game is running and sf::Window exists

sf::RenderWindow& GameEngine::getWindow() { return m_window; }

sf::Vector2u GameEngine::getResolution() { return m_resolution; }

float GameEngine::getAspectRatio() { return m_aspectRatio; }

const Assets& GameEngine::getAssets() const { return m_assets; }


// Protected methods

void GameEngine::init(const std::string& configFilePath)
{
    std::ifstream file(configFilePath);
    if (!file.is_open())
    {
        std::cerr << "GameEngine.cpp, Line 20: Error opening config file." << std::endl;
        exit(-1);
    }

    // Read config.txt
    std::string line;
    std::string firstElement;

    int framerateCap = 60;

    while (std::getline(file, line))
    {
        // Enables reading of tokens separated by whitespace
        std::istringstream lineStream(line);

        // If there is text on this line, parse line
        if (lineStream >> firstElement)
        {
            if (firstElement == "Window")
            {
                lineStream >> m_resolution.x >> m_resolution.y >> framerateCap;
            }
        }
    }

    file.close();

    // Create window using values from config.txt
    m_window.create(sf::VideoMode(m_resolution.x, m_resolution.y), "Assignment 3");
    m_window.setFramerateLimit(framerateCap);
    m_aspectRatio = static_cast<float>(m_resolution.x) / static_cast<float>(m_resolution.y);

    // Creates GUI
    /*
    ImGui::SFML::Init(m_window);
    ImGui::GetStyle().ScaleAllSizes(3.5f);		// Scales imgui GUI
    ImGui::GetIO().FontGlobalScale = 1.3f;		// Scale imgui text size
    */

    // Load assets into Assets object
    m_assets.loadFromFile("bin/assets.txt");

    // Create new menu scene, add it to scene map, and make it the current scene
    changeScene("MENU", std::make_shared<SceneStartMenu>(this));
}

void GameEngine::update()
{
    sUserInput();
    getCurrentScene()->update();
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // ImGui::SFML::ProcessEvent(m_window, event);

        if (event.type == sf::Event::Closed)
        {
            quit();
        }
        else if (event.type == sf::Event::Resized)
        {
            // If window hasn't changed size, continue to next iteration of while loop
            if (event.size.width == m_resolution.x && event.size.height == m_resolution.y)
            {
                continue;
            }

            // If one dimension has changed, change the other according to aspect ratio
            if (event.size.width != m_resolution.x)
            {
                m_window.setSize(sf::Vector2u(event.size.width, event.size.width / m_aspectRatio));
            }
            else
            {
                m_window.setSize(sf::Vector2u(event.size.height * m_aspectRatio, event.size.height));
            }

            // Update local resolution variable
            m_resolution = m_window.getSize();

            continue;
        }
        // Instantiates an Action object and makes Scene perform Action if registered in the current scene
        else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // If X pressed, take screenshot, regardless of scene
            if (event.key.code == sf::Keyboard::X)
            {
                sf::Texture texture;
                texture.create(m_window.getSize().x, m_window.getSize().y);
                texture.update(m_window);

                // If successful screenshot, print
                if (texture.copyToImage().saveToFile("test.png"))
                {
                    std::cout << "Screenshot saved as test.png." << std::endl;
                }

                // Continue to next event
                continue;
            }

            // If the current scene doesn't have a mapping for the key pressed/released, continue
            if (getCurrentScene()->getActionMap().find(event.key.code) == getCurrentScene()->getActionMap().end())
            {
                continue;
            }

            // If the key was pressed, type is START, else type is END
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

            // Lookup action in current scene's action map, and do action within this scene
            getCurrentScene()->sDoAction(Action(getCurrentScene()->getActionMap().at(event.key.code), actionType));
        }
    }
}

std::shared_ptr<Scene> GameEngine::getCurrentScene() { return m_sceneMap[m_currentScene]; }