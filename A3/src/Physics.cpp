#include "Physics.h"

#include "Components.h"

// Anonymous namespace -> essentially "private" helper function
namespace
{
    // Helper function to avoid code repetition
    Vec2f getOverlapHelper(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b, bool usePrevious)
    {
        if (a->hasComponent<CBody>() && b->hasComponent<CBody>() && a->hasComponent<CTransform>() && b->hasComponent<CTransform>())
        {
            const auto& aTransform = a->getComponent<CTransform>();
            const auto& bTransform = b->getComponent<CTransform>();

            const Vec2f& aPosition = usePrevious ? aTransform.previousPosition : aTransform.position;
            const Vec2f& bPosition = usePrevious ? bTransform.previousPosition : bTransform.position;
            
            const Vec2f aHalfSize = a->getComponent<CBody>().bBox.size / 2.f;
            const Vec2f bHalfSize = b->getComponent<CBody>().bBox.size / 2.f;

            Vec2f overlap;

            float xDiff = std::abs(aPosition.x - bPosition.x);
            overlap.x = aHalfSize.x + bHalfSize.x - xDiff;
            
            float yDiff = std::abs(aPosition.y - bPosition.y);
            overlap.y = aHalfSize.y + bHalfSize.y - yDiff;

            return overlap;
        }

        return Vec2f(0.f, 0.f);
    }

    // Axis-agnostic move function to avoid repeated code
    void moveEntity(float& aPosition, float& bPosition, float aHalfSize, float bHalfSize, float aVelocity)
    {
        // If a was the moving object, move it, else move b
        if (aVelocity != 0)
        {
            aPosition += aPosition > bPosition ? aHalfSize : -aHalfSize;
        }
        else
        {
            bPosition += bPosition > aPosition ? bHalfSize : -bHalfSize;
        }
    }
}


// "Public" functions
namespace Physics
{
    Vec2f getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        return getOverlapHelper(a, b, false);
    }

    Vec2f getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        return getOverlapHelper(a, b, true);
    }

    void resolveCollision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        // Resolve collision by seeing whether overlap is new in x direction or y direction
        Vec2f previousOverlap = Physics::getPreviousOverlap(a, b);

        auto& aTransform = a->getComponent<CTransform>();
        auto& bTransform = b->getComponent<CTransform>();
        const Vec2f aHalfSize = a->getComponent<CBody>().bBox.size / 2.f;
        const Vec2f bHalfSize = b->getComponent<CBody>().bBox.size / 2.f;

        // If new X overlap, resolve in X direction, else resolve in Y direction
        if (previousOverlap.x < 0)
        {
            moveEntity(aTransform.position.x, bTransform.position.x, aHalfSize.x, bHalfSize.x, aTransform.velocity.x);
        }
        else
        {
            moveEntity(aTransform.position.y, bTransform.position.y, aHalfSize.y, bHalfSize.y, aTransform.velocity.y);
        }
    }
}