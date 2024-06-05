#include "SceneStartMenu.h"

SceneStartMenu::SceneStartMenu(GameEngine* gameEngine = nullptr)
    : Scene(gameEngine), m_title("Platformer"), m_menuStrings({ "Level 1", "Quit" }), m_levelPaths({ "level.txt" })
{
    init();
    m_menuText.setFont(m_game->getAssets().getFont("/assets/fonts/Dune_Rise.ttf"));
}

void SceneStartMenu::init()
{
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Enter, "SELECT");
}

void SceneStartMenu::update()
{
    sRender();
}

void SceneStartMenu::sRender()
{
    const sf::Window& window = m_game->getWindow();
    int midScreen = window.getSize().x / 2;
    int distanceBetweenStrings = window.getSize().y / (m_menuStrings.size() + 1);
    
    // Sets outline colour & thickness (same for all title screen text)
    m_menuText.setOutlineColor(sf::Color::Black);
    m_menuText.setOutlineThickness(4.f);

    // Clear screen with the background colour
    window.clear(sf::Color::Green);

    // Draw title
    m_menuText.setString(m_title);
    m_menuText.setCharacterSize(60);
    m_menuText.setPosition(midScreen, distanceBetweenStrings);
    m_menuText.setFillColor(sf::Color::Blue);
    window.draw(m_menuText);

    // Draw selectable menu text
    m_menuText.setCharacterSize(45);
    m_menuText.setOutlineThickness(2.f);
    for (int i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setPosition(midScreen, (i + 2) * distanceBetweenStrings);

        if (i == m_selectedMenuIndex)
        {
            m_menuText.setFillColor(sf::Color::Red);
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
    if (action.getType() == "START")
    {
        // Register actions
            // Up arrow -> decrement menu index
            // Down arrow -> increment menu index
            // Enter -> changeScene(selectedLevel)
            // Esc -> quit (already implemented?)
    }
}

