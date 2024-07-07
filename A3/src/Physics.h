#pragma once

#include "Entity.h"
#include "Vec2.h"

class Physics
{
public:
    Vec2f getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);     // Positive if overlap
    Vec2f getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

private:
    Vec2f getOverlapHelper(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b, bool usePrevious);     // To avoid code repetition
};