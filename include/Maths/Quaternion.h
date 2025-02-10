#pragma once

#include <Maths/float3.h>

namespace eloo::Math {
class Quaternion {
public:
    inline constexpr Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}
    inline constexpr Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
    inline Quaternion(const float3& euler) { set(euler); }
    inline Quaternion(float radians, const float3& axis) { set(radians, axis); }

    void set(float w, float x, float y, float z);
    void set(const float3& euler);
    void set(float radians, const float3& axis);

    float magnitudeSqr() const;
    float magnitude() const;

    static float dot(const Quaternion& q1, const Quaternion& q2);

    static Quaternion normalize(const Quaternion& q);
    inline Quaternion& normalize() { return (*this = normalize(*this)); }

    static Quaternion conjugate(const Quaternion& q);
    inline Quaternion& conjugate() { return (*this = conjugate(*this)); }

    static Quaternion inverse(const Quaternion& q);
    inline Quaternion& inverse() { return (*this = inverse(*this)); }

    float roll() const;
    float pitch() const;
    float yaw() const;
    inline float3 euler() const { return { roll(), pitch(), yaw() }; }

    static inline Quaternion lerp(const Quaternion& from, const Quaternion& to, float t) { return from * (1.0f - t) + to * t; }
    static Quaternion slerp(const Quaternion& from, const Quaternion& to, float t);

    // =========================================================================================
    // Operation overloads

    friend Quaternion operator * (const Quaternion& lhs, const Quaternion& rhs) {
        return {
            lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
            lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
            lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
            lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x
        };
    }

    friend float3 operator * (const float3& v, const Quaternion& q) {
        Quaternion qV = { 0.0f, v.x, v.y, v.z };
        qV = q * qV * inverse(q);
        return { qV.x, qV.y, qV.z };
    }

    friend bool operator!=(const Quaternion& lhs, const Quaternion& rhs)        { return lhs.w != rhs.w && lhs.x != rhs.x && lhs.y != rhs.y && lhs.z != rhs.z; }
    friend bool operator == (const Quaternion& lhs, const Quaternion& rhs)        { return !(lhs != rhs); }

    friend Quaternion operator - (const Quaternion& q)                            { return { -q.w, -q.x, -q.y, -q.z }; }

    friend Quaternion operator / (const Quaternion& lhs, const Quaternion& rhs)   { return { lhs.w / rhs.w, lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z }; }
    friend Quaternion operator - (const Quaternion& lhs, const Quaternion& rhs)   { return { lhs.w - rhs.w, lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }
    friend Quaternion operator + (const Quaternion& lhs, const Quaternion& rhs)   { return { lhs.w + rhs.w, lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }

    friend Quaternion operator * (const Quaternion& q, float value)               { return { q.w * value, q.x * value, q.y * value, q.z * value }; }
    friend Quaternion operator / (const Quaternion& q, float value)               { return { q.w / value, q.x / value, q.y / value, q.z / value }; }
    friend Quaternion operator - (const Quaternion& q, float value)               { return { q.w - value, q.x - value, q.y - value, q.z - value }; }
    friend Quaternion operator + (const Quaternion& q, float value)               { return { q.w + value, q.x + value, q.y + value, q.z + value }; }

    inline Quaternion& operator - ()                                              { return (*this = -*this); }
    inline Quaternion& operator *= (float value)                                  { return (*this = *this * value); }
    inline Quaternion& operator /= (float value)                                  { return (*this = *this / value); }
    inline Quaternion& operator += (const Quaternion& rhs)                        { return (*this = *this + rhs); }
    inline Quaternion& operator -= (const Quaternion& rhs)                        { return (*this = *this - rhs); }

public:
    float w, x, y, z;
};
}; // namespace Math