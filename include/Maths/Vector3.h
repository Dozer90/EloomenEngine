#pragma once

namespace eloo::Math {
class Vector3 {
public:
    inline constexpr Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    inline constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    inline static Vector3 zero() { static constexpr Vector3 v{ 0.0f, 0.0f, 0.0f }; return v; }
    inline static Vector3 one() { static constexpr Vector3 v{ 1.0f, 1.0f, 1.0f }; return v; }
    inline static Vector3 up() { static constexpr Vector3 v{ 0.0f, 1.0f, 0.0f }; return v; }
    inline static Vector3 right() { static constexpr Vector3 v{ 1.0f, 0.0f, 0.0f }; return v; }
    inline static Vector3 forward() { static constexpr Vector3 v{ 0.0f, 0.0f, 1.0f }; return v; }

    inline float magnitudeSqr() const { return x * x + y * y + z * z; }
    float magnitude() const;
    float fastMagnitude(unsigned int iterationCount = 1) const;

    static Vector3 clampMagnitude(const Vector3& v, float maxMagnitude);
    inline Vector3& clampMagnitude(float maxMagnitude) { return (*this = clampMagnitude(*this, maxMagnitude)); }

    static Vector3 normalize(const Vector3& v);
    inline Vector3& normalize() { return (*this = normalize(*this)); }

    static Vector3 fastNormalize(const Vector3& v, unsigned int iterationCount = 1);
    inline Vector3& fastNormalize(unsigned int iterationCount = 1) { return (*this = fastNormalize(*this, iterationCount)); }

    static bool isNormalized(const Vector3& v);
    inline bool isNormalized() const { return Vector3::isNormalized(*this); }

    static float dot(const Vector3& v1, const Vector3& v2);
    inline float dot(const Vector3& other) const { return dot(*this, other); }

    static Vector3 cross(const Vector3& v1, const Vector3& v2);
    inline Vector3 cross(const Vector3& other) const { return cross(*this, other); }

    static float distance(const Vector3& v1, const Vector3& v2);
    inline float distance(const Vector3& other) const { return distance(*this, other); }

    static float distanceSqr(const Vector3& v1, const Vector3& v2);
    inline float distanceSqr(const Vector3& other) const { return distanceSqr(*this, other); }

    static float angle(const Vector3& v1, const Vector3& v2);
    inline float angle(const Vector3& other) const { return angle(*this, other); }

    static Vector3 reflect(const Vector3& v, const Vector3& normal);
    inline Vector3& reflect(const Vector3& normal) { return (*this = reflect(*this, normal)); }

    static Vector3 project(const Vector3& v, const Vector3& normal);
    inline Vector3& project(const Vector3& normal) { return (*this = project(*this, normal)); }

    static Vector3 rotate(const Vector3& toRotate, const Vector3& axis, float radians);
    inline Vector3& rotate(const Vector3& axis, float radians) { return (*this = rotate(*this, axis, radians)); }

    static Vector3 lerp(const Vector3& start, const Vector3& end, float t);

    // =========================================================================================
    // Operation overloads

    inline friend bool operator!=(const Vector3& lhs, const Vector3& rhs)    { return lhs.x != rhs.x && lhs.y != lhs.y && lhs.z != rhs.z; }
    inline friend bool operator==(const Vector3& lhs, const Vector3& rhs)    { return !(lhs != rhs); }

    inline friend Vector3 operator-(const Vector3& v)                        { return { -v.x, -v.y, -v.z }; }

    inline friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs)  { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }
    inline friend Vector3 operator-(const Vector3& lhs, float value)         { return { lhs.x - value, lhs.y - value, lhs.z - value }; }
    inline friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs)  { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
    inline friend Vector3 operator+(const Vector3& lhs, float value)         { return { lhs.x + value, lhs.y + value, lhs.z + value }; }
    inline friend Vector3 operator*(const Vector3& lhs, const Vector3& rhs)  { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z }; }
    inline friend Vector3 operator*(const Vector3& lhs, float value)         { return { lhs.x * value, lhs.y * value, lhs.z * value }; }
    inline friend Vector3 operator/(const Vector3& lhs, const Vector3& rhs)  { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z }; }
    inline friend Vector3 operator/(const Vector3& lhs, float value)         { return { lhs.x / value, lhs.y / value, lhs.z / value }; }

    inline Vector3& operator-()                                              { return (*this = { -x, -y, -z }); }
    inline Vector3& operator+=(const Vector3& rhs)                           { return (*this = *this + rhs); }
    inline Vector3& operator-=(const Vector3& rhs)                           { return (*this = *this - rhs); }
    inline Vector3& operator*=(float value)                                  { return (*this = *this * value); }
    inline Vector3& operator/=(float value)                                  { return (*this = *this / value); }

public:
    float x, y, z;
};
}; // namespace Math