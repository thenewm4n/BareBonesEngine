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
            const Vec2f positionA = usePrevious ? a->getComponent<CTransform>().previousPosition : a->getComponent<CTransform>().position;
            const Vec2f positionB = usePrevious ? b->getComponent<CTransform>().previousPosition : b->getComponent<CTransform>().position;
            
            const Vec2f halfSizeA = a->getComponent<CBody>().bBox.size / 2.0f;
            const Vec2f halfSizeB = b->getComponent<CBody>().bBox.size / 2.0f;

            Vec2f overlap;

            float diffX = std::abs(positionA.x - positionB.x);
            overlap.x = halfSizeA.x + halfSizeB.x - diffX;
            
            float diffY = std::abs(positionA.y - positionB.y);
            overlap.y = halfSizeA.y + halfSizeB.y - diffY;

            return overlap;
        }

        std::cerr << "Physics.cpp: one or both entities lack a CBody or CTransform." << std::endl;
        return Vec2f(0.0f, 0.0f);
    }

    // Axis-agnostic move function to avoid repeated code
    void movePlayer(std::shared_ptr<Entity> player, std::shared_ptr<Entity> object, bool isXDirection)
    {
        float& posPlayer = isXDirection ? player->getComponent<CTransform>().position.x : player->getComponent<CTransform>().position.y;
        float& prevPosPlayer = isXDirection ? player->getComponent<CTransform>().previousPosition.x : player->getComponent<CTransform>().previousPosition.y;
        float posObject = isXDirection ? object->getComponent<CTransform>().position.x : object->getComponent<CTransform>().position.y;
        float overlap = isXDirection ? Physics::getOverlap(player, object).x : Physics::getOverlap(player, object).y;

        // Move player in correct direction
        int direction = posPlayer > posObject ? 1 : -1;
        posPlayer += direction * overlap;

        // Previous position for the next frame is now the position after resolution
        prevPosPlayer = posPlayer;
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
    void resolveCollision(std::shared_ptr<Entity> player, std::shared_ptr<Entity> object)
    {
        // This function is only called there's overlap in both x and y directions;
        // if previous x overlap is negative, x overlap is new

        const Vec2f previousOverlap = Physics::getPreviousOverlap(player, object); 
        bool isXDirection = previousOverlap.x <= 0 && previousOverlap.y > 0;

        movePlayer(player, object, isXDirection);
		
        // If collision in y direction, set velocity to 0 and handle according to direction
        if (!isXDirection)
        {
            // Whether collision from above or below, set y velocity to 0
            player->getComponent<CTransform>().velocity.y = 0.0f;

            // If player's previous position was less (i.e. closer to top of screen), collision from above
            bool isFromAbove = player->getComponent<CTransform>().previousPosition.y > object->getComponent<CTransform>().previousPosition.y;

            // If collision is in y direction and is from above, reset player y velocity (sets player animation to standing) and allow jumping
            if (isFromAbove)
            {
                player->getComponent<CInput>().canJump = true;
            }
            else
            {
                // TODO: Handle question mark collision here?
            }
        }
    }
    */

    bool resolveCollision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
    {
        // This function is only called there's overlap in both x and y directions;
        // if previous x overlap is negative, x overlap is new

        const Vec2f previousOverlap = Physics::getPreviousOverlap(a, b);
        bool isXDirection = previousOverlap.x <= 0 && previousOverlap.y > 0;

        movePlayer(a, b, isXDirection);

        return isXDirection;
    }
}