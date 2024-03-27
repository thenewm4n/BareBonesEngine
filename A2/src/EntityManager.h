#pragma once

#include <vector>
#include <map>

using EntityVector = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVector>;

class EntityManager
{
private:
    EntityVector m_entities;
    EntityVector m_entitiesToAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

private:
    void removeDeadEntities(EntityVector& vector);

public:
    EntityManager();
    void update();
    
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    
    const EntityVector& getEntities();
    const EntityVector& getEntities(const std::string& tag);
    const std::map<std::string, EntityVector>& getEntityMap();
}