#include "SceneStartMenu.h"
#include "ScenePlatformer.h"

SceneStartMenu::SceneStartMenu(GameEngine* gameEngine)
    : Scene(gameEngine), m_title("Platformer"), m_menuStrings({ "Level 1", "Level 2", "Level 3" }), m_levelPaths({ "level.txt" })
{
    init();
    m_menuText.setFont(m_game->getAssets().getFont("Pixel"));
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
    float midScreen = static_cast<float>(window.getSize().x) / 2.f;
    float distanceBetweenStrings = static_cast<float>(window.getSize().y) / (static_cast<float>(m_menuStrings.size() + 1));

    // Sets outline colour & thickness (same for all title screen text)
    m_menuText.setOutlineColor(sf::Color::Black);
    m_menuText.setOutlineThickness(1.f);

    // Clear screen with the background colour
    window.clear(sf::Color(244, 214, 204));

    // Draw title
    m_menuText.setString(m_title);
    m_menuText.setCharacterSize(80);
    m_menuText.setPosition(midScreen, distanceBetweenStrings);
    m_menuText.setFillColor(sf::Color(244, 180, 96));
    window.draw(m_menuText);

    // Draw selectable menu text
    m_menuText.setCharacterSize(60);
    m_menuText.setOutlineThickness(0.f);
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setPosition(midScreen, (i + 2) * distanceBetweenStrings);

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

