#pragma once

#include <vector>
#include <map>
#include "Entity.h"

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
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	void removeDeadEntities(EntityVector& vector);

public:
	EntityManager();
	void update();

	const EntityVector& getEntities();
	const EntityVector& getEntities(const std::string& tag);
	const std::map<std::string, EntityVector>& getEntityMap();
}