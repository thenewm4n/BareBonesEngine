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
            const auto& transformA = a->getComponent<CTransform>();
            const auto& transformB = b->getComponent<CTransform>();

            const Vec2f& positionA = usePrevious ? aTransform.previousPosition : aTransform.position;
            const Vec2f& positionB = usePrevious ? bTransform.previousPosition : bTransform.position;
            
            const Vec2f halfSizeA = a->getComponent<CBody>().bBox.size / 2.f;
            const Vec2f halfSizeB = b->getComponent<CBody>().bBox.size / 2.f;

            Vec2f overlap;

            float diffX = std::abs(positionA.x - positionB.x);
            overlap.x = halfSizeA.x + halfSizeB.x - diffX;
            
            float diffY = std::abs(positionA.y - positionB.y);
            overlap.y = halfSizeA.y + halfSizeB.y - diffY;

            return overlap;
        }

        return Vec2f(0.f, 0.f);
    }

    // Axis-agnostic move function to avoid repeated code
    /*
    void moveEntity(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b, bool isXDirection, bool aIsFaster)
    {   
        // Don't we want to correct by halfsize - overlap?

        float& positionA = isXDirection ? a->getComponent<CTransform>().position.x : a->getComponent<CTransform>().position.y;
        float& positionB = isXDirection ? b->getComponent<CTransform>().position.x : b->getComponent<CTransform>().position.y;
        float overlap = isXDirection ? Physics::getOverlap(a, b).x : Physics::getOverlap(a, b).y;

        // const float halfSizeA = isXDirection ? a->getComponent<CBody>().bBox.size.x / 2.f : a->getComponent<CBody>().bBox.size.y;
        // const float halfSizeB = isXDirection ? b->getComponent<CBody>().bBox.size.x / 2.f : b->getComponent<CBody>().bBox.size.y;

        float massA = a->getComponent<CBody>().mass;
        float massB = b->getComponent<CBody>().mass;

        // Calculate direction
        int direction = positionA > positionB ? 1 : -1;

        // Helper function to move a position
        auto movePosition = [&](float& position, float overlap, int dir)
        {
            position += dir * overlap;
        };

        // Move the entity based on mass and speed
        if (massA > massB)          // If a is heavier, move b
        {
            movePosition(positionB, overlap, direction);        // overlap was halfSizeB
        }
        else if (massA < massB)     // If b is heavier, move a
        {
            movePosition(positionA, overlap, -direction);       // overlap was halfSizeA
        }   
        else
        {                           // if masses are equal, move the faster entity
            if (aIsFaster)
            {
                movePosition(positionA, overlap, -direction);   // overlap was halfSizeA
            }
            else
            {
                movePosition(positionB, overlap, direction);  // overlap was halfSizeB
            }
        }
    }
    */

    void movePlayer(std::shared_ptr<Entity> player, std::shared_ptr<Entity> object, bool isXDirection)
    {
        // !!! the y positions of player and object need to be windowSize.y - position.y

        float& posPlayer = isXDirection ? player->getComponent<CTransform>().position.x : player->getComponent<CTransform>().position.y;
        float& posObject = isXDirection ? object->getComponent<CTransform>().position.x : object->getComponent<CTransform>().position.y;
        float overlap = isXDirection ? Physics::getOverlap(player, object).x : Physics::getOverlap(player, object).y;

        // Calculate direction
        int direction = posPlayer > posObject ? 1 : -1;

        // Move the player in correct direction
        posPlayer += direction * overlap;
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

    /*
    void resolveCollision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        // Resolve collision by seeing whether overlap is new in x direction or y direction
        Vec2f previousOverlap = Physics::getPreviousOverlap(a, b);

        Vec2f velocityA = a->getComponent<CTransform>().velocity;
        Vec2f velocityB = b->getComponent<CTransform>().velocity;

        // If new X overlap, resolve in X direction, else resolve in Y direction
        if (previousOverlap.x < 0)
        {
            moveEntity(a, b, true, std::abs(velocityA.x) > std::abs(velocityB.x));
        }
        else
        {
            moveEntity(a, b, false, std::abs(velocityA.y) > std::abs(velocityB.y));
        }
    }
    */

    void resolveCollision(std::shared_ptr<Entity> player, std::shared_ptr<Entity> object)
    {
        Vec2f previousOverlap = Physics::getPreviousOverlap(a, b);
        
        // This function is only called there's overlap in both x and y directions;
        // if previous x overlap is negative, x overlap is new
        bool isXDirection = previousOverlap.x < 0;

        moveEntity(a, b, isXDirection);
    }
}