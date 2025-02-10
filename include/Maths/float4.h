#pragma once

#include <Generic/Utilities.h>

namespace eloo::Math {
class float4;
static float4 normalize(const float4& v);
static float4 fastNormalize(const float4& v, unsigned int iterationCount = 1);
static bool isNormalized(const float4& v);
static float dot(const float4& v1, const float4& v2);
static float4 cross(const float4& v1, const float4& v2);
static float distance(const float4& v1, const float4& v2);
static float distanceSqr(const float4& v1, const float4& v2);
static float angle(const float4& v1, const float4& v2);
static float4 reflect(const float4& v, const float4& normal);
static float4 project(const float4& v, const float4& normal);
static float4 rotate(const float4& toRotate, const float4& axis, float radians);
static float4 lerp(const float4& start, const float4& end, float t);

class float4 {
public:
    inline constexpr float4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    inline constexpr float4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
    inline constexpr float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    inline constexpr float4(const float3& xyz, float w = 0.0f) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

    inline float* elements()                                                { return &x; }
    inline const float* elements() const                                    { return &x; }

    inline static constexpr float4 zero()                                   { return { 0.0f }; }
    inline static constexpr float4 one()                                    { return { 1.0f }; }

    float magnitude() const;
    float fastMagnitude(unsigned int iterationCount = 1) const;
    inline float magnitudeSqr() const                                       { return x * x + y * y + z * z + w * w; }

    inline bool isNormalized() const                                        { return Math::isNormalized(*this); }
    inline float4& fastNormalize(unsigned int iterationCount = 1)           { return (*this = Math::fastNormalize(*this, iterationCount)); }
    inline float4& normalize()                                              { return (*this = Math::normalize(*this)); }
    inline float4& reflect(const float4& normal)                            { return (*this = Math::reflect(*this, normal)); }
    inline float4& project(const float4& normal)                            { return (*this = Math::project(*this, normal)); }
    inline float4& rotate(const float4& axis, float radians)                { return (*this = Math::rotate(*this, axis, radians)); }


    //////////////////////////////////////////////
    // Swizzles

    SWIZZLE_FLOAT4


    //////////////////////////////////////////////
    // Accessors

    inline float& operator [] (uint8_t index)                               { return elements()[index]; }
    inline const float& operator [] (uint8_t index) const                   { return elements()[index]; }


    //////////////////////////////////////////////
    // Operator overloads

    inline friend bool operator != (const float4& lhs, const float4& rhs)   { return lhs.x != rhs.x && lhs.y != lhs.y && lhs.z != lhs.z && lhs.w != lhs.w; }
    inline friend bool operator == (const float4& lhs, const float4& rhs)   { return !(lhs != rhs); }

    inline friend float4 operator - (const float4& v)                       { return { -v.x, -v.y, -v.z, -v.w }; }
    inline float4& operator - ()                                            { return (*this = -*this); }

    inline friend float4 operator - (const float4& lhs, const float4& rhs)  { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w }; }
    inline friend float4 operator - (const float4& v, float f)              { return { v.x - f, v.y - f, v.z - f, v.w - f }; }
    inline float4& operator -= (const float4& v)                            { return (*this = *this - v); }
    inline float4& operator -= (float f)                                    { return (*this = *this - f); }

    inline friend float4 operator + (const float4& lhs, const float4& rhs)  { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w }; }
    inline friend float4 operator + (const float4& v, float f)              { return { v.x + f, v.y + f, v.z + f, v.w + f }; }
    inline float4& operator += (const float4& v)                            { return (*this = *this + v); }
    inline float4& operator += (float f)                                    { return (*this = *this + f); }

    inline friend float4 operator * (const float4& lhs, const float4& rhs)  { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w }; }
    inline friend float4 operator * (const float4& v, float f)              { return { v.x * f, v.y * f, v.z * f, v.w * f }; }
    inline float4& operator *= (const float4& v)                            { return (*this = *this * v); }
    inline float4& operator *= (float f)                                    { return (*this = *this * f); }

    inline friend float4 operator / (const float4& lhs, const float4& rhs)  { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w }; }
    inline friend float4 operator / (const float4& v, float f)              { return { v.x / f, v.y / f, v.z / f, v.w / f }; }
    inline float4& operator /= (const float4& v)                            { return (*this = *this / v); }
    inline float4& operator /= (float f)                                    { return (*this = *this / f); }

public:
    float x, y, z, w;
};
}; // namespace eloo::Math