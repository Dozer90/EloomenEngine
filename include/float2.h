#pragma once

namespace eloo {
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

    inline float magnitudeSqr() const                               { return x * x + y * y; }

    bool isNormalized() const;
    float2& normalize();
    float2& normalizeFast(unsigned int iterationCount = 1);
    float magnitude() const;
    float magnitudeFast(unsigned int iterationCount = 1) const;
    float2& reflect(const float2& normal);
    float2& project(const float2& normal);
    float2& rotate(float radians);


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