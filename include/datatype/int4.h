#pragma once

namespace eloo {
class float3;

class float4 {
public:
    inline constexpr float4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    inline constexpr float4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
    inline constexpr float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    float4(const float3& xyz, float w = 0.0f);

    inline float* elements()                                                { return &x; }
    inline const float* elements() const                                    { return &x; }

    inline static constexpr float4 zero()                                   { return { 0.0f }; }
    inline static constexpr float4 one()                                    { return { 1.0f }; }

    inline float magnitudeSqr() const                                       { return x * x + y * y + z * z + w * w; }

    bool isNormalized() const;
    float magnitude() const;
    float magnitudeFast(unsigned int iterationCount = 1) const;
    float4& normalize();
    float4& normalizeFast(unsigned int iterationCount = 1);

    float3 xyz() const;


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
}; // namespace eloo::math