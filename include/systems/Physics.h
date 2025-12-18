#pragma once

#include "entities/Entity.h"
#include "utils/Vec2.h"

namespace Physics
{
    Vec2f getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);             // Positive if overlap
    Vec2f getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    bool resolveCollision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);        // First Entity is moved out of the second; returns true if collision is in X direction
};