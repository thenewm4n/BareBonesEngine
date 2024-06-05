#include "Components.h"
#include "Physics.h"

Vec2 Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return overlap rectangle size of the bounding boxes of a and b

        // Check both have bounding boxes
        // Check both have transforms

    return Vec2(0, 0);
}

Vec2 Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return overlap rectangle size of bounding boxes of a and b from previous frame i.e. uses previous position

    return Vec2(0, 0);
}