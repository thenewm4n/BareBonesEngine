#pragma once

#include "Vec2.h"

#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation();
    Animation(const sf::Texture& tex, const std::string& animationName = "NONE", int frameCount = 1, int frameDuration = 1);

    void update();
    
    bool hasEnded() const;
    const std::string& getName() const;
    const Vec2f& getSize() const;
    sf::Sprite& getSprite();

private:
    std::string m_name;
    sf::Sprite m_sprite;
    Vec2f m_size;                       // Pixel dimensions of each animation frame
    int m_currentAnimationFrame = 0;
    size_t m_gameLoopsSinceStart = 0;   
    int m_frameCount;                   // No. frames in one loop of animation
    int m_frameDuration;                // Number of game loops each animation frame persists
};