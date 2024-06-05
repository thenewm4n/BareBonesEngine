#pragma once

class Action
{
public:
    std::string m_name;
    std::string m_type;

public:
    Action(const std::string& actionName, const std::string& actionType);
    const std::string& getName() const;
    const std::string& getType() const;
};