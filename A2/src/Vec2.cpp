#include "Vec2.h"
#include <cmath>

Vec2::Vec2()
{
}

Vec2::Vec2(float xIn, float yIn)
    : x(xIn), y(yIn)
{
}

Vec2::Vec2(const sf::Vector2f& vec)
    : x(vec.x), y(vec.y)
{
}

Vec2::Vec2(const sf::Vector2i& vec)
    : x(vec.x), y(vec.y)
{
}

Vec2 Vec2::operator + (const Vec2& other) const
{
    return Vec2(this->x + other.x, this->y + other.y);
}

Vec2 Vec2::operator - (const Vec2& other) const
{
    return Vec2(this->x - other.x, this->y - other.y);
}

Vec2 Vec2::operator * (float scalar) const
{
    return Vec2(this->x * scalar, this->y * scalar);
}

// Note - not a member function
Vec2 operator * (float scalar, const Vec2& vec)
{
    return Vec2(vec.x * scalar, vec.y * scalar);
}

Vec2 Vec2::operator / (const float scalar) const
{
    return Vec2(this->x / scalar, this->y / scalar);
}

bool Vec2::operator == (const Vec2& other) const
{
    if (this->x == other.x && this->y == other.y)
        return true;
    
    return false;
}

bool Vec2::operator != (const Vec2& other) const
{
    if (this->x == other.x && this->y == other.y)
        return false;
    
    return true;
}

void Vec2::operator += (const Vec2& other)
{
    this->x += other.x;
    this->y += other.y;
}

void Vec2::operator -= (const Vec2& other)
{
    this->x -= other.x;
    this->y -= other.y;
}

void Vec2::operator *= (const float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
}

void Vec2::operator /= (const float scalar)
{
    this->x /= scalar;
    this->y /= scalar;
}

float Vec2::distance(const Vec2& other) const
{
    Vec2 diff = other - *this;
    
    return sqrtf(diff.x * diff.x + diff.y * diff.y);
}

void Vec2::normalise()
{
    // Distance distance from (0,0) to end of vector
    float length = distance(Vec2(0, 0));
    // float length = sqrtf(this->x * this->x + this->y * this->y);
    *this /= length;
}