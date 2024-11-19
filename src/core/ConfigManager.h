#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class ConfigManager
{
    public:
        // Initialisation
        ConfigManager();
        bool loadFromFile(const std::string& filename);

        // Getters
        std::string getString(const std::string& key, const std::string& defaultValue = "") const;
        int getInt(const std::string& key, int defaultValue = 0) const;
        float getFloat(const std::string& key, float defaultValue = 0.0f) const;
        bool getBool(const std::string& key, bool defaultValue = false) const;
        std::vector<std::string> getStringVector(const std::string& key, const std::vector<std::string>& defaultValue = {}) const;

    private:
        std::unordered_map<std::string, std::string> m_config;
};