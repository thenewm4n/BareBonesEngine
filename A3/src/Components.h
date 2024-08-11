#pragma once

#include "Animation.h"

#include <iostream>

class Component
{
public:
    bool has = false;
};

enum class PlayerState
{
    Running,
    Standing,
    InAir,
    Shooting
};

class CTransform : public Component
{
public:
    Vec2f position = { 0.f, 0.f };
    Vec2f previousPosition = { 0.f, 0.f };
    Vec2f scale = { 1.f, 1.f };
    Vec2f velocity = { 0.f, 0.f };
    float angle = 0.f;

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
    float radius = 0.f;

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
        : mass(1.f) {}
    
    CBody(Vec2f size, float mass = 1.f)
        : bBox(size)
    {
        if (mass < 0.001f) { std::cout << "Mass is negative or zero." << std::endl; }
        if (mass > 1.f) { std::cout << "Mass is greater than 1." << std::endl; }
    }
    
    /*
    CBody(float radius, float mass = 1.f)
        : bCircle(radius)
    {
        if (mass < 0.001f) { std::cout << "Mass is negative or zero." << std::endl; }
        if (mass > 1.f) { std::cout << "Mass is greater than 1." << std::endl; }
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
    PlayerState currentState = PlayerState::Standing;
    PlayerState previousState;
    bool isFacingRight = false;

    CState() {}
    CState(PlayerState state)
        : currentState(state) {}
};

class CAnimation : public Component     // For textures and animations
{
public:
    Animation animation;
    bool toRepeat = false;

    CAnimation() {}
    CAnimation(const Animation& animation, bool toRepeat)
        : animation(animation), toRepeat(toRepeat) {}
};

class CGravity : public Component
{
public:
    float acceleration = 9.8;

    CGravity() {}
    CGravity(float accel)
        : acceleration(accel) {}
};