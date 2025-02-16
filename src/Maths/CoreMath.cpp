#include <Maths/CoreMath.h>

using namespace eloo;

///////////////////////////////////////////////
// float2
///////////////////////////////////////////////

bool Math::isNormalized(const float2& v) {
    return isEqual(v.magnitudeSqr(), 1.0f);
}

float2 Math::normalize(const float2& v) {
    if (v.isNormalized()) return v;
    if (v.magnitudeSqr() == 0.0f) return float2::zero();
    return v / v.magnitude();
}

float2 Math::normalizeFast(const float2& v, unsigned int iterationCount) {
    if (v.isNormalized()) return v;
    const float magSqr = v.magnitudeSqr();
    if (magSqr == 0.0f) return float2::zero();
    if (isEqual(v.x, v.y)) return Consts::flt::RSqrt2;
    return v * rsqrtFast(magSqr, iterationCount);
}

float Math::magnitudeFast(const float2& v, unsigned int iterationCount) {
    return sqrtFast(v.magnitudeSqr(), iterationCount);
}

float Math::dot(const float2& v1, const float2& v2) {
    const float2 n1 = normalize(v1);
    const float2 n2 = normalize(v2);
    return n1.x * n2.x + n1.y * n2.y;
}

float Math::distance(const float2& v1, const float2& v2) {
    return (v1 - v2).magnitude();
}

float Math::distanceSqr(const float2& v1, const float2& v2) {
    return (v1 - v2).magnitudeSqr();
}

float Math::angle(const float2& v1, const float2& v2) {
    const float magTotalSqr = v1.magnitudeSqr() * v2.magnitudeSqr();
    if (isEqual(magTotalSqr, 0.0f)) return 0.0f;
    if (isEqual(v1.x, v1.y) && isEqual(v2.x, v2.y)) return acos(Consts::flt::RSqrt2);
    const float d = dot(v1, v2);
    return acos(clamp(d * rsqrt(magTotalSqr), -1.0f, 1.0f));
}

float Math::angleSigned(const float2& v1, const float2& v2) {
    return atan2(v1.x * v2.y - v1.y * v2.x, dot(v1, v2));
}

float2 Math::reflect(const float2& v, const float2& normal) {
    return v - normal * 2.0f * dot(v, normal);
}

float2 Math::project(const float2& v, const float2& normal) {
    const float magSqr = normal.magnitudeSqr();
    if (isEqual(magSqr, 0.0f)) {
        return float2::zero();
    }
    return normal * (dot(v, normal) / magSqr);
}

float2 Math::rotate(const float2& v, float radians) {
    return {
        v.x * cos(radians) - v.y * sin(radians),
        v.x * sin(radians) + v.y * cos(radians),
    };
}

float2 Math::Interpolation::interpolate(const float2& start, const float2& end, double t, Type type, Options options) {
    return {
        static_cast<float>(interpolate(static_cast<double>(start.x), static_cast<double>(end.x), t, type, options)),
        static_cast<float>(interpolate(static_cast<double>(start.y), static_cast<double>(end.y), t, type, options))
    };
}


///////////////////////////////////////////////
// float3
///////////////////////////////////////////////

bool Math::isNormalized(const float3& v) {
    return isEqual(v.magnitudeSqr(), 1.0f);
}

float3 Math::normalize(const float3& v) {
    if (v.isNormalized()) return v;
    if (v.magnitudeSqr() == 0.0f) return float3::zero();
    return v / v.magnitude();
}

float3 Math::normalizeFast(const float3& v, unsigned int iterationCount) {
    if (v.isNormalized()) return v;
    if (v.magnitudeSqr() == 0.0f) return float3::zero();
    return v / magnitudeFast(v, iterationCount);
}

float Math::magnitudeFast(const float3& v, unsigned int iterationCount) {
    return sqrtFast(v.magnitudeSqr(), iterationCount);
}

float Math::dot(const float3& v1, const float3& v2) {
    const float3 n1 = normalize(v1);
    const float3 n2 = normalize(v2);
    return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
}

float3 cross(const float3& v1, const float3& v2) {
    const float3 n1 = Math::normalize(v1);
    const float3 n2 = Math::normalize(v2);
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
    if (isEqual(magTotalSqr, 0.0f)) {
        return 0.0f;
    }
    const float d = dot(v1, v2);
    return acos((d * d) / magTotalSqr);
}

float Math::angleSigned(const float3& v1, const float3& v2, const float3& normal) {
    return atan2(dot(cross(v1, v2), normal), dot(v1, v2));
}

float3 Math::reflect(const float3& v, const float3& normal) {
    return v - normal * 2.0f * dot(v, normal);
}

float3 Math::project(const float3& v, const float3& normal) {
    const float magSqr = normal.magnitudeSqr();
    if (isEqual(magSqr, 0.0f)) {
        return float3::zero();
    }
    return normal * (dot(v, normal) / magSqr);
}

float3 Math::rotate(const float3& toRotate, float radians, const float3& axis) {
    const float3 naxis = normalize(axis);
    const float cosVal = cos(radians);
    const float3 v1 = toRotate * cosVal;
    const float3 v2 = cross(naxis, toRotate) * sin(radians);
    const float3 v3 = naxis * dot(naxis, toRotate) * (-1.0f - cosVal);
    return v1 + v2 + v3;
}

float3 Math::Interpolation::interpolate(const float3& start, const float3& end, double t, Type type, Options options) {
    return {
        static_cast<float>(interpolate(static_cast<double>(start.x), static_cast<double>(end.x), t, type, options)),
        static_cast<float>(interpolate(static_cast<double>(start.y), static_cast<double>(end.y), t, type, options)),
        static_cast<float>(interpolate(static_cast<double>(start.z), static_cast<double>(end.z), t, type, options))
    };
}


///////////////////////////////////////////////
// float4
///////////////////////////////////////////////

bool Math::isNormalized(const float4& v) {
    return isEqual(v.magnitudeSqr(), 1.0f);
}

float4 Math::normalize(const float4& v) {
    if (v.isNormalized()) return v;
    if (v.magnitudeSqr() == 0.0f) return float4::zero();
    return v / v.magnitude();
}

float4 Math::normalizeFast(const float4& v, unsigned int iterationCount) {
    if (v.isNormalized()) return v;
    if (v.magnitudeSqr() == 0.0f) return float4::zero();
    return v / magnitudeFast(v, iterationCount);
}

float Math::magnitudeFast(const float4& v, unsigned int iterationCount) {
    return sqrtFast(v.magnitudeSqr(), iterationCount);
}

float Math::dot(const float4& v1, const float4& v2) {
    const float4 n1 = normalize(v1);
    const float4 n2 = normalize(v2);
    return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z + n1.w * n2.w;
}

float Math::distance(const float4& v1, const float4& v2) {
    return (v1 - v2).magnitude();
}

float Math::distanceSqr(const float4& v1, const float4& v2) {
    return (v1 - v2).magnitudeSqr();
}

float3 Math::projectTo3D(const float4& v) {
    return isEqual(v.w, 0.0f) ? v.xyz() : v.xyz() / v.w;
}

float Math::distanceToPlane(const float4& plane, const float3& point) {
    const float4 pn = normalize(plane);
    return dot(pn, float4{ point, 1.0f });
}

float4 Math::Interpolation::interpolate(const float4& start, const float4& end, double t, Type type, Options options) {
    return {
        static_cast<float>(interpolate(static_cast<double>(start.x), static_cast<double>(end.x), t, type, options)),
        static_cast<float>(interpolate(static_cast<double>(start.y), static_cast<double>(end.y), t, type, options)),
        static_cast<float>(interpolate(static_cast<double>(start.z), static_cast<double>(end.z), t, type, options)),
        static_cast<float>(interpolate(static_cast<double>(start.w), static_cast<double>(end.w), t, type, options))
    };
}