#include <Maths/float3.h>
#include <Maths/Math.h>

using namespace eloo;

/////////////////////////////////////////////////
// Maths generics

Math::float3 Math::normalize(const float3& v) {
    if (v.isNormalized()) {
        return v;
    }
    return v / v.magnitude();
}

Math::float3 Math::fastNormalize(const float3& v, unsigned int iterationCount) {
    if (v.isNormalized()) {
        return v;
    }
    return v / v.fastMagnitude(iterationCount);
}

bool Math::isNormalized(const float3& v) {
    return isEqual(v.magnitudeSqr(), 1.0f);
}

float Math::dot(const float3& v1, const float3& v2) {
    const float3 n1 = normalize(v1);
    const float3 n2 = normalize(v2);
    return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
}

Math::float3 Math::cross(const float3& v1, const float3& v2) {
    const float3 n1 = normalize(v1);
    const float3 n2 = normalize(v2);
    return {
        n1.y * n2.z - n1.z * n2.y,
        n1.z * n2.x - n1.x * n2.z,
        n1.x * n2.y - n1.y * n2.x
    };
}

float Math::distance(const float3& v1, const float3& v2) {
    return (v1 - v2).magnitude();
}

float Math::distanceSqr(const float3& v1, const float3& v2) {
    return (v1 - v2).magnitudeSqr();
}

float Math::angle(const float3& v1, const float3& v2) {
    const float magTotalSqr = v1.magnitudeSqr() * v2.magnitudeSqr();
    if (isZero(magTotalSqr)) {
        return 0.0f;
    }
    const float d = dot(v1, v2);
    return acos((d * d) / magTotalSqr);
}

Math::float3 Math::reflect(const float3& v, const float3& normal) {
    return v - normal * 2.0f * dot(v, normal);
}

Math::float3 Math::project(const float3& v, const float3& normal) {
    const float magSqr = normal.magnitudeSqr();
    if (isZero(magSqr)) {
        return float3::zero();
    }
    return normal * (dot(v, normal) / magSqr);
}

Math::float3 Math::rotate(const float3& toRotate, const float3& axis, float radians) {
    const float3 naxis = normalize(axis);
    const float cosVal = cos(radians);
    const float3 v1 = toRotate * cosVal;
    const float3 v2 = cross(naxis, toRotate) * sin(radians);
    const float3 v3 = naxis * dot(naxis, toRotate) * (-1.0f - cosVal);
    return v1 + v2 + v3;
}

Math::float3 Math::lerp(const float3& start, const float3& end, float t) {
    return start + (end - start) * t;
}

/////////////////////////////////////////////////
// float3

float Math::float3::magnitude() const {
    return sqrt(magnitudeSqr());
}

float Math::float3::fastMagnitude(unsigned int iterationCount) const {
    return fastSqrt(magnitudeSqr(), iterationCount);
}