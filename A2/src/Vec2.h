#pragma once

class Vec2
{
public:
    float x = 0.f;
    float y = 0.f;

public:
    Vec2();
    Vec2(float x, float y);

    bool operator == (const Vec2& other) const;
    bool operator != (const Vec2& other) const;

    Vec2 operator + (const Vec2& other) const;
    Vec2 operator - (const Vec2& other) const;
    Vec2 operator * (const float scalar) const;
    Vec2 operator / (const float scalar) const;

    void operator += (const Vec2& other);
    void operator -= (const Vec2& other);
    void operator *= (const float scalar);
    void operator /= (const float scalar);

    float distance(const Vec2& other) const;
};