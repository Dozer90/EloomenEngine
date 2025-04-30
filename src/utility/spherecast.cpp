#include <utility/spherecast.h>
#include <utility/raycast.h>

#include <maths/math.h>


#define ELOO_RAYCAST_PARAMS_1 const float3::values& rayOrigin, const float3::values& rayDir, float rayLength
#define ELOO_RAYCAST_PARAMS_2 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir), float rayLength
#define ELOO_RAYCAST_PARAMS_3 const float3::values& rayOrigin, const float3::values& rayDir
#define ELOO_RAYCAST_PARAMS_4 FLOAT3_DECLARE_PARAMS(rayOrigin), FLOAT3_DECLARE_PARAMS(rayDir)

#define ELOO_RAYCAST_FORWARD_PARAMS_1 rayOrigin, rayDir, rayLength
#define ELOO_RAYCAST_FORWARD_PARAMS_2 { FLOAT3_FORWARD_PARAMS(rayOrigin) }, { FLOAT3_FORWARD_PARAMS(rayDir) }, rayLength
#define ELOO_RAYCAST_FORWARD_PARAMS_3 rayOrigin, rayDir, FLT_MAX
#define ELOO_RAYCAST_FORWARD_PARAMS_4 { FLOAT3_FORWARD_PARAMS(rayOrigin) }, { FLOAT3_FORWARD_PARAMS(rayDir) }, FLT_MAX


// ==================================================================
// ==================================================================
//
// Spherecasts
//
// ==================================================================
// ==================================================================

namespace eloo::spherecast {

    /////////////////////////////////////////////////////////////////////
    // Plane

    bool test_plane(ELOO_RAYCAST_PARAMS_1, float castRadius, const float4::values& plane, result& hit) {
        const float4::values planeOffset = { plane.x(), plane.y(), plane.z(), plane.w() + castRadius };
        return raycast::test_plane(ELOO_RAYCAST_FORWARD_PARAMS_1, planeOffset, hit);
    }
    bool test_plane(ELOO_RAYCAST_PARAMS_2, float castRadius, FLOAT4_DECLARE_PARAMS(plane), result& hit) {
        return test_plane(ELOO_RAYCAST_FORWARD_PARAMS_2, castRadius, { FLOAT4_FORWARD_PARAMS(plane) }, hit);
    }
    bool test_plane(ELOO_RAYCAST_PARAMS_3, float castRadius, const float4::values& plane, result& hit) {
        return test_plane(ELOO_RAYCAST_FORWARD_PARAMS_3, castRadius, plane, hit);
    }
    bool test_plane(ELOO_RAYCAST_PARAMS_4, float castRadius, FLOAT4_DECLARE_PARAMS(plane), result& hit) {
        return test_plane(ELOO_RAYCAST_FORWARD_PARAMS_4, castRadius, { FLOAT4_FORWARD_PARAMS(plane) }, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Quad

    bool test_quad(ELOO_RAYCAST_PARAMS_1, float castRadius, const float4::values& quad, float width, float height, result& hit) {
        // First do a sphere-swept plane:
        if (!test_plane(ELOO_RAYCAST_FORWARD_PARAMS_1, castRadius, quad, hit)) {
            return false;
        }

        // Reconstruct quad axes (same as in raycast):
        const auto n = math::vector::normalize(quad).xyz();
        const float3::values up = (math::vector::magnitude_sqr(n * float3::UP) > 0.0f)
            ? float3::UP
            : float3::FORWARD;
        const float3::values U = math::vector::cross(up, n);
        const float3::values V = math::vector::cross(n, U);

        // Measure offset from plane intersection:
        const float3::values P0 = n * (-quad.w() / math::vector::dot(n, n));
        const float3::values posOffset = hit.position - (rayOrigin + n * castRadius);

        // Widen the quad by castRadius:
        const float halfW = 0.5f * width + castRadius;
        const float halfH = 0.5f * height + castRadius;

        const float dotU = math::vector::dot(posOffset, U);
        const float dotV = math::vector::dot(posOffset, V);
        return (math::abs(dotU) <= halfW && math::abs(dotV) <= halfH);
    }
    bool test_quad(ELOO_RAYCAST_PARAMS_2, float castRadius, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& hit) {
        return test_quad(ELOO_RAYCAST_FORWARD_PARAMS_2, castRadius, { FLOAT4_FORWARD_PARAMS(quad) }, width, height, hit);
    }
    bool test_quad(ELOO_RAYCAST_PARAMS_3, float castRadius, const float4::values& quad, float width, float height, result& hit) {
        return test_quad(ELOO_RAYCAST_FORWARD_PARAMS_3, castRadius, quad, width, height, hit);
    }
    bool test_quad(ELOO_RAYCAST_PARAMS_4, float castRadius, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& hit) {
        return test_quad(ELOO_RAYCAST_FORWARD_PARAMS_4, castRadius, { FLOAT4_FORWARD_PARAMS(quad) }, width, height, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Triangle

    bool test_tri(ELOO_RAYCAST_PARAMS_1, float castRadius, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& hit) {
        hit = result();
        result testResult;

        // 1) Face - sphere-inflated plane test
        {
            // Build the triangle plane
            const float3::values edge1 = vertex2 - vertex1;
            const float3::values edge2 = vertex3 - vertex1;
            const float3::values normal = math::vector::normalize(math::vector::cross(edge1, edge2));
            const float4::values plane = { normal.x(), normal.y(), normal.z(), -math::vector::dot(normal, vertex1) };

            if (test_plane(ELOO_RAYCAST_FORWARD_PARAMS_1, castRadius, plane, testResult)) {
                // Pull intersection point back to the triangle plane
                const float3::values hitPoint = testResult.position - normal * castRadius;

                // Barycentric sign test
                const auto signOk = [&](const float3::values& A, const float3::values& B) {
                    const float3::values C = math::vector::cross(B - A, hitPoint - A);
                    return math::vector::dot(normal, C) >= 0.0f;
                };
                if (signOk(vertex1, vertex2) && signOk(vertex2, vertex3) && signOk(vertex3, vertex1)) {
                    hit = testResult;
                }
            }
        }

        const auto storeBest = [&](const result& toTest) {
            if (toTest.distance >= 0.0f && toTest.distance < hit.distance) {
                hit.distance = toTest.distance;
                hit.position = toTest.distance;
            }
        };

        // 2) Edges - capsule vs. ray for each segment
        for (const auto& [edgeStart, edgeEnd] : { std::pair(vertex1, vertex2), std::pair(vertex2, vertex3), std::pair(vertex3, vertex1) }) {
            const float3::values edgeDir = edgeEnd - edgeStart;
            const float edgeLength = math::vector::magnitude(edgeDir);
            if (edgeLength < math::f32::EPSILON) {
                continue; // Degenerate edge
            }

            const float3::values U = edgeDir / edgeLength;
            const float3::values& V = rayDir;
            const float3::values w0 = rayOrigin - edgeStart;
            const float w0u = math::vector::dot(w0, U);
            const float Vu = math::vector::dot(V, U);
            const float3::values wP = w0 - U * w0u;
            const float3::values vP = V - U * Vu;

            const float a = math::vector::dot(vP, vP);
            const float b = 2.0f * math::vector::dot(wP, vP);
            const float c = math::vector::dot(wP, wP) - castRadius * castRadius;

            if (a > 0.0f) {
                const float disc = b * b - 4.0f * a * c;
                if (disc >= 0.0f) {
                    const float sqrtDisc = math::sqrt(disc);
                    for (float t : { (-b - sqrtDisc) / (2.0f * a), (-b + sqrtDisc) / (2.0f * a) }) {
                        if (t < 0.0f || t > rayLength) {
                            continue;
                        }
                        const float s = w0u + Vu * t;
                        if (s >= 0.0f && s <= edgeLength) {
                            testResult = { t, rayOrigin + rayDir * t };
                            storeBest(testResult);
                        }
                    }
                }
            }

            // 3) Sphere caps
            for (const auto& sphereOrigin : { edgeStart, edgeEnd }) {
                raycast::result raycastResult;
                if (raycast::test_sphere(ELOO_RAYCAST_FORWARD_PARAMS_1, sphereOrigin, castRadius, raycastResult)) {
                    storeBest({ raycastResult.distance, raycastResult.position });
                }
            }
        }

        return hit.distance >= 0.0f;
    }
    bool test_tri(ELOO_RAYCAST_PARAMS_2, float castRadius, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& hit) {
        return test_tri(ELOO_RAYCAST_FORWARD_PARAMS_2, castRadius, { FLOAT3_FORWARD_PARAMS(vertex1) }, { FLOAT3_FORWARD_PARAMS(vertex2) }, { FLOAT3_FORWARD_PARAMS(vertex3) }, hit);
    }
    bool test_tri(ELOO_RAYCAST_PARAMS_3, float castRadius, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& hit) {
        return test_tri(ELOO_RAYCAST_FORWARD_PARAMS_3, castRadius, vertex1, vertex2, vertex3, hit);
    }
    bool test_tri(ELOO_RAYCAST_PARAMS_4, float castRadius, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& hit) {
        return test_tri(ELOO_RAYCAST_FORWARD_PARAMS_4, castRadius, { FLOAT3_FORWARD_PARAMS(vertex1) }, { FLOAT3_FORWARD_PARAMS(vertex2) }, { FLOAT3_FORWARD_PARAMS(vertex3) }, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // AABB

    bool test_aabb(ELOO_RAYCAST_PARAMS_1, float castRadius, const float3::values& min, const float3::values& max, result& hit) {
        const float3::values lo = min - float3::ONE * castRadius;
        const float3::values hi = max + float3::ONE * castRadius;
        return raycast::test_aabb(ELOO_RAYCAST_FORWARD_PARAMS_1, lo, hi, hit);
    }
    bool test_aabb(ELOO_RAYCAST_PARAMS_2, float castRadius, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& hit) {
        return test_aabb(ELOO_RAYCAST_FORWARD_PARAMS_2, castRadius, { FLOAT3_FORWARD_PARAMS(min) }, { FLOAT3_FORWARD_PARAMS(max) }, hit);
    }
    bool test_aabb(ELOO_RAYCAST_PARAMS_3, float castRadius, const float3::values& min, const float3::values& max, result& hit) {
        return test_aabb(ELOO_RAYCAST_FORWARD_PARAMS_3, castRadius, min, max, hit);
    }
    bool test_aabb(ELOO_RAYCAST_PARAMS_4, float castRadius, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& hit) {
        return test_aabb(ELOO_RAYCAST_FORWARD_PARAMS_4, castRadius, { FLOAT3_FORWARD_PARAMS(min) }, { FLOAT3_FORWARD_PARAMS(max) }, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Sphere

    bool test_sphere(ELOO_RAYCAST_PARAMS_1, float castRadius, const float3::values& origin, float radius, result& hit) {
        return raycast::test_sphere(ELOO_RAYCAST_FORWARD_PARAMS_1, origin, radius + castRadius, hit);
    }
    bool test_sphere(ELOO_RAYCAST_PARAMS_2, float castRadius, FLOAT3_DECLARE_PARAMS(origin), float radius, result& hit) {
        return test_sphere(ELOO_RAYCAST_FORWARD_PARAMS_2, castRadius, { FLOAT3_FORWARD_PARAMS(origin) }, radius, hit);
    }
    bool test_sphere(ELOO_RAYCAST_PARAMS_3, float castRadius, const float3::values& origin, float radius, result& hit) {
        return test_sphere(ELOO_RAYCAST_FORWARD_PARAMS_3, castRadius, origin, radius, hit);
    }
    bool test_sphere(ELOO_RAYCAST_PARAMS_4, float castRadius, FLOAT3_DECLARE_PARAMS(origin), float radius, result& hit) {
        return test_sphere(ELOO_RAYCAST_FORWARD_PARAMS_4, castRadius, { FLOAT3_FORWARD_PARAMS(origin) }, radius, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Ellipsoid

    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_1, float castRadius, const float3::values& origin, const float3::values& radii, result& hit) {
        return raycast::test_ellipsoid(ELOO_RAYCAST_FORWARD_PARAMS_1, origin, radii + float3::ONE * castRadius, hit);
    }
    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_2, float castRadius, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& hit) {
        return test_ellipsoid(ELOO_RAYCAST_FORWARD_PARAMS_2, castRadius, { FLOAT3_FORWARD_PARAMS(origin) }, { FLOAT3_FORWARD_PARAMS(radii) }, hit);
    }
    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_3, float castRadius, const float3::values& origin, const float3::values& radii, result& hit) {
        return test_ellipsoid(ELOO_RAYCAST_FORWARD_PARAMS_3, castRadius, origin, radii, hit);
    }
    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_4, float castRadius, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& hit) {
        return test_ellipsoid(ELOO_RAYCAST_FORWARD_PARAMS_4, castRadius, { FLOAT3_FORWARD_PARAMS(origin) }, { FLOAT3_FORWARD_PARAMS(radii) }, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Capsule

    bool test_capsule(ELOO_RAYCAST_PARAMS_1, float castRadius, const float3::values& origin, float height, float radius, result& hit) {
        return raycast::test_capsule(ELOO_RAYCAST_FORWARD_PARAMS_1, origin, height + castRadius, radius + castRadius, hit);
    }
    bool test_capsule(ELOO_RAYCAST_PARAMS_2, float castRadius, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& hit) {
        return test_capsule(ELOO_RAYCAST_FORWARD_PARAMS_2, castRadius, { FLOAT3_FORWARD_PARAMS(origin) }, height, radius, hit);
    }
    bool test_capsule(ELOO_RAYCAST_PARAMS_3, float castRadius, const float3::values& origin, float height, float radius, result& hit) {
        return test_capsule(ELOO_RAYCAST_FORWARD_PARAMS_3, castRadius, origin, height, radius, hit);
    }
    bool test_capsule(ELOO_RAYCAST_PARAMS_4, float castRadius, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& hit) {
        return test_capsule(ELOO_RAYCAST_FORWARD_PARAMS_4, castRadius, { FLOAT3_FORWARD_PARAMS(origin) }, height, radius, hit);
    }
};


#undef ELOO_RAYCAST_PARAMS_1
#undef ELOO_RAYCAST_PARAMS_2
#undef ELOO_RAYCAST_PARAMS_3
#undef ELOO_RAYCAST_PARAMS_4
#undef ELOO_RAYCAST_FORWARD_PARAMS_1
#undef ELOO_RAYCAST_FORWARD_PARAMS_2
#undef ELOO_RAYCAST_FORWARD_PARAMS_3
#undef ELOO_RAYCAST_FORWARD_PARAMS_4