#pragma once

#include <Maths/float3.h>

#include <EASTL/optional.h>

namespace eloo {

struct Ray {
    struct HitInfo {
        eastl::optional<float3> hitPosition = eastl::nullopt;
    };

    float3 origin;
    float3 direction;
    float length = 0.0f;

    HitInfo castPlane(const float3& origin, const float3& normal) const;
    HitInfo castQuad(const float3& origin, const float3& normal) const;
    HitInfo castTri(const float3& vertex1, const float3& vertex2, const float3& vertex3) const;
    HitInfo castAABB(const float3& minValues, const float3& maxValues) const;
    HitInfo castSphere(const float3& origin, float radius) const;
    HitInfo castEllipsoid(const float3& origin, const float3& radii) const;
    HitInfo castCapsule(const float3& top, const float3& bottom, float radius) const;

    HitInfo sphereCastPlane(float castRadius, const float3& origin, const float3& normal) const;
    HitInfo sphereCastQuad(float castRadius, const float3& origin, const float3& normal) const;
    HitInfo sphereCastTri(float castRadius, const float3& vertex1, const float3& vertex2, const float3& vertex3) const;
    HitInfo sphereCastAABB(float castRadius, const float3& minValues, const float3& maxValues) const;
    HitInfo sphereCastSphere(float castRadius, const float3& origin, float radius) const;
    HitInfo sphereCastEllipsoid(float castRadius, const float3& origin, const float3& radii) const;
    HitInfo sphereCastCapsule(float castRadius, const float3& top, const float3& bottom, float radius) const;
};

}; // namespace Math