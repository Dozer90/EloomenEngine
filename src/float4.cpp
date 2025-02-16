#include <Maths/CoreMath.h>

using namespace eloo;

float4::float4(const float3& xyz, float w) :
    x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

float float4::magnitude() const {
    return Math::sqrt(x * x + y * y + z * z + w * w);
}

bool float4::isNormalized() const {
    return Math::isNormalized(*this);
}

float4& float4::normalize() {
    return (*this = Math::normalize(*this));
}

float4& float4::normalizeFast(unsigned int iterationCount) {
    return (*this = Math::normalizeFast(*this, iterationCount));
}

float float4::magnitude() const {
    return Math::sqrt(x * x + y * y + z * z + w * w);
}

float float4::magnitudeFast(unsigned int iterationCount) const {
    return Math::sqrtFast(x * x + y * y + z * z + w * w, iterationCount);
}

float3 float4::xyz() const {
    return { x, y, z };
}