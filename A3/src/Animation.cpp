#include <cmath>

#include "Animation.h"

Animation::Animation()
{
}

Animation::Animation(const std::string& animationName, const sf::Texture& tex)
    : Animation(animationName, tex, 1, 0)
{
}

Animation::Animation(const std::string& animationName, const sf::Texture& tex, size_t frameCount, size_t frameDuration)
    : m_name(animationName), m_sprite(tex), m_frameCount(frameCount), m_frameDuration(frameDuration), m_currentFrame(0)
{
    m_size = Vec2f(static_cast<float>(tex.getSize().x) / frameCount, static_cast<float>(tex.getSize().y));
    m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);       // Origin set to centre of sprite
    m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));      // left, top, width, height
}

// Updates animation to show next frame, depending on its speed, then loops when reaches end
void Animation::update()
{
    // Add the speed variable to the current frame (?)
    m_currentFrame++;

    // TODO
    // 1) Calculate the correct frame of animation to play based on currentFrame and speed
    // 2) Set texture rectangle properly (see constructor for sample)
}

bool Animation::hasEnded() const
{
    // TODO: detect when animation has ended (i.e. last frame was played) and return true, else false
}

const std::string& Animation::getName() const
{
    return m_name;
}

const Vec2f& Animation::getSize() const
{
    return m_size;
}

sf::Sprite& Animation::getSprite()
{
    return m_sprite;
}