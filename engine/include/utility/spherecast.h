#pragma once

#include "datatypes/float3.h"
#include "datatypes/float4.h"

#include <EASTL/optional.h>


namespace sge::raycast {
    struct result;
};

namespace sge::spherecast {
    using result = raycast::result;

#define SGE_SPHERECAST_PARAMS_1 const float3::values& rayOrigin, const float3::values& rayDir, float rayLength, float castRadius
#define SGE_SPHERECAST_PARAMS_2 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir), float rayLength, float castRadius
#define SGE_SPHERECAST_PARAMS_3 const float3::values& rayOrigin, const float3::values& rayDir, float castRadius
#define SGE_SPHERECAST_PARAMS_4 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir), float castRadius

    bool test_plane(SGE_SPHERECAST_PARAMS_1, const float4::values& plane, result& info);
    bool test_plane(SGE_SPHERECAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(plane), result& info);
    bool test_plane(SGE_SPHERECAST_PARAMS_3, const float4::values& plane, result& info);
    bool test_plane(SGE_SPHERECAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(plane), result& info);

    bool test_quad(SGE_SPHERECAST_PARAMS_1, const float4::values& quad, float width, float height, result& info);
    bool test_quad(SGE_SPHERECAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& info);
    bool test_quad(SGE_SPHERECAST_PARAMS_3, const float4::values& quad, float width, float height, result& info);
    bool test_quad(SGE_SPHERECAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& info);

    bool test_tri(SGE_SPHERECAST_PARAMS_1, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& info);
    bool test_tri(SGE_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& info);
    bool test_tri(SGE_SPHERECAST_PARAMS_3, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& info);
    bool test_tri(SGE_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& info);

    bool test_aabb(SGE_SPHERECAST_PARAMS_1, const float3::values& min, const float3::values& max, result& info);
    bool test_aabb(SGE_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& info);
    bool test_aabb(SGE_SPHERECAST_PARAMS_3, const float3::values& min, const float3::values& max, result& info);
    bool test_aabb(SGE_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& info);

    bool test_sphere(SGE_SPHERECAST_PARAMS_1, const float3::values& origin, float radius, result& info);
    bool test_sphere(SGE_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float radius, result& info);
    bool test_sphere(SGE_SPHERECAST_PARAMS_3, const float3::values& origin, float radius, result& info);
    bool test_sphere(SGE_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float radius, result& info);

    bool test_ellipsoid(SGE_SPHERECAST_PARAMS_1, const float3::values& origin, const float3::values& radii, result& info);
    bool test_ellipsoid(SGE_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& info);
    bool test_ellipsoid(SGE_SPHERECAST_PARAMS_3, const float3::values& origin, const float3::values& radii, result& info);
    bool test_ellipsoid(SGE_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& info);

    bool test_capsule(SGE_SPHERECAST_PARAMS_1, const float3::values& origin, float height, float radius, result& info);
    bool test_capsule(SGE_SPHERECAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& info);
    bool test_capsule(SGE_SPHERECAST_PARAMS_3, const float3::values& origin, float height, float radius, result& info);
    bool test_capsule(SGE_SPHERECAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& info);

#undef SGE_SPHERECAST_PARAMS_1
#undef SGE_SPHERECAST_PARAMS_2
#undef SGE_SPHERECAST_PARAMS_3
#undef SGE_SPHERECAST_PARAMS_4
};