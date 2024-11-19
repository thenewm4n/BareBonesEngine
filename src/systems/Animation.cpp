#include "Animation.h"

#include <cmath>
#include <iostream>

Animation::Animation() : m_name("NONE"), m_frameSize(Vec2f(1.0f, 1.0f)), m_frameCount(1), m_frameDuration(1) {}

Animation::Animation(const sf::Texture& tex, const std::string& animationName, int frameCount, int frameDuration)
    : m_name(animationName),
    m_sprite(tex),
    m_frameCount(frameCount),
    m_frameDuration(frameDuration)
{
    m_frameSize = Vec2f(static_cast<float>(tex.getSize().x) / frameCount, static_cast<float>(tex.getSize().y));
    m_sprite.setOrigin(m_frameSize.x / 2.0f, m_frameSize.y / 2.0f);                                                     // Origin set to centre of sprite
    m_sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(m_frameSize.x), static_cast<int>(m_frameSize.y)));       // left, top, width, height
}

// Updates animation to show next frame, depending on its speed, then loops when reaches end
void Animation::update()
{
    // Calculate correct frame to display
    m_currentAnimationFrame = (m_gameLoopsSinceStart - (m_gameLoopsSinceStart % m_frameDuration)) / m_frameDuration;    // for standing animation, this results in 0 / 1

    // Set texture rectangle properly
    sf::IntRect frameRect(std::floor(m_currentAnimationFrame * m_frameSize.x), 0, m_frameSize.x, m_frameSize.y);

    m_sprite.setTextureRect(frameRect);

    // Add the speed variable to the current frame (?)
    m_gameLoopsSinceStart++;
}

void Animation::reset()
{
	m_gameLoopsSinceStart = 0;
    update();
}

const bool Animation::hasEnded() const
{
    return m_currentAnimationFrame > m_frameCount - 1;
}

const std::string& Animation::getName() const { return m_name; }

const Vec2f& Animation::getSize() const { return m_frameSize; }

sf::Sprite& Animation::getSprite() { return m_sprite; }

const int Animation::getCurrentFrame() const { return m_currentAnimationFrame; }

const int Animation::getFrameCount() const { return m_frameCount; }

const int Animation::getFrameDuration() const { return m_frameDuration; }