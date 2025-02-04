#include <Maths/Vector3.h>
#include <Maths/Math.h>

using namespace eloo::Math;

float Vector3::magnitude() const {
    return sqrt(magnitudeSqr());
}

float Vector3::fastMagnitude(unsigned int iterationCount) const {
    return fastSqrt(magnitudeSqr(), iterationCount);
}

Vector3 Vector3::clampMagnitude(const Vector3& v, float maxMagnitude) {
    if (v.magnitudeSqr() < maxMagnitude * maxMagnitude) {
        return v;
    }
    return normalize(v) * maxMagnitude;
}

Vector3 Vector3::normalize(const Vector3& v) {
    if (v.isNormalized()) {
        return v;
    }
    return v / v.magnitude();
}

Vector3 Vector3::fastNormalize(const Vector3& v, unsigned int iterationCount) {
    if (v.isNormalized()) {
        return v;
    }
    return v / v.fastMagnitude(iterationCount);
}

bool Vector3::isNormalized(const Vector3& v) {
    return isEqual(v.magnitudeSqr(), 1.0f);
}

float Vector3::dot(const Vector3& v1, const Vector3& v2) {
    const Vector3 n1 = normalize(v1);
    const Vector3 n2 = normalize(v2);
    return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
}

Vector3 Vector3::cross(const Vector3& v1, const Vector3& v2) {
    const Vector3 n1 = normalize(v1);
    const Vector3 n2 = normalize(v2);
    return {
        n1.y * n2.z - n1.z * n2.y,
        n1.z * n2.x - n1.x * n2.z,
        n1.x * n2.y - n1.y * n2.x
    };
}

float Vector3::distance(const Vector3& v1, const Vector3& v2) {
    return (v1 - v2).magnitude();
}

float Vector3::distanceSqr(const Vector3& v1, const Vector3& v2) {
    return (v1 - v2).magnitudeSqr();
}

float Vector3::angle(const Vector3& v1, const Vector3& v2) {
    const float magTotalSqr = v1.magnitudeSqr() * v2.magnitudeSqr();
    if (isZero(magTotalSqr)) {
        return 0.0f;
    }
    const float d = dot(v1, v2);
    return acos((d * d) / magTotalSqr);
}

Vector3 Vector3::reflect(const Vector3& v, const Vector3& normal) {
    return v - normal * 2.0f * dot(v, normal);
}

Vector3 Vector3::project(const Vector3& v, const Vector3& normal) {
    const float magSqr = normal.magnitudeSqr();
    if (isZero(magSqr)) {
        return zero();
    }
    return normal * (dot(v, normal) / magSqr);
}

Vector3 Vector3::rotate(const Vector3& toRotate, const Vector3& axis, float radians) {
    const Vector3 naxis = normalize(axis);
    const float cosVal = cos(radians);
    const Vector3 v1 = toRotate * cosVal;
    const Vector3 v2 = cross(naxis, toRotate) * sin(radians);
    const Vector3 v3 = naxis * dot(naxis, toRotate) * (-1.0f - cosVal);
    return v1 + v2 + v3;
}

Vector3 Vector3::lerp(const Vector3& start, const Vector3& end, float t) {
    return start + (end - start) * t;
}