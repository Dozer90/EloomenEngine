#include <Maths/Quaternion.h>

#include <Maths/Math.h>

using namespace eloo::Math;

void Quaternion::set(float w, float x, float y, float z) {
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

void Quaternion::set(const float3& euler) {
    const float cx = cos(euler.x * 0.5f), sx = sin(euler.x * 0.5f);
    const float cy = cos(euler.y * 0.5f), sy = sin(euler.y * 0.5f);
    const float cz = cos(euler.z * 0.5f), sz = sin(euler.z * 0.5f);
    w = cx * cy * cz + sx * sy * sz;
    x = sx * cy * cz - cx * sy * sz;
    y = cx * sy * cz + sx * cy * sz;
    z = cx * cy * sz - sx * sy * cz;
}

void Quaternion::set(float radians, const float3& axis) {
    const float s = sin(radians * 0.5f);
    w = cos(radians * 0.5f);
    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
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