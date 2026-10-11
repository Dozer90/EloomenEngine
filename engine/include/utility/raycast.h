#pragma once

#include "datatypes/float3.h"
#include "datatypes/float4.h"

#include <EASTL/optional.h>


namespace sge::raycast {
    struct result {
        float distance = FLT_MAX;
        float3::values position = float3::ZERO;
    };

#define SGE_RAYCAST_PARAMS_1 const float3::values& rayOrigin, const float3::values& rayDir, float rayLength
#define SGE_RAYCAST_PARAMS_2 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir), float rayLength
#define SGE_RAYCAST_PARAMS_3 const float3::values& rayOrigin, const float3::values& rayDir
#define SGE_RAYCAST_PARAMS_4 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir)

    bool test_plane(SGE_RAYCAST_PARAMS_1, const float4::values& plane, result& info);
    bool test_plane(SGE_RAYCAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(plane), result& info);
    bool test_plane(SGE_RAYCAST_PARAMS_3, const float4::values& plane, result& info);
    bool test_plane(SGE_RAYCAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(plane), result& info);

    bool test_quad(SGE_RAYCAST_PARAMS_1, const float4::values& quad, float width, float height, result& info);
    bool test_quad(SGE_RAYCAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& info);
    bool test_quad(SGE_RAYCAST_PARAMS_3, const float4::values& quad, float width, float height, result& info);
    bool test_quad(SGE_RAYCAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& info);

    bool test_tri(SGE_RAYCAST_PARAMS_1, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& info);
    bool test_tri(SGE_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& info);
    bool test_tri(SGE_RAYCAST_PARAMS_3, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& info);
    bool test_tri(SGE_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& info);

    bool test_aabb(SGE_RAYCAST_PARAMS_1, const float3::values& min, const float3::values& max, result& info);
    bool test_aabb(SGE_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& info);
    bool test_aabb(SGE_RAYCAST_PARAMS_3, const float3::values& min, const float3::values& max, result& info);
    bool test_aabb(SGE_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& info);

    bool test_sphere(SGE_RAYCAST_PARAMS_1, const float3::values& origin, float radius, result& info);
    bool test_sphere(SGE_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float radius, result& info);
    bool test_sphere(SGE_RAYCAST_PARAMS_3, const float3::values& origin, float radius, result& info);
    bool test_sphere(SGE_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float radius, result& info);

    bool test_ellipsoid(SGE_RAYCAST_PARAMS_1, const float3::values& origin, const float3::values& radii, result& info);
    bool test_ellipsoid(SGE_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& info);
    bool test_ellipsoid(SGE_RAYCAST_PARAMS_3, const float3::values& origin, const float3::values& radii, result& info);
    bool test_ellipsoid(SGE_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& info);

    bool test_capsule(SGE_RAYCAST_PARAMS_1, const float3::values& origin, float height, float radius, result& info);
    bool test_capsule(SGE_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& info);
    bool test_capsule(SGE_RAYCAST_PARAMS_3, const float3::values& origin, float height, float radius, result& info);
    bool test_capsule(SGE_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& info);

#undef SGE_RAYCAST_PARAMS_1
#undef SGE_RAYCAST_PARAMS_2
#undef SGE_RAYCAST_PARAMS_3
#undef SGE_RAYCAST_PARAMS_4
};