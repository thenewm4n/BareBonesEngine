#include "SceneStartMenu.h"

#include "SceneLevelEditor.h"
#include "ScenePlatformer.h"

SceneStartMenu::SceneStartMenu(GameEngine* gameEngine) :
    Scene(gameEngine),
    m_title(gameEngine->GAME_TITLE),
    m_menuStrings(gameEngine->LEVELS),//{ "Level 1", "Level 2", "Level 3" }),
    m_levelFiles({"level_Mario.txt", "level_Platformer.txt"}),
    m_menuText(sf::Text(m_game->getAssets().getFont("Pixel"), "", 150))
{
    init();
}

void SceneStartMenu::init()
{
    registerAction(sf::Keyboard::Key::W, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::Enter, "SELECT");
    registerAction(sf::Keyboard::Key::Grave, "EDIT_LEVEL");
    registerAction(sf::Keyboard::Key::Escape, "QUIT");

    m_menuText.setOutlineColor(sf::Color::Black);
    m_menuText.setOutlineThickness(1.5f);

    sf::View view(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));  // View size is hardcoded, as to be consistent after window resizing
    m_game->getWindow().setView(view);
}

void SceneStartMenu::update()
{
    sRender();
}

void SceneStartMenu::sPerformAction(const Action& action)
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
        else if (actionName == "SELECT" || actionName == "EDIT_LEVEL")
        {
            const std::string sceneName("LEVEL_%i", m_selectedMenuIndex);
            const std::filesystem::path levelPath =
                m_game->getExecutableDir().parent_path() /
                "assets" /
                "levels" /
                m_levelFiles[m_selectedMenuIndex];

            std::shared_ptr<Scene> levelScene;
            
            if (actionName == "SELECT")
                levelScene = std::make_shared<ScenePlatformer>(m_game, levelPath);
            else                // If editing level
                levelScene = std::make_shared<SceneLevelEditor>(m_game, levelPath);
            
            m_game->changeScene(sceneName, levelScene);
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
    m_menuText.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
    m_menuText.setPosition({midScreenX, distanceBetweenStrings});

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
        m_menuText.setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
        m_menuText.setPosition({midScreenX, (i + 2) * distanceBetweenStrings});

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