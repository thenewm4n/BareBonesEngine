#pragma once


namespace RenderUtils
{
    void renderEntity(sf::RenderWindow& window, std::shared_ptr<Entity> entity);
    void renderGrid(sf::RenderWindow& window);
    void renderBBox(sf::RenderWindow& window);
}