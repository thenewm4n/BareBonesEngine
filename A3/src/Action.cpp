#include "Action.h"

Action::Action()
{
}

Action::Action(const std::string& actionName, const std::string& actionType)
    : m_name(actionName), m_type(actionType)
{
}

const std::string& Action::getName() const { return m_name; }

const std::string& Action::getType() const { return m_type; }