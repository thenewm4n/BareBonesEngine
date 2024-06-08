#include "Components.h"
#include "Physics.h"

Vec2f Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return overlap rectangle size of the bounding boxes of a and b

        // Check both have bounding boxes
        // Check both have transforms

    return Vec2f(0.f, 0.f);
}

Vec2f Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return overlap rectangle size of bounding boxes of a and b from previous frame i.e. uses previous position

    return Vec2f(0.f, 0.f);
}