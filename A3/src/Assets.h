#pragma once

#include "Animation.h"

#include <SFML/Graphics.hpp>

#include <map>

using AnimationMap = std::map<std::string, Animation>;
using FontMap = std::map<std::string, sf::Font>;
using TextureMap = std::map<std::string, sf::Texture>;

class Assets
{
public:
    Assets();

    void loadFromFile(const std::string& assetsFilePath);

    const Animation& getAnimation(const std::string& animationName) const;
    const sf::Font& getFont(const std::string& fontName) const;
    const sf::Texture& getTexture(const std::string& textureName) const;

private:
    AnimationMap m_animationMap;
    FontMap m_fontMap;
    TextureMap m_textureMap;
};