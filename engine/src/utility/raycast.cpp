#include "utility/raycast.h"

#include "maths/math.h"


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
// Raycasts
//
// ==================================================================
// ==================================================================

namespace eloo::raycast {

    /////////////////////////////////////////////////////////////////////
    // Plane

    bool test_plane(ELOO_RAYCAST_PARAMS_1, const float4::values& plane, result& hit) {
        hit = result();

        const float3::values normRayDir = math::vector::normalize(rayDir);
        const float3::values planeNormal = math::vector::normalize(plane.xyz());

        float denom = math::vector::dot(normRayDir, planeNormal);
        if (math::is_close_to_zero(math::abs(denom))) {
            return false;
        }

        const float numerator = math::vector::dot(rayOrigin, planeNormal) + plane.w();
        const float t = -numerator / denom;
        if (t < 0.0f || t > rayLength) {
            return false;
        }

        hit.distance = t;
        hit.position = rayOrigin + normRayDir * t;
        return true;
    }
    bool test_plane(ELOO_RAYCAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(plane), result& hit) {
        return test_plane(ELOO_RAYCAST_FORWARD_PARAMS_2, { FLOAT4_FORWARD_PARAMS(plane) }, hit);
    }
    bool test_plane(ELOO_RAYCAST_PARAMS_3, const float4::values& plane, result& hit) {
        return test_plane(ELOO_RAYCAST_FORWARD_PARAMS_3, plane, hit);
    }
    bool test_plane(ELOO_RAYCAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(plane), result& hit) {
        return test_plane(ELOO_RAYCAST_FORWARD_PARAMS_4, { FLOAT4_FORWARD_PARAMS(plane) }, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Quad

    bool test_quad(ELOO_RAYCAST_PARAMS_1, const float4::values& quad, float width, float height, result& hit) {
        if (!test_plane(rayOrigin, rayDir, quad, hit)) {
            return false;
        }

        float3::values normalizedQuad = math::vector::normalize(quad).xyz();
        const float upMag = math::vector::magnitude_sqr(normalizedQuad * float3::UP);
        const float3::values up = upMag > 0.0f ? float3::UP : float3::FORWARD;
        const float3::values U = math::vector::cross(up, normalizedQuad);
        const float3::values V = math::vector::cross(normalizedQuad, U);

        const float3::values P0 = normalizedQuad * (-quad.w() / math::vector::dot(normalizedQuad, normalizedQuad));
        const float3::values posOffset = hit.position - rayOrigin;

        float dotU = math::vector::dot(posOffset, U);
        float dotV = math::vector::dot(posOffset, V);
        if (math::abs(dotU) <= width * 0.5f && math::abs(dotV) <= height * 0.5f) {
            return true;
        }

        hit = result();
        return false;
    }
    bool test_quad(ELOO_RAYCAST_PARAMS_2, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& hit) {
        return test_quad(ELOO_RAYCAST_FORWARD_PARAMS_2, { FLOAT4_FORWARD_PARAMS(quad) }, width, height, hit);
    }
    bool test_quad(ELOO_RAYCAST_PARAMS_3, const float4::values& quad, float width, float height, result& hit) {
        return test_quad(ELOO_RAYCAST_FORWARD_PARAMS_3, quad, width, height, hit);
    }
    bool test_quad(ELOO_RAYCAST_PARAMS_4, FLOAT4_DECLARE_PARAMS(quad), float width, float height, result& hit) {
        return test_quad(ELOO_RAYCAST_FORWARD_PARAMS_4, { FLOAT4_FORWARD_PARAMS(quad) }, width, height, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Triangle

    bool test_tri(ELOO_RAYCAST_PARAMS_1, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& hit) {
        hit = result();

        const float3::values edge1 = vertex2 - vertex1;
        const float3::values edge2 = vertex3 - vertex1;
        const float3::values P = math::vector::cross(rayDir, edge2);
        const float det = math::vector::dot(edge1, P);
        if (math::is_close_to_zero(det)) {
            return false;
        }

        const float invDet = 1.0f / det;
        const float3::values T = rayOrigin - vertex1;
        const float u = math::vector::dot(T, P) * invDet;
        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        const float3::values Q = math::vector::cross(T, edge1);
        const float v = math::vector::dot(rayDir, Q) * invDet;
        float t = math::vector::dot(rayDir, Q) * invDet;
        if (t < 0.0f || t > rayLength) {
            return false;
        }

        hit.distance = t;
        hit.position = rayOrigin + rayDir * t;
        return true;
    }
    bool test_tri(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& hit) {
        return test_tri(ELOO_RAYCAST_FORWARD_PARAMS_2, { FLOAT3_FORWARD_PARAMS(vertex1) }, { FLOAT3_FORWARD_PARAMS(vertex2) }, { FLOAT3_FORWARD_PARAMS(vertex3) }, hit);
    }
    bool test_tri(ELOO_RAYCAST_PARAMS_3, const float3::values& vertex1, const float3::values& vertex2, const float3::values& vertex3, result& hit) {
        return test_tri(ELOO_RAYCAST_FORWARD_PARAMS_3, vertex1, vertex2, vertex3, hit);
    }
    bool test_tri(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(vertex1), FLOAT3_DECLARE_PARAMS(vertex2), FLOAT3_DECLARE_PARAMS(vertex3), result& hit) {
        return test_tri(ELOO_RAYCAST_FORWARD_PARAMS_4, { FLOAT3_FORWARD_PARAMS(vertex1) }, { FLOAT3_FORWARD_PARAMS(vertex2) }, { FLOAT3_FORWARD_PARAMS(vertex3) }, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // AABB

    bool test_aabb(ELOO_RAYCAST_PARAMS_1, const float3::values& min, const float3::values& max, result& hit) {
        hit = result();

        float tmin = (min.x() - rayOrigin.x()) / rayDir.x();
        float tmax = (max.x() - rayOrigin.x()) / rayDir.x();
        if (tmin > tmax) {
            std::swap(tmin, tmax);
        }

        float tymin = (min.y() - rayOrigin.y()) / rayDir.y();
        float tymax = (max.y() - rayOrigin.y()) / rayDir.y();
        if (tymin > tymax) {
            std::swap(tymin, tymax);
        }

        if ((tmin > tymax) || (tymin > tmax)) {
            return false;
        }
        tmin = math::max(tmin, tymin);
        tmax = math::min(tmax, tymax);

        float tzmin = (min.z() - rayOrigin.z()) / rayDir.z();
        float tzmax = (max.z() - rayOrigin.z()) / rayDir.z();
        if (tzmin > tzmax) {
            std::swap(tzmin, tzmax);
        }

        if ((tmin > tzmax) || (tzmin > tmax)) {
            return false;
        }
        tmin = math::max(tmin, tzmin);
        tmax = math::min(tmax, tzmax);

        float t = (tmin >= 0.0f) ? tmin : tmax;
        if (t < 0.0f || t > rayLength) {
            return false;
        }

        hit.distance = t;
        hit.position = rayOrigin + rayDir * t;
        return true;
    }
    bool test_aabb(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& hit) {
        return test_aabb(ELOO_RAYCAST_FORWARD_PARAMS_2, { FLOAT3_FORWARD_PARAMS(min) }, { FLOAT3_FORWARD_PARAMS(max) }, hit);
    }
    bool test_aabb(ELOO_RAYCAST_PARAMS_3, const float3::values& min, const float3::values& max, result& hit) {
        return test_aabb(ELOO_RAYCAST_FORWARD_PARAMS_3, min, max, hit);
    }
    bool test_aabb(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(min), FLOAT3_DECLARE_PARAMS(max), result& hit) {
        return test_aabb(ELOO_RAYCAST_FORWARD_PARAMS_4, { FLOAT3_FORWARD_PARAMS(min) }, { FLOAT3_FORWARD_PARAMS(max) }, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Sphere

    bool test_sphere(ELOO_RAYCAST_PARAMS_1, const float3::values& origin, float radius, result& hit) {
        hit = result();

        float3::values L = rayOrigin - origin;
        const float a = math::vector::dot(rayDir, rayDir);
        const float b = 2.0f * math::vector::dot(L, rayDir);
        const float c = math::vector::dot(L, L) - radius * radius;
        const float disc = b * b - 4.0f * a * c;
        if (disc < 0.0f) {
            return false;
        }

        const float sqrtDisc = math::sqrt(disc);
        const float t0 = (-b - sqrtDisc) / (2.0f * a);
        const float t1 = (-b + sqrtDisc) / (2.0f * a);
        const float t = (t0 > 0.0f ? t0 : (t1 >= 0.0f ? t1 : -1.0f));
        if (t < 0.0f || t > rayLength) {
            return false;
        }

        hit.distance = t;
        hit.position = rayOrigin + rayDir * t;
        return true;
    }
    bool test_sphere(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float radius, result& hit) {
        return test_sphere(ELOO_RAYCAST_FORWARD_PARAMS_2, { FLOAT3_FORWARD_PARAMS(origin) }, radius, hit);
    }
    bool test_sphere(ELOO_RAYCAST_PARAMS_3, const float3::values& origin, float radius, result& hit) {
        return test_sphere(ELOO_RAYCAST_FORWARD_PARAMS_3, origin, radius, hit);
    }
    bool test_sphere(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float radius, result& hit) {
        return test_sphere(ELOO_RAYCAST_FORWARD_PARAMS_4, { FLOAT3_FORWARD_PARAMS(origin) }, radius, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Ellipsoid

    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_1, const float3::values& origin, const float3::values& radii, result& hit) {
        hit = result();

        const float3::values L = rayOrigin - origin;

        const float invRadiiX = 1.0f / (radii.x() * radii.x());
        const float invRadiiY = 1.0f / (radii.y() * radii.y());
        const float invRadiiZ = 1.0f / (radii.z() * radii.z());
        const float a = (rayDir.x() * rayDir.x()) * invRadiiX + (rayDir.y() * rayDir.y()) * invRadiiY + (rayDir.z() * rayDir.z()) * invRadiiZ;
        const float b = 2.0f * ((L.x() * rayDir.x()) * invRadiiX + (L.y() * rayDir.y()) * invRadiiY + (L.z() * rayDir.z()) * invRadiiZ);
        const float c = (L.x() * L.x()) * invRadiiX + (L.y() * L.y()) * invRadiiY + (L.z() * L.z()) * invRadiiZ - 1.0f;
        const float disc = b * b - 4.0f * a * c;
        if (disc < 0.0f) {
            return false;
        }

        const float sqrtDisc = math::sqrt(disc);
        const float t0 = (-b - sqrtDisc) / (2.0f * a);
        const float t1 = (-b + sqrtDisc) / (2.0f * a);
        const float t = (t0 > 0.0f ? t0 : (t1 >= 0.0f ? t1 : -1.0f));
        if (t < 0.0f || t > rayLength) {
            return false;
        }

        hit.distance = t;
        hit.position = rayOrigin + rayDir * t;
        return true;
    }
    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& hit) {
        return test_ellipsoid(ELOO_RAYCAST_FORWARD_PARAMS_2, { FLOAT3_FORWARD_PARAMS(origin) }, { FLOAT3_FORWARD_PARAMS(radii) }, hit);
    }
    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_3, const float3::values& origin, const float3::values& radii, result& hit) {
        return test_ellipsoid(ELOO_RAYCAST_FORWARD_PARAMS_3, origin, radii, hit);
    }
    bool test_ellipsoid(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), FLOAT3_DECLARE_PARAMS(radii), result& hit) {
        return test_ellipsoid(ELOO_RAYCAST_FORWARD_PARAMS_4, { FLOAT3_FORWARD_PARAMS(origin) }, { FLOAT3_FORWARD_PARAMS(radii) }, hit);
    }


    /////////////////////////////////////////////////////////////////////
    // Capsule

    bool test_capsule(ELOO_RAYCAST_PARAMS_1, const float3::values& origin, float height, float radius, result& hit) {
        hit = result();

        const float3::values P0 = origin - float3::UP * height * 0.5f;
        const float3::values P1 = P0 + float3::UP * height;

        // 1) Cylinder body
        const float ox = rayOrigin.x() - P0.x();
        const float oz = rayOrigin.z() - P0.z();
        const float a = rayDir.x() * rayDir.x() + rayDir.z() * rayDir.z();
        if (a > 0.0f) {
            const float b = 2.0f * (ox * rayDir.x() + oz * rayDir.z());
            const float c = ox * ox + oz * oz - radius * radius;
            const float disc = b * b - 4.0f * a * c;
            if (disc >= 0.0f) {
                const float sqrtDisc = math::sqrt(disc);
                for (float t : { (-b - sqrtDisc) / (2.0f * a), (-b + sqrtDisc) / (2.0f * a) }) {
                    if (t < 0.0f || t > rayLength) {
                        continue;
                    }
                    const float y = rayOrigin.y() + rayDir.y() * t;
                    if (y >= P0.y() && y <= P1.y()) {
                        hit.distance = t;
                        hit.position = rayOrigin + rayDir * t;
                        return true;
                    }
                }
            }
        }

        // 2) Sphere caps
        const auto testSphere = [&](const float3::values& sphereOrigin) {
            result sphereHit;
            const bool didHit = test_sphere(rayOrigin, rayDir, sphereOrigin, radius, sphereHit);
            if (didHit && (hit.distance < 0.0f || sphereHit.distance < hit.distance)) {
                hit = sphereHit;
            }
        };

        testSphere(P0);
        testSphere(P1);

        return hit.distance >= 0.0f;
    }
    bool test_capsule(ELOO_RAYCAST_PARAMS_2, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& hit) {
        return test_capsule(ELOO_RAYCAST_FORWARD_PARAMS_2, { FLOAT3_FORWARD_PARAMS(origin) }, height, radius, hit);
    }
    bool test_capsule(ELOO_RAYCAST_PARAMS_3, const float3::values& origin, float height, float radius, result& hit) {
        return test_capsule(ELOO_RAYCAST_FORWARD_PARAMS_3, origin, height, radius, hit);
    }
    bool test_capsule(ELOO_RAYCAST_PARAMS_4, FLOAT3_DECLARE_PARAMS(origin), float height, float radius, result& hit) {
        return test_capsule(ELOO_RAYCAST_FORWARD_PARAMS_4, { FLOAT3_FORWARD_PARAMS(origin) }, height, radius, hit);
    }
};