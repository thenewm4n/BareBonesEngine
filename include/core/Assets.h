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
    Assets(const std::filesystem::path& assetsDir);

    void loadFromFile(const std::string& assetsTxtPath);

    const Animation& getAnimation(const std::string& animationName) const;
    const sf::Font& getFont(const std::string& fontName) const;
    const sf::Texture& getTexture(const std::string& textureName) const;

    const AnimationMap& getAnimationMap() const;
    const FontMap& getFontMap() const;
    const TextureMap& getTextureMap() const;

private:
    std::filesystem::path m_assetsDir;
    AnimationMap m_animationMap;
    FontMap m_fontMap;
    TextureMap m_textureMap;
};