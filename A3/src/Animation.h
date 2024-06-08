#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "Vec2.h"

class Animation
{
private:
    std::string m_name = "NONE";
    sf::Sprite m_sprite;
    size_t m_frameCount = 1;        // No. frames in the animation
    size_t m_currentFrame = 0;      // Current animation frame
    size_t m_frameDuration = 0;     // Number of game frames each animation frame persists
    Vec2f m_size = { 1, 1 };        // Size of each animation frame

public:
    Animation();
    Animation(const std::string& animationName, const sf::Texture& tex);        // Calls the below constructor
    Animation(const std::string& animationName, const sf::Texture& tex, size_t frameCount, size_t frameDuration);

    void update();
    bool hasEnded() const;
    const std::string& getName() const;
    const Vec2f& getSize() const;
    sf::Sprite& getSprite();
};