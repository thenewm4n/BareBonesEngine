#include "ConfigManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

ConfigManager::ConfigManager() {}

bool ConfigManager::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening config file." << std::endl;
        return false;
    }

    
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string key;

        // If there is text, read into key until '=', and discard '='
        if (std::getline(lineStream, key, '='))
        {
            std::string value;

            // If there is text, read into value until end
            if (std::getline(lineStream, value))
            {
                // Set key-value pair
                m_config[key] = value;
            }
        }
    }

    return true;
}

std::string ConfigManager::getString(const std::string& key, const std::string& defaultValue) const
{
    auto iterator = m_config.find(key);
    if (iterator != m_config.end())
    {
        std::string value = iterator->second;
        
        // Trim leading and trailing whitespace
        value.erase(0, value.find_first_not_of(" \t"));     // Erase from first position to position of first character that is not a space or tab
        value.erase(value.find_last_not_of(" \t") + 1);     // Erase from position of last character that is not a space or tab until end
        
        return value;
    }
    
    std::cerr << "Error: no key called " << key << " found in config file." << std::endl;
    return defaultValue;
}

int ConfigManager::getInt(const std::string& key, int defaultValue) const
{
    auto iterator = m_config.find(key);
    if (iterator == m_config.end())
    {
        std::cerr << "Error: no key called " << key << " found in config file." << std::endl;
        return defaultValue;
    }

    try
    {
        return std::stoi(iterator->second);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error converting value for key '" << key << "' to int: " << e.what() << std::endl;
    }

    return defaultValue;
}

float ConfigManager::getFloat(const std::string& key, float defaultValue) const
{
    auto iterator = m_config.find(key);
    if (iterator == m_config.end())
    {
        std::cerr << "Error: no key called " << key << " found in config file." << std::endl;
        return defaultValue;
    }

    try
    {
        return std::stof(iterator->second);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error converting value for key '" << key << "' to int: " << e.what() << std::endl;
    }

    return defaultValue;
}

bool ConfigManager::getBool(const std::string& key, bool defaultValue) const
{
    auto iterator = m_config.find(key);
    
    // If key is in the map, access value
    if (iterator != m_config.end())
    {
        std::string value = m_config.at(key);
        if (value == "true")
        {
            return true;
        }
        else if (value == "false")        // Requires values to be "true" or "false"
        {
            return false;
        }
        else
        {
            std::cerr << "Error: boolean value for key '" << key << "' must be exactly \"true\" or \"false\"." << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: no key called " << key << " found in config file." << std::endl;
    }

    return defaultValue;
}

std::vector<std::string> ConfigManager::getStringVector(const std::string& key, const std::vector<std::string>& defaultValue) const
{
    auto iterator = m_config.find(key);
    if (iterator == m_config.end())
    {
        std::cerr << "Error: no key called " << key << " found in config file." << std::endl;
        return defaultValue;
    }

    const std::string& value = iterator->second;
    std::vector<std::string> result;
    std::istringstream stream(value);
    std::string token;

    while(std::getline(stream, token, ','))
    {
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
    }

    if (!token.empty())
    {
        result.push_back(token);
    }

    return result;
}