#pragma once

#include "Components.h"
#include <memory>
#include <string>

class Entity
{
    friend class EntityManager;

private:
    bool m_alive = true;
    size_t m_id = 0;
    std::string m_tag = "Default";

private:
    Entity(constr size_t if, const std::string& tag);

public:
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifespan> cLifespan;

public:
    bool isAlive() const;
    const std::string& getTag() const;
    const size_t getId() const;
    void destroy();
};