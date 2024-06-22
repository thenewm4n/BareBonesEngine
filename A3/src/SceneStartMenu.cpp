#include "SceneStartMenu.h"
#include "ScenePlatformer.h"
#include <iostream>

SceneStartMenu::SceneStartMenu(GameEngine* gameEngine)
    : Scene(gameEngine), m_title("Platformer"), m_menuStrings({ "Level 1", "Level 2", "Level 3" }), m_levelPaths({"level.txt"})
{
    init();
    
    // Set universal menu text properties
    m_menuText.setOutlineColor(sf::Color::Black);
    m_menuText.setOutlineThickness(1.5f);
    m_menuText.setFont(m_game->getAssets().getFont("Pixel"));

    // Set window view
    //m_game->getWindow().setView(sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(1920.f, 1080.f)));
    sf::View titleScreenView(sf::FloatRect(0.f, 0.f, 1920.f, 1080.f)); // Adjust dimensions as needed
    m_game->getWindow().setView(titleScreenView);
}

void SceneStartMenu::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Enter, "SELECT");
}

void SceneStartMenu::update()
{
    sRender();
}

void SceneStartMenu::sRender()
{
    sf::RenderWindow& window = m_game->getWindow();
    static float midScreenX = static_cast<float>(window.getView().getSize().x) / 2.f;
    static float distanceBetweenStrings = static_cast<float>(window.getView().getSize().y) / (static_cast<float>(m_menuStrings.size() + 2));

    // Clear screen with the background colour
    window.clear(sf::Color(244, 214, 204));

    // Set title text properties
    m_menuText.setString(m_title);
    m_menuText.setCharacterSize(120);
    m_menuText.setPosition(midScreenX, distanceBetweenStrings);
    m_menuText.setFillColor(sf::Color(244, 180, 96));

    // Set origin to center of text after character size change
    static sf::FloatRect textRect = m_menuText.getLocalBounds();
    m_menuText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

    std::cout << "After setting title text... " "Left: " << textRect.left << " " << "Top: " << textRect.top << " " << "Width : " << textRect.width << " " << "Height : " << textRect.height << std::endl;
    std::cout << "Origin X: " << m_menuText.getOrigin().x << " " << "Origin Y: " << m_menuText.getOrigin().y << std::endl;

    // Draw title text
    window.draw(m_menuText);

    // Set properties selectable menu text
    m_menuText.setCharacterSize(70);

    // Set origin according to new text size
    textRect = m_menuText.getLocalBounds();
    m_menuText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);

    std::cout << "After setting selectable text... " "Left: " << textRect.left << " " << "Top: " << textRect.top << " " << "Width : " << textRect.width << " " << "Height : " << textRect.height << std::endl;
    std::cout << "Origin X: " << m_menuText.getOrigin().x << " " << "Origin Y: " << m_menuText.getOrigin().y << std::endl;

    // Draw selectable menu text
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setPosition(midScreenX, (i + 2) * distanceBetweenStrings);

        if (i == m_selectedMenuIndex)
        {
            m_menuText.setFillColor(sf::Color(74, 88, 89));
        }
        else
        {
            m_menuText.setFillColor(sf::Color::White);
        }

        window.draw(m_menuText);
    }

    window.display();
}

void SceneStartMenu::endScene()
{
    m_game->quit();
}

void SceneStartMenu::sDoAction(const Action& action)
{
    const std::string& actionName = action.getName();

    // Implement scene-specific consequences for Actions
    if (action.getType() == "START")
    {
        if (actionName == "UP")
        {
            if (m_selectedMenuIndex > 0)
            {
                m_selectedMenuIndex--;
            }
        }
        else if (actionName == "DOWN")
        {
            if (m_selectedMenuIndex < m_menuStrings.size() - 1)
            {
                m_selectedMenuIndex++;
            }
        }
        else if (actionName == "SELECT")
        {
            const std::string sceneName("LEVEL_%i", m_selectedMenuIndex);
            m_game->changeScene(sceneName, std::make_shared<ScenePlatformer>(m_game, m_levelPaths[m_selectedMenuIndex]));
        }
        else if (actionName == "QUIT")
        {
            m_game->quit();
        }
    }
}

