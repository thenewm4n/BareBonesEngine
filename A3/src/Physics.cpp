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
            
            const Vec2f halfSizeA = a->getComponent<CBody>().bBox.size / 2.f;
            const Vec2f halfSizeB = b->getComponent<CBody>().bBox.size / 2.f;

            Vec2f overlap;

            float diffX = std::abs(positionA.x - positionB.x);
            overlap.x = halfSizeA.x + halfSizeB.x - diffX;
            
            float diffY = std::abs(positionA.y - positionB.y);
            overlap.y = halfSizeA.y + halfSizeB.y - diffY;

            return overlap;
        }

        std::cerr << "Physics.cpp: one or both entities lack a CBody or CTransform." << std::endl;
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

    /* ORIGINAL
    void movePlayer(std::shared_ptr<Entity> player, std::shared_ptr<Entity> object, bool isXDirection)
    {
        // !!! the y positions of player and object need to be windowSize.y - position.y

        float& posPlayer = isXDirection ? player->getComponent<CTransform>().position.x : player->getComponent<CTransform>().position.y;
        float& posObject = isXDirection ? object->getComponent<CTransform>().position.x : object->getComponent<CTransform>().position.y;
        float overlap = isXDirection ? Physics::getOverlap(player, object).x : Physics::getOverlap(player, object).y;

        if (isXDirection)
        {
            std::cout << "!X COLLISION!" << std::endl;
			std::cout << "Overlap: " << overlap << std::endl;
            std::cout << "Actual player position: " << player->getComponent<CTransform>().position.x << ' ' << player->getComponent<CTransform>().position.y << std::endl;
            std::cout << "Actual object position: " << object->getComponent<CTransform>().position.x << ' ' << object->getComponent<CTransform>().position.y << std::endl;
            std::cout << "Calculated player position: " << posPlayer << std::endl;
            std::cout << "Calculated object position: " << posObject << std::endl;
        }

        // Calculate direction
        int direction = posPlayer > posObject ? 1 : -1;

		if (isXDirection)
		{
			std::cout << "Direction: " << direction << std::endl;
		}

        // Move the player in correct direction
        posPlayer += direction * overlap;

        if (isXDirection)
        {
            std::cout << "New actual player position: " << player->getComponent<CTransform>().position.x << ' ' << player->getComponent<CTransform>().position.y << std::endl;
            std::cout << "New calculated player position X: " << posPlayer << std::endl;

			std::cout << "New overlap: " << Physics::getOverlap(player, object).x << std::endl;
        }
    }
    */

    void movePlayer(std::shared_ptr<Entity> player, std::shared_ptr<Entity> object, bool isXDirection)
    {
		float& posPlayer = isXDirection ? player->getComponent<CTransform>().position.x : player->getComponent<CTransform>().position.y;
		float& posObject = isXDirection ? object->getComponent<CTransform>().position.x : object->getComponent<CTransform>().position.y;
		float overlap = isXDirection ? Physics::getOverlap(player, object).x : Physics::getOverlap(player, object).y;
        

        if (isXDirection)
        {
            // Debug output to verify overlap and positions
            std::cout << "Initial posPlayer: " << posPlayer << std::endl;
            std::cout << "Initial posObject: " << posObject << std::endl;
            std::cout << "Overlap: " << overlap << std::endl;
        }
        
		// Determine whether to push positive (down and right) or negative (up and left)
		int direction = posPlayer > posObject ? 1 : -1;

        posPlayer += direction * overlap;

        if (isXDirection)
        {
            // Debug output to verify new position
            std::cout << "New posPlayer: " << posPlayer << std::endl;
			std::cout << "New overlap: " << Physics::getOverlap(player, object).x << std::endl;
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
        // This function is only called there's overlap in both x and y directions;
        // if previous x overlap is negative, x overlap is new

        const Vec2f currentOverlap = Physics::getOverlap(player, object);
        const Vec2f previousOverlap = Physics::getPreviousOverlap(player, object); 

        bool isXDirection = previousOverlap.x < 0;       // !!! This is where the problem is: because a collision the loop after a collision, the previous overlap is positive
        
		movePlayer(player, object, isXDirection);

        // If collision in y direction, set velocity to 0 and handle according to direction
        if (!isXDirection)
        {
            // Whether collision from above or below, set y velocity to 0
            player->getComponent<CTransform>().velocity.y = 0.f;

            // If player's previous position was less (i.e. closer to top of screen), collision from above
            bool isFromAbove = player->getComponent<CTransform>().previousPosition.y < object->getComponent<CTransform>().previousPosition.y;

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
}