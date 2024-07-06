#pragma once

#include "Vec2.h"

#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation();
    Animation(const std::string& animationName, const sf::Texture& tex, int frameCount = 1, int frameDuration = 10);

    void update();
    void flipX(bool shouldBeFlipped);
    
    bool hasEnded() const;
    bool isFlipped() const;
    const std::string& getName() const;
    const Vec2f& getSize() const;
    sf::Sprite& getSprite();

private:
    std::string m_name = "NONE";
    sf::Sprite m_sprite;
    Vec2f m_size;            // Size of each animation frame
    int m_currentAnimationFrame;
    int m_frameCount;                // No. frames in the animation
    int m_frameDuration;             // Number of game loops each animation frame persists
    size_t m_gameLoopsSinceStart;
    bool m_flippedX = false;
};