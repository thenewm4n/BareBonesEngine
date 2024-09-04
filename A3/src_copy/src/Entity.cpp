#include "Entity.h"

Entity::Entity(size_t id, const std::string& tag)
    : m_id(id), m_tag(tag) {}

void Entity::destroy() { m_alive = false; }

bool Entity::isAlive() const { return m_alive; }

const size_t Entity::getId() const { return m_id; }

const std::string& Entity::getTag() const { return m_tag; }