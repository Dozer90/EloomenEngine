#include <Maths/float2.h>
#include <Maths/Math.h>

using namespace eloo;

/////////////////////////////////////////////////
// Maths generics

Math::float2 Math::normalize(const float2& v) {
    if (v.isNormalized()) {
        return v;
    }
    return v / v.magnitude();
}

Math::float2 Math::fastNormalize(const float2& v, unsigned int iterationCount) {
    if (v.isNormalized()) {
        return v;
    }
    return v / v.fastMagnitude(iterationCount);
}

bool Math::isNormalized(const float2& v) {
    return isEqual(v.magnitudeSqr(), 1.0f);
}

float Math::dot(const float2& v1, const float2& v2) {
    const float2 n1 = normalize(v1);
    const float2 n2 = normalize(v2);
    return n1.x * n2.x + n1.y * n2.y;
}

float Math::distance(const float2& v1, const float2& v2) {
    return (v1 - v2).magnitude();
}

float Math::angle(const float2& v1, const float2& v2) {
    const float magTotalSqr = v1.magnitudeSqr() * v2.magnitudeSqr();
    if (isZero(magTotalSqr)) {
        return 0.0f;
    }
    const float d = dot(v1, v2);
    return acos((d * d) / magTotalSqr);
}

Math::float2 Math::reflect(const float2& v, const float2& normal) {
    return v - normal * 2.0f * dot(v, normal);
}

Math::float2 Math::project(const float2& v, const float2& normal) {
    const float magSqr = normal.magnitudeSqr();
    if (isZero(magSqr)) {
        return float2::zero();
    }
    return normal * (dot(v, normal) / magSqr);
}

Math::float2 Math::rotate(const float2& v, float radians) {
    return {
        v.x * cos(radians) - v.y * sin(radians),
        v.x * sin(radians) + v.y * cos(radians),
    };
}

Math::float2 Math::lerp(const float2& start, const float2& end, float t) {
    return start + (end - start) * t;
}

/////////////////////////////////////////////////
// float2

float Math::float2::magnitude() const {
    return sqrt(x * x + y * y);
}

float Math::float2::fastMagnitude(unsigned int iterationCount) const {
    return Math::fastSqrt(x * x + y * y, iterationCount);
}