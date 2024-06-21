#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "Assets.h"
#include "Animation.h"


Assets::Assets()
{
}

void Assets::loadFromFile(const std::string& assetsFilePath)
{
    std::ifstream file(assetsFilePath);
    if (!file.is_open())
    {
        std::cerr << "Error opening assets file." << std::endl;
        exit(-1);
    }

    // Read assets.txt
    std::string line, firstElement;

    while(std::getline(file, line))
    {
        // Enables reading of tokens separated by whitespace
        std::istringstream lineStream(line);

        // If there is text on this line, parse line
        if (lineStream >> firstElement)
        {
            if (firstElement == "Animation")
            {
                std::string animationName, textureName;
                int framesInAnimation, frameDuration;

                lineStream >> animationName >> textureName >> framesInAnimation >> frameDuration;

                m_animationMap[animationName] = Animation(animationName, m_textureMap[textureName], framesInAnimation, frameDuration);
            }
            else if (firstElement == "Font")
            {
                std::string name, path;

                lineStream >> name >> path;

                // Load font from file
                sf::Font font;
                if (!font.loadFromFile(path))
                {
                    std::cerr << "Assets.cpp, Line 53 - Could not load font: " << name << std::endl;
                    exit(-2);
                }

                // Add loaded font to map
                m_fontMap[name] = font;
            }
            else if (firstElement == "Texture")
            {
                std::string name, path;

                lineStream >> name >> path;

                sf::Texture texture;
                if (!texture.loadFromFile(path))
                {
                    std::cerr << "Could not load texture:" << name << std::endl;
                    exit(-3);
                }

                m_textureMap[name] = texture;
            }
        }
    }

    file.close();
}

const Animation& Assets::getAnimation(const std::string& animationName) const
{
    try
    {
        return m_animationMap.at(animationName);
    }
    catch (const std::out_of_range&)
    {
        std::cerr << "Animation not found: " << animationName << std::endl;
        exit(-4);
    }
}

const sf::Font& Assets::getFont(const std::string& fontName) const
{
    try
    {
        return m_fontMap.at(fontName);
    }
    catch (const std::out_of_range&)
    {
        std::cerr << "Font not found: " << fontName << std::endl;
        exit(-5);
    }
}

const sf::Texture& Assets::getTexture(const std::string& textureName) const
{    
    try
    {
        return m_textureMap.at(textureName);
    }
    catch (const std::out_of_range&)
    {
        std::cerr << "Texture not found: " << textureName << std::endl;
        exit(-6);
    }
}