#include "EntityManager.h"

EntityManager::EntityManager()
{
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));   // sets id as m_totalEntities, then increments it
    
    m_entitiesToAdd.push_back(entity);
    return entity;   
}

// Marks dead entities and removes them from the input vector
void EntityManager::removeDeadEntities(EntityVector& vector)
{
    // To avoid iterator invalidation, iterate over and mark for removal, then iterate over again and remove

    // First iteration for marking
    EntityVector entitiesForRemoval;
    for (const auto& entity : vector)
    {
        if (!entity->isAlive())
        {
            entitiesForRemoval.push_back(entity);
        }
    }

    // Second iteration for removal
    for (const auto& entityToRemove : entitiesForRemoval)
    {
        // Lambda function to pass to remove_if
        auto predicate = [entityToRemove](const std::shared_ptr<Entity>& entity)
        {
            return entity->getId() == entityToRemove->getId();
        };

        // Move matching Entity objects to end of vector, and remove them
        vector.erase(std::remove_if(vector.begin(), vector.end(), predicate), vector.end());
    }
}

void EntityManager::update()
{
    // Adding entities
    for (const auto& entity : m_entitiesToAdd)        // use & to avoid copy of the shared_ptr, and therefore increase of its reference count
    {
        // Add entity to vector of all entities
        m_entities.push_back(entity);

        // Add entity to vector of entities with corresponding tag in m_entityMap
        m_entityMap[entity->getTag()].push_back(entity);
    }
    
    m_entitiesToAdd.clear();

    // Removing entities
    // Remove the dead entities from vector of all entities
    removeDeadEntities(m_entities);

    // Remove dead entities from each vector in the entity map
    for (auto& [tag, entityVector] : m_entityMap)
    {
        removeDeadEntities(entityVector);
    }
}

const EntityVector& EntityManager::getEntities()
{
    return m_entities;
}

const EntityVector& EntityManager::getEntities(const std::string& tag)
{
    return m_entityMap[tag];
}

const std::map<std::string, EntityVector>& EntityManager::getEntityMap()
{
    return m_entityMap;
}