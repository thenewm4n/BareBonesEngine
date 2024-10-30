#pragma once

#include "Entity.h"

#include <vector>
#include <unordered_map>

using EntityVector = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::unordered_map<std::string, EntityVector>;

class EntityManager
{
public:
	EntityManager();

	/**
	* Adds and removes entities while avoiding iterator invalidation
	*/
	void update();
	std::shared_ptr<Entity> addEntity(const std::string& tag);

	EntityVector& getEntities();
	EntityVector& getEntities(const std::string& tag);
	const std::unordered_map<std::string, EntityVector>& getEntityMap();

private:
	EntityVector m_entities;
	EntityVector m_entitiesToAdd;
	EntityMap m_entityMap;
	size_t m_totalEntities = 0;

	void removeDeadEntities(EntityVector& vector);
};