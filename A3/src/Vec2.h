#pragma once

#include <cmath>

template <typename T>
class Vec2
{
public:
    T x = T();
    T y = T();

    Vec2();
    Vec2(T xIn, T yIn);

    bool operator == (const Vec2<T>& other) const;
    bool operator != (const Vec2<T>& other) const;

    Vec2<T> operator + (const Vec2<T>& other) const;
    Vec2<T> operator - (const Vec2<T>& other) const;
    Vec2<T> operator * (float scalar) const;            // Allows multiplication with scalar on right; see non-member function for scalar on left version
    Vec2<T> operator / (const float scalar) const;

    void operator += (const Vec2<T>& other);
    void operator -= (const Vec2<T>& other);
    void operator *= (const float scalar);
    void operator /= (const float scalar);

    float distance(const Vec2<T>& other) const;
    void normalise();
};


// Method implementations
template <typename T>
Vec2<T>::Vec2() {}

template <typename T>
Vec2<T>::Vec2(T xIn, T yIn)
    : x(xIn), y(yIn) {}

template <typename T>
Vec2<T> Vec2<T>::operator + (const Vec2<T>& other) const
{
    return Vec2<T>(x + other.x, y + other.y);
}

template <typename T>
Vec2<T> Vec2<T>::operator - (const Vec2<T>& other) const
{
    return Vec2<T>(x - other.x, y - other.y);
}

template <typename T>
Vec2<T> Vec2<T>::operator * (float scalar) const
{
    return Vec2<T>(x * scalar, y * scalar);
}

template <typename T>
Vec2<T> Vec2<T>::operator / (const float scalar) const
{
    return Vec2<T>(x / scalar, y / scalar);
}

template <typename T>
void Vec2<T>::operator += (const Vec2<T>& other)
{
    x += other.x;
    y += other.y;
}

template <typename T>
void Vec2<T>::operator -= (const Vec2<T>& other)
{
    x -= other.x;
    y -= other.y;
}

template <typename T>
void Vec2<T>::operator *= (const float scalar)
{
    x *= scalar;
    y *= scalar;
}

template <typename T>
void Vec2<T>::operator /= (const float scalar)
{
    x /= scalar;
    y /= scalar;
}

template <typename T>
bool Vec2<T>::operator == (const Vec2<T>& other) const
{
    return x == other.x && y == other.y;
}

template <typename T>
bool Vec2<T>::operator != (const Vec2<T>& other) const
{
    return !(*this == other);
}

template <typename T>
float Vec2<T>::distance(const Vec2<T>& other) const
{
    Vec2<T> diff = other - *this;
    
    return std::sqrtf(diff.x * diff.x + diff.y * diff.y);
}

template <typename T>
void Vec2<T>::normalise()
{
    // Distance distance from (0,0) to end of vector
    float length = distance(Vec2<T>(0, 0));
    *this /= length;
}


// NOTE: this is a non-member function
// Allows multiplication with scalar on left
template <typename T>
Vec2<T> operator * (float scalar, const Vec2<T>& vec)
{
    return Vec2<T>(vec.x * scalar, vec.y * scalar);
}


using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;