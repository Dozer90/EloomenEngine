#pragma once

#include <datatype/float3.h>
#include <datatype/float4.h>

#include <EASTL/optional.h>


namespace eloo::raycast {
    struct result {
        float distance = FLT_MAX;
        float3::values position = float3::ZERO;
    };

#define ELOO_RAYCAST_PARAMS_1 const float3::values& rayOrigin, const float3::values& rayDir, float rayLength
#define ELOO_RAYCAST_PARAMS_2 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir), float rayLength
#define ELOO_RAYCAST_PARAMS_3 const float3::values& rayOrigin, const float3::values& rayDir
#define ELOO_RAYCAST_PARAMS_4 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir)

    bool test_plane(ELOO_RAYCAST_PARAMS_1, const float4::values& plane, result& info);
    bool test_plane(ELOO_RAYCAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(plane), result& info);
    bool test_plane(ELOO_RAYCAST_PARAMS_3, const float4::values& plane, result& info);
    bool test_plane(ELOO_RAYCAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(plane), result& info);

    bool test_quad(ELOO_RAYCAST_PARAMS_1, const float4::values& quad, float width, float height, result& info);
    bool test_quad(ELOO_RAYCAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& info);
    bool test_quad(ELOO_RAYCAST_PARAMS_3, const float4::values& quad, float width, float height, result& info);
    bool test_quad(ELOO_RAYCAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& info);

    bool test_tri(ELOO_RAYCAST_PARAMS_1, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& info);
    bool test_tri(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& info);
    bool test_tri(ELOO_RAYCAST_PARAMS_3, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& info);
    bool test_tri(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& info);

    bool test_aabb(ELOO_RAYCAST_PARAMS_1, const float3::values& min, const float3::values& max, result& info);
    bool test_aabb(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& info);
    bool test_aabb(ELOO_RAYCAST_PARAMS_3, const float3::values& min, const float3::values& max, result& info);
    bool test_aabb(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& info);

    bool test_sphere(ELOO_RAYCAST_PARAMS_1, const float3::values& origin, float radius, result& info);
    bool test_sphere(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float radius, result& info);
    bool test_sphere(ELOO_RAYCAST_PARAMS_3, const float3::values& origin, float radius, result& info);
    bool test_sphere(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float radius, result& info);

    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_1, const float3::values& origin, const float3::values& radii, result& info);
    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& info);
    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_3, const float3::values& origin, const float3::values& radii, result& info);
    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& info);

    bool test_capsule(ELOO_RAYCAST_PARAMS_1, const float3::values& origin, float height, float radius, result& info);
    bool test_capsule(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& info);
    bool test_capsule(ELOO_RAYCAST_PARAMS_3, const float3::values& origin, float height, float radius, result& info);
    bool test_capsule(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& info);

#undef ELOO_RAYCAST_PARAMS_1
#undef ELOO_RAYCAST_PARAMS_2
#undef ELOO_RAYCAST_PARAMS_3
#undef ELOO_RAYCAST_PARAMS_4
};