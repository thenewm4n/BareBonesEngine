#pragma once

#include "systems/Animation.h"

#include <iostream>

class Component
{
public:
    bool has = false;
};

enum class PlayerState
{
    Idle,
    Running,
    Jumping,
    Falling,
    Shooting
};

class CTransform : public Component
{
public:
    Vec2f position = { 0.0f, 0.0f };
    Vec2f previousPosition = { 0.0f, 0.0f };
    Vec2f scale = { 1.0f, 1.0f };
    Vec2f velocity = { 0.0f, 0.0f };
    float angle = 0.0f;

    CTransform() {}
    CTransform(const Vec2f& pos)
        : position(pos) {}
    CTransform(const Vec2f& pos, const Vec2f& vel, float ang)
        : position(pos), previousPosition(pos), velocity(vel), angle(ang) {}
};

class CBoundingBox : public Component
{
public:
    Vec2f size;

    CBoundingBox() {}
    CBoundingBox(const Vec2f& size)
        : size(size) {}
};

class CBoundingCircle : public Component
{
public:
    float radius = 0.0f;

    CBoundingCircle() {}
    CBoundingCircle(float rad)
        : radius(rad) {}
};

class CBody : public Component
{
public:
    CBoundingBox bBox;
    // CBoundingCircle bCircle;
    float mass;         // 0 < mass <= 1

    CBody()
        : mass(1.0f) {}
    
    CBody(Vec2f size, float mass = 1.0f)
        : bBox(size), mass(mass)
    {
        if (mass < 0.001f)
        {
            std::cout << "Mass is negative or zero." << std::endl;
            this->mass = 0.001f;
        }
        if (mass > 1.0f) 
        {
            std::cout << "Mass is greater than 1." << std::endl;
			this->mass = 1.0f;
        }
    }
    
    /*
    CBody(float radius, float mass = 1.0f)
        : bCircle(radius)
    {
        if (mass < 0.001f) { std::cout << "Mass is negative or zero." << std::endl; }
        if (mass > 1.0f) { std::cout << "Mass is greater than 1." << std::endl; }
    }
    */
};

class CLifespan : public Component
{
public:
    int framesDuration = 0;
    int frameCreated = 0;

    CLifespan() {}
    CLifespan(int duration, int frame)
        : framesDuration(duration), frameCreated(frame) {}
};

class CInput : public Component
{
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;
    bool canJump = true;
    bool canShoot = true;

    CInput() {}
};

class CState : public Component
{
public:
    PlayerState currentState;
    PlayerState previousState = PlayerState::Idle;
    bool isFacingRight = false;

    CState()
        : currentState(PlayerState::Idle) {}
    CState(PlayerState state)
        : currentState(state) {}

    std::string toString(bool usePrevious)
    {
        PlayerState state = usePrevious ? previousState : currentState;

        switch (state)
        {
            case PlayerState::Idle: return "Idle";
            case PlayerState::Running: return "Running";
            case PlayerState::Jumping: return "Jumping";
            case PlayerState::Falling: return "Falling";
            case PlayerState::Shooting: return "Shooting";
            default: return "Unknown";
        }
    }
};

class CAnimation : public Component     // For textures and animations
{
public:
    Animation animation;
    bool toRepeat = false;

    CAnimation() {}
    CAnimation(const Animation& animation, bool toRepeat)
        : animation(animation), toRepeat(toRepeat) {}

    bool update()
    {
        animation.update();

        if (animation.hasEnded())
        {
            if (toRepeat)
            {
                animation.reset();
            }
            else
            {
                return true;
            }
        }

        return false;
    }
};

class CGravity : public Component
{
public:
    float acceleration;

    CGravity()
        : acceleration(1) {}
    CGravity(float accel)
        : acceleration(accel) {}
};

class CParallax : public Component
{
public:
    unsigned int layerIndex;

    CParallax(unsigned int index = 0)
        : layerIndex(index) {}
};