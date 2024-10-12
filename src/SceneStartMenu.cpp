#include "SceneStartMenu.h"

#include "ScenePlatformer.h"

SceneStartMenu::SceneStartMenu(GameEngine* gameEngine)
    : Scene(gameEngine), m_title("Game Engine: The Game"), m_menuStrings({ "Level 1", "Level 2", "Level 3" }), m_levelPaths({"assets/levels/level_1.txt"})
{
    init();
}

void SceneStartMenu::init()
{
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::Enter, "SELECT");
    registerAction(sf::Keyboard::Escape, "QUIT");

    m_menuText.setOutlineColor(sf::Color::Black);
    m_menuText.setOutlineThickness(1.5f);
    m_menuText.setFont(m_game->getAssets().getFont("Pixel"));

    sf::View view(sf::FloatRect(0.0f, 0.0f, 1920.0f, 1080.0f));  // View size is hardcoded, as to be consistent after window resizing
    m_game->getWindow().setView(view);
}

void SceneStartMenu::update()
{
    sRender();
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

void SceneStartMenu::sRender()
{
    sf::RenderWindow& window = m_game->getWindow();
    static float midScreenX = window.getView().getSize().x / 2.0f;
    static float distanceBetweenStrings = window.getView().getSize().y / static_cast<float>(m_menuStrings.size() + 2);

    // Clear screen with the background colour
    window.clear(sf::Color(244, 214, 204));

    // Set title text properties
    m_menuText.setString(m_title);
    m_menuText.setCharacterSize(150);
    m_menuText.setFillColor(sf::Color(244, 180, 96));

    // Set origin to center of text after changing string and character size, then set position with new origin
    sf::FloatRect textRect = m_menuText.getLocalBounds();
    m_menuText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    m_menuText.setPosition(midScreenX, distanceBetweenStrings);

    // Draw title text
    window.draw(m_menuText);

    // Set character size for selectable menu text
    m_menuText.setCharacterSize(100);

    // Draw selectable menu text
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);

        // Set origin according to new string, then set position with new origin
        textRect = m_menuText.getLocalBounds();
        m_menuText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
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