#include <Maths/CoreMath.h>

using namespace eloo;

bool float2::isNormalized() const {
    return Math::isNormalized(*this);
}

float2& float2::normalize() {
    return (*this = Math::normalize(*this));
}

float2& float2::normalizeFast(unsigned int iterationCount) {
    return (*this = Math::normalizeFast(*this, iterationCount));
}

float float2::magnitude() const {
    return Math::sqrt(x * x + y * y);
}

float float2::magnitudeFast(unsigned int iterationCount) const {
    return Math::magnitudeFast(*this, iterationCount);
}

float2& float2::reflect(const float2& normal) {
    return (*this = Math::reflect(*this, normal));
}

float2& float2::project(const float2& normal) {
    return (*this = Math::project(*this, normal));
}

float2& float2::rotate(float radians) {
    return (*this = Math::rotate(*this, radians));
}