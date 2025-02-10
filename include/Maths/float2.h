#pragma once

#include <Generic/Utilities.h>

namespace eloo::Math {
class float2;

float2 normalize(const float2& v);
float2 fastNormalize(const float2& v, unsigned int iterationCount = 1);
bool isNormalized(const float2& v);
float dot(const float2& v1, const float2& v2);
float distance(const float2& v1, const float2& v2);
float angle(const float2& v1, const float2& v2);
float2 reflect(const float2& v, const float2& normal);
float2 project(const float2& v, const float2& normal);
float2 rotate(const float2& v, float radians);
float2 lerp(const float2& start, const float2& end, float t);

class float2 {
public:
    inline constexpr float2() : x(0.0f), y(0.0f) {}
    inline constexpr float2(float xy) : x(xy), y(xy) {}
    inline constexpr float2(float x, float y) : x(x), y(y) {}

    inline float* elements()                                        { return &x; }
    inline const float* elements() const                            { return &x; }

    inline static constexpr float2 zero()                           { return { 0.0f }; }
    inline static constexpr float2 one()                            { return { 1.0f }; }
    inline static constexpr float2 up()                             { return { 0.0f, 1.0f }; }
    inline static constexpr float2 right()                          { return { 1.0f, 0.0f }; }

    float magnitude() const;
    float fastMagnitude(unsigned int iterationCount = 1) const;
    inline float magnitudeSqr() const                               { return x * x + y * y; }

    inline bool isNormalized() const                                { return Math::isNormalized(*this); }
    inline float2& normalize()                                      { return (*this = Math::normalize(*this)); }
    inline float2& fastNormalize(unsigned int iterationCount = 1)   { return (*this = Math::fastNormalize(*this, iterationCount)); }
    inline float2& reflect(const float2& normal)                    { return (*this = Math::reflect(*this, normal)); }
    inline float2& project(const float2& normal)                    { return (*this = Math::project(*this, normal)); }
    inline float2& rotate(float radians)                            { return (*this = Math::rotate(*this, radians)); }


    //////////////////////////////////////////////
    // Swizzles

    SWIZZLE_FLOAT2


    //////////////////////////////////////////////
    // Accessors

    inline float& operator [] (uint8_t index)                               { return elements()[index]; }
    inline const float& operator [] (uint8_t index) const                   { return elements()[index]; }


    //////////////////////////////////////////////
    // Operator overloads

    inline friend bool operator != (const float2& lhs, const float2& rhs)   { return lhs.x != rhs.x && lhs.y != lhs.y; }
    inline friend bool operator == (const float2& lhs, const float2& rhs)   { return !(lhs != rhs); }

    inline friend float2 operator - (const float2& v)                       { return { -v.x, -v.y }; }
    inline float2& operator - ()                                            { return (*this = -*this); }

    inline friend float2 operator - (const float2& lhs, const float2& rhs)  { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
    inline friend float2 operator - (const float2& v, float f)              { return { v.x - f, v.y - f }; }
    inline float2& operator -= (const float2& v)                            { return (*this = *this - v); }
    inline float2& operator -= (float f)                                    { return (*this = *this - f); }

    inline friend float2 operator + (const float2& lhs, const float2& rhs)  { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
    inline friend float2 operator + (const float2& v, float f)              { return { v.x + f, v.y + f }; }
    inline float2& operator += (const float2& v)                            { return (*this = *this + v); }
    inline float2& operator += (float f)                                    { return (*this = *this + f); }

    inline friend float2 operator * (const float2& lhs, const float2& rhs)  { return { lhs.x * rhs.x, lhs.y * rhs.y }; }
    inline friend float2 operator * (const float2& v, float f)              { return { v.x * f, v.y * f }; }
    inline float2& operator *= (const float2& v)                            { return (*this = *this * v); }
    inline float2& operator *= (float f)                                    { return (*this = *this * f); }

    inline friend float2 operator / (const float2& lhs, const float2& rhs)  { return { lhs.x / rhs.x, lhs.y / rhs.y }; }
    inline friend float2 operator / (const float2& v, float f)              { return { v.x / f, v.y / f }; }
    inline float2& operator /= (const float2& v)                            { return (*this = *this / v); }
    inline float2& operator /= (float f)                                    { return (*this = *this / f); }

public:
    float x, y;
};
}; // namespace eloo::Math