#include "SceneStartMenu.h"
#include "SceneLevelEditor.h"
#include "ScenePlatformer.h"

#include <fstream>
#include <sstream>


SceneStartMenu::SceneStartMenu(GameEngine* gameEngine)
    : Scene(gameEngine),
    m_title(m_game->getTitle()),
    m_menuText(sf::Text(m_game->getAssets().getFont("Pixel"), "", 150))
{
    init();
}

void SceneStartMenu::init()
{
    // Read level names
    std::ifstream levelsTextFile(m_game->getExecutableDir() / "assets/levels/levels.txt");
    std::string line;
    std::string token;

    while (std::getline(levelsTextFile, line))
    {
        std::istringstream lineStream(line);
        
        LevelData level;
        if (lineStream >> level.name >> level.path)
        {
            m_levels.emplace_back(level);
        }
    }

    // TODO: add Play, Level Select, Settings and Exit menu strings;
    for (size_t i = 0; i < m_levels.size(); i++)
    {
        m_menuStrings.push_back(m_levels[i].name);
    }

    registerAction(sf::Keyboard::Key::W, "UP");
    registerAction(sf::Keyboard::Key::S, "DOWN");
    registerAction(sf::Keyboard::Key::Enter, "SELECT");
    registerAction(sf::Keyboard::Key::Grave, "EDIT_LEVEL");
    registerAction(sf::Keyboard::Key::Escape, "QUIT");

    m_menuText.setOutlineColor(sf::Color::Black);
    m_menuText.setOutlineThickness(1.5f);

    sf::View view(sf::FloatRect({ 0.0f, 0.0f }, {1920.0f, 1080.0f}));     // View size is hardcoded, as to be consistent after window resizing
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
            const std::filesystem::path levelPath =
                m_game->getExecutableDir() /
                "assets" /
                "levels" /
                m_levels[m_selectedMenuIndex].path;

            std::shared_ptr<Scene> levelScene;
            
            if (actionName == "SELECT")
                levelScene = std::make_shared<ScenePlatformer>(m_game, levelPath);
            else                // If editing level
                levelScene = std::make_shared<SceneLevelEditor>(m_game, levelPath);
            
            m_game->changeScene(m_levels[m_selectedMenuIndex].name, levelScene);
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
    float midScreenX = window.getView().getSize().x / 2.0f;
    float distanceBetweenStrings = window.getView().getSize().y / static_cast<float>(m_menuStrings.size() + 2);

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