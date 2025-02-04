#include <Maths/Vector2.h>
#include <Maths/Maths.h>

using namespace eloo::Math;

float Vector2::magnitude() const {
    return sqrt(x * x + y * y);
}

float Vector2::fastMagnitude(unsigned int iterationCount) const {
    return fastSqrt(x * x + y * y, iterationCount);
}

Vector2 Vector2::clampMagnitude(const Vector2& v, float maxMagnitude) {
    if (v.magnitudeSqr() < maxMagnitude * maxMagnitude) {
        return v;
    }
    return normalize(v) * maxMagnitude;
}

Vector2 Vector2::normalize(const Vector2& v) {
    if (v.isNormalized()) {
        return v;
    }
    return v / v.magnitude();
}

Vector2 Vector2::fastNormalize(const Vector2& v, unsigned int iterationCount) {
    if (v.isNormalized()) {
        return v;
    }
    return v / v.fastMagnitude(iterationCount);
}

bool Vector2::isNormalized(const Vector2& v) {
    return isEqual(v.magnitudeSqr(), 1.0f);
}

float Vector2::dot(const Vector2& v1, const Vector2& v2) {
    const Vector2 n1 = normalize(v1);
    const Vector2 n2 = normalize(v2);
    return n1.x * n2.x + n1.y * n2.y;
}

float Vector2::distance(const Vector2& v1, const Vector2& v2) {
    return (v1 - v2).magnitude();
}

float Vector2::angle(const Vector2& v1, const Vector2& v2) {
    const float magTotalSqr = v1.magnitudeSqr() * v2.magnitudeSqr();
    if (isZero(magTotalSqr)) {
        return 0.0f;
    }
    const float d = dot(v1, v2);
    return acos((d * d) / magTotalSqr);
}

Vector2 Vector2::reflect(const Vector2& v, const Vector2& normal) {
    return v - normal * 2.0f * dot(v, normal);
}

Vector2 Vector2::project(const Vector2& v, const Vector2& normal) {
    const float magSqr = normal.magnitudeSqr();
    if (isZero(magSqr)) {
        return zero();
    }
    return normal * (dot(v, normal) / magSqr);
}

Vector2 Vector2::rotate(const Vector2& v, float radians) {
    return {
        v.x * cos(radians) - v.y * sin(radians),
        v.x * sin(radians) + v.y * cos(radians),
    };
}

Vector2 Vector2::lerp(const Vector2& start, const Vector2& end, float t) {
    return start + (end - start) * t;
}