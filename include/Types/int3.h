#pragma once

namespace eloo {
class float2;

class float3 {
public:
    inline constexpr float3() : x(0.0f), y(0.0f), z(0.0f) {}
    inline constexpr float3(float xyz) : x(xyz), y(xyz), z(xyz) {}
    inline constexpr float3(float x, float y, float z) : x(x), y(y), z(z) {}
    float3(const float2& xy, float z = 0.0f);

    inline float* elements()                                                { return &x; }
    inline const float* elements() const                                    { return &x; }

    inline static constexpr float3 zero()                                   { return { 0.0f }; }
    inline static constexpr float3 one()                                    { return { 1.0f }; }
    inline static constexpr float3 up()                                     { return { 0.0f, 1.0f, 0.0f }; }
    inline static constexpr float3 right()                                  { return { 1.0f, 0.0f, 0.0f }; }
    inline static constexpr float3 forward()                                { return { 0.0f, 0.0f, 1.0f }; }

    inline float magnitudeSqr() const                                       { return x * x + y * y + z * z; }

    bool isNormalized() const;
    float3& normalize();
    float3& normalizeFast(unsigned int iterationCount = 1);
    float magnitude() const;
    float magnitudeFast(unsigned int iterationCount = 1) const;
    float3& reflect(const float3& normal);
    float3& project(const float3& normal);
    float3& rotate(float radians, const float3& axis);

    float2 xy() const;


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