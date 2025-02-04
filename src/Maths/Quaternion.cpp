#include <Maths/Quaternion.h>

#include <Maths/Math.h>

using namespace eloo::Math;

Quaternion::Quaternion(const Vector3& euler) {
    const Quaternion qx = { cos(euler.x * 0.5f), sin(euler.x * 0.5f), 0.0f, 0.0f };
    const Quaternion qy = { cos(euler.y * 0.5f), 0.0f, sin(euler.y * 0.5f), 0.0f };
    const Quaternion qz = { cos(euler.z * 0.5f), 0.0f, 0.0f, sin(euler.z * 0.5f) };
    *this = qz * qy * qx;
}

Quaternion::Quaternion(const Vector3& axis, float radians) {
    const float sinVal = sin(radians * 0.5f);
    w = cos(radians * 0.5f);
    x = axis.x * sinVal;
    y = axis.y * sinVal;
    z = axis.z * sinVal;
}

float Quaternion::magnitudeSqr() const {
    return w * w + x * x + y * y + z * z;
}

float Quaternion::magnitude() const {
    return sqrt(magnitudeSqr());
}

float Quaternion::dot(const Quaternion& q1, const Quaternion& q2) {
    return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

Quaternion Quaternion::normalize(const Quaternion& q) {
    return q / q.magnitude();
}

Quaternion Quaternion::conjugate(const Quaternion& q) {
    return { q.w, -q.x, -q.y, -q.z };
}

Quaternion Quaternion::inverse(const Quaternion& q) {
    return conjugate(q) / q.magnitudeSqr();
}

float Quaternion::roll() const {
    return atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
}

float Quaternion::pitch() const {
    return asin(clamp(2.0f * (w * y - z * x), -1.0f, 1.0f));
}

float Quaternion::yaw() const {
    return atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
}

Quaternion Quaternion::slerp(const Quaternion& from, const Quaternion& to, float t) {
    const float angleDiff = acos(dot(from, to));
    const Quaternion _to = angleDiff < 0.0f ? -to : to;
    return (from * (sin(1.0f - t) * angleDiff) + _to * sin(t * angleDiff)) / sin(angleDiff);
}