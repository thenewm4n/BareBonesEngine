#include "GameEngine.h"

#include "Action.h"
#include "SceneStartMenu.h"

#include "imgui.h"
#include "imgui-SFML.h"

#include <chrono>
#include <fstream>
#include <iomanip>
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

    ImGui::SFML::Shutdown();
    m_window.close();
}

void GameEngine::quit() { m_running = false; }

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene)
{
    m_sceneMap[sceneName] = scene;  // Automatically adds new scene to map, or reassigns an already existing scene
    m_currentScene = sceneName;
}

bool GameEngine::isRunning() { return m_running && m_window.isOpen(); }

sf::RenderWindow& GameEngine::getWindow() { return m_window; }

sf::Vector2u GameEngine::getResolution() { return m_resolution; }

float GameEngine::getAspectRatio() { return m_aspectRatio; }

const Assets& GameEngine::getAssets() const { return m_assets; }


// Private methods

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

    // Create window using values from config.txt or current desktop configuration
    // sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    m_window.create(sf::VideoMode(m_resolution.x, m_resolution.y), "BareBones", sf::Style::Default);    // TODO: change title and fullscreen according to config
    m_window.setFramerateLimit(framerateCap);
    m_aspectRatio = static_cast<float>(m_resolution.x) / static_cast<float>(m_resolution.y);

    // Create and resize ImGui
    auto result = ImGui::SFML::Init(m_window);
    if (!result)
    {
        std::cerr << "ImGui::SFML::Init failed." << std::endl;
        exit(-8);
    }
    ImGui::GetStyle().ScaleAllSizes(3.5f);		// Scales imgui elements
    ImGui::GetIO().FontGlobalScale = 1.3f;		// Scales imgui text size

    // Load assets into Assets object
    m_assets.loadFromFile("assets.txt");

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
        ImGui::SFML::ProcessEvent(m_window, event);
        
        switch (event.type)
        {
            case sf::Event::KeyPressed:
            case sf::Event::KeyReleased:
                // Instantiates an Action object and makes Scene perform Action if registered in the current scene
                if (getCurrentScene()->getActionMap().find(event.key.code) != getCurrentScene()->getActionMap().end())
                {
                    const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
                    getCurrentScene()->sDoAction(Action(getCurrentScene()->getActionMap().at(event.key.code), actionType));
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L)
                {
                    takeScreenshot();
                }
                break;
            
            case sf::Event::Closed:
                quit();
                break;

            case sf::Event::Resized:
                if ((event.size.width != m_resolution.x) || (event.size.height != m_resolution.y))
                {
                    // Changes window size according to whether width or height has changed
                    sf::Vector2u newWindowSize = (event.size.width != m_resolution.x) ?
                        sf::Vector2u(event.size.width, static_cast<unsigned int>(event.size.width / m_aspectRatio)) :
                        sf::Vector2u(static_cast<unsigned int>(event.size.height * m_aspectRatio), event.size.height);

                    m_window.setSize(newWindowSize);

                    // Update local resolution variable
                    m_resolution = m_window.getSize();
                }
                break;

            default:
                break;
        }
    }
}

void GameEngine::takeScreenshot()
{
    // Create a texture to hold the current window contents
    sf::Texture texture;
    texture.create(m_window.getSize().x, m_window.getSize().y);
    texture.update(m_window);

    // Create an image from the texture
    sf::Image screenshot = texture.copyToImage();

    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::tm now_tm;

    // Uses correct version of localtime function according to platform
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&now_tm, &now_c);
    #elif defined(__unix__) || defined(__linux__) || defined(__APPLE__)
        localtime_r(&now_c, &now_tm);
    #else
        #error "Unsupported platform"
    #endif

    // Format the filename with date and time
    std::ostringstream filename;
    filename << "screenshot_" 
             << std::put_time(&now_tm, "%Y-%m-%d_%H-%M-%S") 
             << ".png";

    if (screenshot.saveToFile(filename.str()))
    {
        std::cout << "Screenshot saved as " + filename.str() << '.' << std::endl;
    }
    else
    {
        std::cerr << "Error saving screenshot!" << std::endl;
    }
}

std::shared_ptr<Scene> GameEngine::getCurrentScene() { return m_sceneMap[m_currentScene]; }