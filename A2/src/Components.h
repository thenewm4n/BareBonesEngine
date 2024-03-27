#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public:
    Vec2 position = { 0.f, 0.f };
    Vec2 velocity = {0.f, 0.f};
    float angle = 0.f;

public:
    CTransform(const Vec2& pos, const Vec2& vel, float ang)
        : position(pos), velocity(vel), angle(ang) {}
};

class CShape
{
public:
    sf::CircleShape circle;

public:
    CShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float thickness)
        : circle(radius, points)
    {
        circle.setFillColor(color);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
    }
};

class CCollision
{
public:
    float radius = 0.f;

public:
    CCollision(float rad)
        : radius(rad) {}
};

class CScore
{
public:
    int score = 0;

public:
    CScore(int s)
        : score(s) {}
};

Class CLifespan
{
public:
    int remaining = 0;
    int total = 0;

public:
    CLifespan(int total)
        :remaining(total), total(total) {}
};

Class CInput
{
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;

public:
    CInput() {}
};