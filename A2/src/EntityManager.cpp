#include "EntityManager.h"

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
    // TODO: add entities from m_entityToAdd to the proper location(s)
    // - add them to vector of all entities
    // - add them to vector inside the map, with the tag as the (?)key

    // Remove the dead entities from the vector of all entities
    removeDeadEntities(m_entities);

    // Remove dead entities from each vector in the entitiy map
    for (auto& [tag, entityVector] : m_entityMap)
    {
        removeDeadEntities(entityVector);
    }
}

void EntityManager::removeDeadEntities(EntityVector& vector)
{
    // TODO: remove all dead entities from the input vector
    // (this is called by update())

    // To avoid iterator invalidation...
        // loop over all entities, checking which are dead (i.e. !isAlive())
        // then, loop over these marked entities and remove them from the vector
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));  // sets id as m_totalEntities, then increments it
    m_entitiesToAdd.push_back(entity);
    return entity;   
}

const EntityVector& EntityManager::getEntities()
{
    return m_entities;
}

const EntityVector& EntityManager::getEntities(const std::string& tag)
{
    // TODO return the vector of entities with the given tag from the map
    return m_entities; // this is wrong
}

const std::map<std::string, EntityVector>& EntityManager::getEntityMap()
{
    return m_entityMap;
}