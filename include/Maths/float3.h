#pragma once

#include <Generic/Utilities.h>

namespace eloo::Math {
class float3;
static float3 normalize(const float3& v);
static float3 fastNormalize(const float3& v, unsigned int iterationCount = 1);
static bool isNormalized(const float3& v);
static float dot(const float3& v1, const float3& v2);
static float3 cross(const float3& v1, const float3& v2);
static float distance(const float3& v1, const float3& v2);
static float distanceSqr(const float3& v1, const float3& v2);
static float angle(const float3& v1, const float3& v2);
static float3 reflect(const float3& v, const float3& normal);
static float3 project(const float3& v, const float3& normal);
static float3 rotate(const float3& toRotate, const float3& axis, float radians);
static float3 lerp(const float3& start, const float3& end, float t);

class float3 {
public:
    inline constexpr float3() : x(0.0f), y(0.0f), z(0.0f) {}
    inline constexpr float3(float xyz) : x(xyz), y(xyz), z(xyz) {}
    inline constexpr float3(float x, float y, float z) : x(x), y(y), z(z) {}

    inline float* elements()                                                { return &x; }
    inline const float* elements() const                                    { return &x; }

    inline static constexpr float3 zero()                                   { return { 0.0f }; }
    inline static constexpr float3 one()                                    { return { 1.0f }; }
    inline static constexpr float3 up()                                     { return { 0.0f, 1.0f, 0.0f }; }
    inline static constexpr float3 right()                                  { return { 1.0f, 0.0f, 0.0f }; }
    inline static constexpr float3 forward()                                { return { 0.0f, 0.0f, 1.0f }; }

    float magnitude() const;
    float fastMagnitude(unsigned int iterationCount = 1) const;
    inline float magnitudeSqr() const                                       { return x * x + y * y + z * z; }

    inline bool isNormalized() const                                        { return Math::isNormalized(*this); }
    inline float3& fastNormalize(unsigned int iterationCount = 1)           { return (*this = Math::fastNormalize(*this, iterationCount)); }
    inline float3& normalize()                                              { return (*this = Math::normalize(*this)); }
    inline float3& reflect(const float3& normal)                            { return (*this = Math::reflect(*this, normal)); }
    inline float3& project(const float3& normal)                            { return (*this = Math::project(*this, normal)); }
    inline float3& rotate(const float3& axis, float radians)                { return (*this = Math::rotate(*this, axis, radians)); }


    //////////////////////////////////////////////
    // Swizzles

    SWIZZLE_FLOAT3


    //////////////////////////////////////////////
    // Accessors

    inline float& operator [] (uint8_t index)                               { return elements()[index]; }
    inline const float& operator [] (uint8_t index) const                   { return elements()[index]; }


    //////////////////////////////////////////////
    // Operator overloads

    inline friend bool operator != (const float3& lhs, const float3& rhs)   { return lhs.x != rhs.x && lhs.y != lhs.y && lhs.z != rhs.z; }
    inline friend bool operator == (const float3& lhs, const float3& rhs)   { return !(lhs != rhs); }

    inline friend float3 operator - (const float3& v)                       { return { -v.x, -v.y, -v.z }; }
    inline float3& operator - ()                                            { return (*this = -*this); }

    inline friend float3 operator - (const float3& lhs, const float3& rhs)  { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }
    inline friend float3 operator - (const float3& v, float f)              { return { v.x - f, v.y - f, v.z - f }; }
    inline float3& operator -= (const float3& v)                            { return (*this = *this - v); }
    inline float3& operator -= (float f)                                    { return (*this = *this - f); }

    inline friend float3 operator + (const float3& lhs, const float3& rhs)  { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
    inline friend float3 operator + (const float3& v, float f)              { return { v.x + f, v.y + f, v.z + f }; }
    inline float3& operator += (const float3& v)                            { return (*this = *this + v); }
    inline float3& operator += (float f)                                    { return (*this = *this + f); }

    inline friend float3 operator * (const float3& lhs, const float3& rhs)  { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z }; }
    inline friend float3 operator * (const float3& v, float f)              { return { v.x * f, v.y * f, v.z * f }; }
    inline float3& operator *= (float f)                                    { return (*this = *this * f); }
    inline float3& operator *= (float f)                                    { return (*this = *this * f); }

    inline friend float3 operator / (const float3& lhs, const float3& rhs)  { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z }; }
    inline friend float3 operator / (const float3& v, float f)              { return { v.x / f, v.y / f, v.z / f }; }
    inline float3& operator /= (float f)                                    { return (*this = *this / f); }
    inline float3& operator /= (float f)                                    { return (*this = *this / f); }

public:
    float x, y, z;
};
}; // namespace Math