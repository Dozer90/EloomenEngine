#pragma once

namespace eloo::Math {
class Vector4 {
public:
    inline constexpr Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    inline constexpr Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    inline static Vector4 zero() { static constexpr Vector4 v{ 0.0f, 0.0f, 0.0f, 0.0f }; return v; }
    inline static Vector4 one() { static constexpr Vector4 v{ 1.0f, 1.0f, 1.0f, 1.0f }; return v; }

    inline float magnitudeSqr() const { return x * x + y * y + z * z + w * w; }
    float magnitude() const;
    float fastMagnitude(unsigned int iterationCount = 1) const;

    static Vector4 clampMagnitude(const Vector4& v, float maxMagnitude);
    inline Vector4& clampMagnitude(float maxMagnitude) { return (*this = clampMagnitude(*this, maxMagnitude)); }

    static Vector4 normalize(const Vector4& v);
    inline Vector4& normalize() { return (*this = normalize(*this)); }

    static Vector4 fastNormalize(const Vector4& v, unsigned int iterationCount = 1);
    inline Vector4& fastNormalize(unsigned int iterationCount = 1) { return (*this = fastNormalize(*this, iterationCount)); }

    static bool isNormalized(const Vector4& v);
    inline bool isNormalized() const { return Vector4::isNormalized(*this); }

    static float dot(const Vector4& v1, const Vector4& v2);
    inline float dot(const Vector4& other) const { return dot(*this, other); }

    static Vector4 cross(const Vector4& v1, const Vector4& v2);
    inline Vector4 cross(const Vector4& other) const { return cross(*this, other); }

    static float distance(const Vector4& v1, const Vector4& v2);
    inline float distance(const Vector4& other) const { return distance(*this, other); }

    static float distanceSqr(const Vector4& v1, const Vector4& v2);
    inline float distanceSqr(const Vector4& other) const { return distanceSqr(*this, other); }

    static float angle(const Vector4& v1, const Vector4& v2);
    inline float angle(const Vector4& other) const { return angle(*this, other); }

    static Vector4 reflect(const Vector4& v, const Vector4& normal);
    inline Vector4& reflect(const Vector4& normal) { return (*this = reflect(*this, normal)); }

    static Vector4 project(const Vector4& v, const Vector4& normal);
    inline Vector4& project(const Vector4& normal) { return (*this = project(*this, normal)); }

    static Vector4 rotate(const Vector4& toRotate, const Vector4& axis, float radians);
    inline Vector4& rotate(const Vector4& axis, float radians) { return (*this = rotate(*this, axis, radians)); }

    static Vector4 lerp(const Vector4& start, const Vector4& end, float t);

    // =========================================================================================
    // Operation overloads

    inline bool operator!=(const Vector4& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }
    inline bool operator==(const Vector4& rhs) const { return !(*this != rhs); }

    inline Vector4 operator-() const { return { -x, -y, -z }; }

    inline Vector4 operator+(const Vector4& rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
    inline Vector4 operator-(const Vector4& rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
    inline Vector4 operator*(const float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
    inline Vector4 operator/(const float scalar) const { return { x / scalar, y / scalar, z / scalar }; }

    inline Vector4& operator+=(const Vector4& rhs) { return (*this = *this + rhs); }
    inline Vector4& operator-=(const Vector4& rhs) { return (*this = *this - rhs); }
    inline Vector4& operator*=(const float scalar) { return (*this = *this * scalar); }
    inline Vector4& operator/=(const float scalar) { return (*this = *this / scalar); }

public:
    float x, y, z;
};
}; // namespace Math