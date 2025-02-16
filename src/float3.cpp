#include <Maths/CoreMath.h>

using namespace eloo;

float3::float3(const float2& xy, float z = 0.0f) :
    x(xy.x), y(xy.y), z(z) {}

bool float3::isNormalized() const {
    return Math::isNormalized(*this);
}

float3& float3::normalize() {
    return (*this = Math::normalize(*this));
}

float3& float3::normalizeFast(unsigned int iterationCount) {
    return (*this = Math::normalizeFast(*this, iterationCount));
}

float float3::magnitude() const {
    return Math::sqrt(x * x + y * y + z * z);
}

float float3::magnitudeFast(unsigned int iterationCount) const {
    return Math::sqrtFast(x * x + y * y + z * z, iterationCount);
}

float3& float3::reflect(const float3& normal) {
    return (*this = Math::reflect(*this, normal));
}

float3& float3::project(const float3& normal) {
    return (*this = Math::project(*this, normal));
}

float3& float3::rotate(float radians, const float3& axis) {
    return (*this = Math::rotate(*this, radians, axis));
}

float2 float3::xy() const {
    return { x, y };
}