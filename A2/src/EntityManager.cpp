#include "EntityManager.h"

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
    // Adding entities
    for (auto& entity : m_entitiesToAdd)        // use & to avoid copy of the shared_ptr, and therefore increase of its reference count
    {
        // Add entity to vector of all entities
        m_entities.push_back(entity);

        // Add entity to vector of entities with corresponding tag in m_entityMap
        m_entityMap[entity.getTag()].push_back(entity);
    }


    // Removing entities
    // - Remove the dead entities from vector of all entities
    removeDeadEntities(m_entities);

    // - Remove dead entities from each vector in the entitiy map
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

    // To avoid iterator invalidation, iterate over and mark for removal, then iterate over again and remove

    // First iteration for marking
    EntityVector entitiesForRemoval;
    for (auto& entity : m_entities)
    {
        if (!entity.isAlive())
        {
            entitiesForRemoval.push_back(entity);
        }
    }

    // Second iteration for removal
    for (auto& entity : m_entitiesForRemoval)
    {
       // WHAT IS GOING ON HERE
       // m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [entity.getTag()](const Entity& entity) { return entity.}));
    }
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