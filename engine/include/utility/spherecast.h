#pragma once

#include "datatypes/float3.h"
#include "datatypes/float4.h"

#include <EASTL/optional.h>


namespace eloo::raycast {
    struct result;
};

namespace eloo::spherecast {
    using result = raycast::result;

#define ELOO_SPHERECAST_PARAMS_1 const float3::values& rayOrigin, const float3::values& rayDir, float rayLength, float castRadius
#define ELOO_SPHERECAST_PARAMS_2 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir), float rayLength, float castRadius
#define ELOO_SPHERECAST_PARAMS_3 const float3::values& rayOrigin, const float3::values& rayDir, float castRadius
#define ELOO_SPHERECAST_PARAMS_4 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir), float castRadius

    bool test_plane(ELOO_SPHERECAST_PARAMS_1, const float4::values& plane, result& info);
    bool test_plane(ELOO_SPHERECAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(plane), result& info);
    bool test_plane(ELOO_SPHERECAST_PARAMS_3, const float4::values& plane, result& info);
    bool test_plane(ELOO_SPHERECAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(plane), result& info);

    bool test_quad(ELOO_SPHERECAST_PARAMS_1, const float4::values& quad, float width, float height, result& info);
    bool test_quad(ELOO_SPHERECAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& info);
    bool test_quad(ELOO_SPHERECAST_PARAMS_3, const float4::values& quad, float width, float height, result& info);
    bool test_quad(ELOO_SPHERECAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& info);

    bool test_tri(ELOO_SPHERECAST_PARAMS_1, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& info);
    bool test_tri(ELOO_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& info);
    bool test_tri(ELOO_SPHERECAST_PARAMS_3, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& info);
    bool test_tri(ELOO_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& info);

    bool test_aabb(ELOO_SPHERECAST_PARAMS_1, const float3::values& min, const float3::values& max, result& info);
    bool test_aabb(ELOO_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& info);
    bool test_aabb(ELOO_SPHERECAST_PARAMS_3, const float3::values& min, const float3::values& max, result& info);
    bool test_aabb(ELOO_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& info);

    bool test_sphere(ELOO_SPHERECAST_PARAMS_1, const float3::values& origin, float radius, result& info);
    bool test_sphere(ELOO_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float radius, result& info);
    bool test_sphere(ELOO_SPHERECAST_PARAMS_3, const float3::values& origin, float radius, result& info);
    bool test_sphere(ELOO_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float radius, result& info);

    bool test_ellipsoid(ELOO_SPHERECAST_PARAMS_1, const float3::values& origin, const float3::values& radii, result& info);
    bool test_ellipsoid(ELOO_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& info);
    bool test_ellipsoid(ELOO_SPHERECAST_PARAMS_3, const float3::values& origin, const float3::values& radii, result& info);
    bool test_ellipsoid(ELOO_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& info);

    bool test_capsule(ELOO_SPHERECAST_PARAMS_1, const float3::values& origin, float height, float radius, result& info);
    bool test_capsule(ELOO_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& info);
    bool test_capsule(ELOO_SPHERECAST_PARAMS_3, const float3::values& origin, float height, float radius, result& info);
    bool test_capsule(ELOO_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& info);

#undef ELOO_SPHERECAST_PARAMS_1
#undef ELOO_SPHERECAST_PARAMS_2
#undef ELOO_SPHERECAST_PARAMS_3
#undef ELOO_SPHERECAST_PARAMS_4
};