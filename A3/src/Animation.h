#pragma once

#include "Vec2.h"

#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation();
    Animation(const std::string& animationName, const sf::Texture& tex);        // Calls the below constructor
    Animation(const std::string& animationName, const sf::Texture& tex, size_t frameCount, size_t frameDuration);

    void update();
    bool hasEnded() const;
    const std::string& getName() const;
    const Vec2f& getSize() const;
    sf::Sprite& getSprite();

private:
    std::string m_name = "NONE";
    sf::Sprite m_sprite;
    Vec2f m_size = { 1, 1 };           // Size of each animation frame
    // size_t m_currentFrame = 0;       // Current animation frame   I'M NOT SURE THIS IS WHAT IT'S MEANT TO BE
    uint8_t m_currentAnimationFrame = 0;
    size_t m_frameCount = 1;            // No. frames in the animation
    size_t m_frameDuration = 1;         // Number of game frames each animation frame persists
    size_t m_gameFramesSinceStart; 
};