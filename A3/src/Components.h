#pragma once

#include "Animation.h"


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

class CCollision : public Component
{
public:
    float radius = 0.f;

    CCollision() {}
    CCollision(float rad)
        : radius(rad) {}
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

class CBoundingBox : public Component
{
public:
    Vec2f size;
    Vec2f halfSize;

    CBoundingBox() {}
    CBoundingBox(const Vec2f& size)
        : size(size), halfSize(size / 2) {}
};

class CGravity : public Component
{
public:
    float acceleration = 9.8;

    CGravity() {}
    CGravity(float accel)
        : acceleration(accel) {}
};