#pragma once

namespace eloo::Math {
class Vector2 {
public:
    inline constexpr Vector2() : x(0.0f), y(0.0f) {}
    inline constexpr Vector2(float x, float y) : x(x), y(y) {}

    inline static Vector2 zero() { static constexpr Vector2 v{ 0.0f, 0.0f }; return v; }
    inline static Vector2 one() { static constexpr Vector2 v{ 1.0f, 1.0f }; return v; }
    inline static Vector2 up() { static constexpr Vector2 v{ 0.0f, 1.0f }; return v; }
    inline static Vector2 right() { static constexpr Vector2 v{ 1.0f, 0.0f }; return v; }

    inline float magnitudeSqr() const { return x * x + y * y; }
    float magnitude() const;
    float fastMagnitude(unsigned int iterationCount = 1) const;

    static Vector2 clampMagnitude(const Vector2& v, float maxMagnitude);
    inline Vector2& clampMagnitude(float maxMagnitude) { return (*this = clampMagnitude(*this, maxMagnitude)); }

    static Vector2 normalize(const Vector2& v);
    inline Vector2& normalize() { return (*this = normalize(*this)); }

    static Vector2 fastNormalize(const Vector2& v, unsigned int iterationCount = 1);
    inline Vector2& fastNormalize(unsigned int iterationCount = 1) { return (*this = fastNormalize(*this, iterationCount)); }

    static bool isNormalized(const Vector2& v);
    inline bool isNormalized() const { return Vector2::isNormalized(*this); }

    static float dot(const Vector2& v1, const Vector2& v2);
    inline float dot(const Vector2& other) const { return dot(*this, other); }

    static float distance(const Vector2& v1, const Vector2& v2);
    inline float distance(const Vector2& other) const { return distance(*this, other); }

    static float angle(const Vector2& v1, const Vector2& v2);
    inline float angle(const Vector2& other) const { return angle(*this, other); }

    static Vector2 reflect(const Vector2& v, const Vector2& normal);
    inline Vector2& reflect(const Vector2& normal) { return (*this = reflect(*this, normal)); }

    static Vector2 project(const Vector2& v, const Vector2& normal);
    inline Vector2& project(const Vector2& normal) { return (*this = project(*this, normal)); }

    static Vector2 rotate(const Vector2& v, float radians);
    inline Vector2& rotate(float radians) { return (*this = rotate(*this, radians)); }

    static Vector2 lerp(const Vector2& start, const Vector2& end, float t);

    // =========================================================================================
    // Operation overloads

    inline friend bool operator!=(const Vector2& lhs, const Vector2& rhs)    { return lhs.x != rhs.x && lhs.y != lhs.y; }
    inline friend bool operator==(const Vector2& lhs, const Vector2& rhs)    { return !(lhs != rhs); }

    inline friend Vector2 operator-(const Vector2& v)                        { return { -v.x, -v.y }; }

    inline friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs)  { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
    inline friend Vector2 operator-(const Vector2& lhs, float value)         { return { lhs.x - value, lhs.y - value }; }
    inline friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs)  { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
    inline friend Vector2 operator+(const Vector2& lhs, float value)         { return { lhs.x + value, lhs.y + value }; }
    inline friend Vector2 operator*(const Vector2& lhs, const Vector2& rhs)  { return { lhs.x * rhs.x, lhs.y * rhs.y }; }
    inline friend Vector2 operator*(const Vector2& lhs, float value)         { return { lhs.x * value, lhs.y * value }; }
    inline friend Vector2 operator/(const Vector2& lhs, const Vector2& rhs)  { return { lhs.x / rhs.x, lhs.y / rhs.y }; }
    inline friend Vector2 operator/(const Vector2& lhs, float value)         { return { lhs.x / value, lhs.y / value }; }

    inline Vector2& operator-()                                              { return (*this = { -x, -y }); }
    inline Vector2& operator+=(const Vector2& rhs)                           { return (*this = *this + rhs); }
    inline Vector2& operator-=(const Vector2& rhs)                           { return (*this = *this - rhs); }
    inline Vector2& operator*=(float value)                                  { return (*this = *this * value); }
    inline Vector2& operator/=(float value)                                  { return (*this = *this / value); }

public:
    float x, y;
};
}; // namespace Math