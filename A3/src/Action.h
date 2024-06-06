#pragma once

#include <string>

class Action
{
private:
    std::string m_name;
    std::string m_type;

public:
    Action();
    Action(const std::string& actionName, const std::string& actionType);

    const std::string& getName() const;
    const std::string& getType() const;
};