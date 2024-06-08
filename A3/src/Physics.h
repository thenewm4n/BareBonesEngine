#pragma once

#include "Entity.h"
#include "Vec2.h"

class Physics
{
public:
    Vec2f getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    Vec2f getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};