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

GameEngine::GameEngine(const std::filesystem::path& executableDir)
    : m_executableDir(executableDir), m_assets(executableDir.parent_path() / "assets")
{
    init();
}

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

const std::filesystem::path& GameEngine::getExecutableDir() { return m_executableDir; }

sf::RenderWindow& GameEngine::getWindow() { return m_window; }

sf::Vector2u GameEngine::getResolution() { return m_resolution; }

float GameEngine::getAspectRatio() { return m_aspectRatio; }

const Assets& GameEngine::getAssets() const { return m_assets; }


// Private methods

void GameEngine::init()
{
    std::ifstream file(m_executableDir / "config.txt");
    if (!file.is_open())
    {
        std::cerr << "GameEngine.cpp, Line 57: Error opening config file." << std::endl;
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
    m_window.create(sf::VideoMode({m_resolution.x, m_resolution.y}), GAME_TITLE, sf::Style::Default);    // TODO: change title and fullscreen according to config
    m_window.setFramerateLimit(framerateCap);
    m_aspectRatio = static_cast<float>(m_resolution.x) / static_cast<float>(m_resolution.y);

    // Create and resize ImGui
    auto result = ImGui::SFML::Init(m_window);
    if (!result)
    {
        std::cerr << "ImGui::SFML::Init failed." << std::endl;
        exit(-8);
    }
    ImGui::GetStyle().ScaleAllSizes(2.5f);		// Scales imgui elements
    ImGui::GetIO().FontGlobalScale = 1.0f;		// Scales imgui text size

    // Load assets into Assets object
    m_assets.loadFromFile(m_executableDir / "assets.txt");

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
    while (const std::optional event = m_window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(m_window, *event);

        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
            handleSceneAction(keyEvent->code, true);

        else if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>())
            handleSceneAction(keyEvent->code, false);

        else if (event->is<sf::Event::Closed>())
            quit();

        else if (const auto* resizeEvent = event->getIf<sf::Event::Resized>())
            resize(resizeEvent->size);
    }
}

void GameEngine::handleSceneAction(const sf::Keyboard::Key& keyCode, bool isKeyPressed)
{
    if (isKeyPressed && keyCode == sf::Keyboard::Key::L)
    {
        takeScreenshot();
        return;
    }

    auto actionMap = getCurrentScene()->getActionMap();
    auto it = actionMap.find(keyCode);

    if (it != actionMap.end())
    {
        // Instantiates an Action object and makes Scene perform Action if registered in the current scene
        const std::string actionType = isKeyPressed ? "START" : "END";
        getCurrentScene()->sPerformAction(Action(it->second, actionType));
    }
}

void GameEngine::takeScreenshot()
{
    // Create a texture to hold the current window contents
    sf::Texture texture;
    if (!texture.resize(m_window.getSize()))
    {
        std::cerr << "GameEngine.cpp::takeScreenshot() - Failed to resize texture for screenshot." << std::endl;
        return;
    }
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

void GameEngine::resize(const sf::Vector2u& newSize)
{
    if ((newSize.x != m_resolution.x) || (newSize.y != m_resolution.y))
    {
        // Changes window size according to whether width or height has changed
        sf::Vector2u adjustedSize = (newSize.x != m_resolution.x)
            ? sf::Vector2u(newSize.x, static_cast<unsigned int>(newSize.x / m_aspectRatio))
            : sf::Vector2u(static_cast<unsigned int>(newSize.y * m_aspectRatio), newSize.y);

        m_window.setSize(adjustedSize);

        // Update local resolution variable
        m_resolution = m_window.getSize();
    }
}

std::shared_ptr<Scene> GameEngine::getCurrentScene() { return m_sceneMap[m_currentScene]; }