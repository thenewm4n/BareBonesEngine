#include "Physics.h"

#include "Components.h"

Vec2f Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    return getOverlapHelper(a, b, false);
}

Vec2f Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    return getOverlapHelper(a, b, true);
}

Vec2f Physics::getOverlapHelper(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b, bool usePrevious)
{
    if (a->hasComponent<CBoundingBox>() && b->hasComponent<CBoundingBox>() && a->hasComponent<CTransform>() && b->hasComponent<CTransform>())
    {
        const auto& aTransform = a->getComponent<CTransform>();
        const auto& bTransform = b->getComponent<CTransform>();

        const Vec2f& aPosition = usePrevious ? aTransform.previousPosition : aTransform.position;
        const Vec2f& bPosition = usePrevious ? bTransform.previousPosition : bTransform.position;
        
        const Vec2f aHalfSize = a->getComponent<CBoundingBox>().size / 2.f;
        const Vec2f bHalfSize = b->getComponent<CBoundingBox>().size / 2.f;

        Vec2f overlap;

        float xDiff = std::abs(aPosition.x - bPosition.x);
        overlap.x = aHalfSize.x + bHalfSize.x - xDiff;
        
        float yDiff = std::abs(aPosition.y - bPosition.y);
        overlap.y = aHalfSize.y + bHalfSize.y - yDiff;

        return overlap;
    }

    return Vec2f(0.f, 0.f);
}