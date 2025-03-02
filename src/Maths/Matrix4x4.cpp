#include <Maths/Matrix4x4.h>

#include <Maths/Math.h>
#include <Maths/float3.h>
#include <Maths/float4.h>
#include <Maths/Matrix3x3.h>
#include <Maths/Quaternion.h>

#include <Generic/Utilities.h>

using namespace eloo::Math;

namespace {
static constexpr Matrix4x4 gZero({ 0.0f }, { 0.0f }, { 0.0f }, { 0.0f });
static constexpr Matrix4x4 gOne({ 1.0f }, { 1.0f }, { 1.0f }, { 1.0f });
static constexpr Matrix4x4 gIdentity;
}

Matrix4x4 Matrix4x4::transpose(const Matrix4x4& m) {
    return { m.getColumn(0), m.getColumn(1), m.getColumn(2), m.getColumn(3) };
}

const Matrix4x4& Matrix4x4::zero() {
    return gZero;
}

const Matrix4x4& Matrix4x4::one() {
    return gOne;
}

const Matrix4x4& Matrix4x4::identity() {
    return gIdentity;
}

float Matrix4x4::determinant() const {
    float det = 0.0f;
    for (uint8_t r = 0; r < 4; ++r) {
        const int sign = 1 - (r % 2) * 2;
        det += cell[r][0] * Matrix3x3(*this, r, 0).determinant() * sign;
    }
    return det;
}

Matrix4x4 Matrix4x4::cofactor() const {
    Matrix4x4 cofactorMatrix;
    for (uint8_t r = 0; r < 4; ++r) {
        for (uint8_t c = 0; c < 4; ++c) {
            const int sign = 1 - ((r + c) % 2) * 2;
            cofactorMatrix[r][c] = Matrix3x3(*this, r, c).determinant();
        }
    }
    return cofactorMatrix;
}

Matrix4x4 Matrix4x4::adjugate() const {
    return transpose(cofactor());
}

Matrix4x4 Matrix4x4::inverse(const Matrix4x4& mat) {
    const float det = mat.determinant();
    if (is_close(det, 0.0f)) {
        return gZero;
    }
    return mat.adjugate() * (1.0f / det);
}

Matrix4x4 Matrix4x4::createTranslation(const float3& translation) {
    Matrix4x4 mat;
    mat[0][3] = translation.x;
    mat[1][3] = translation.y;
    mat[2][3] = translation.z;
    return mat;
}

Matrix4x4 Matrix4x4::createScale(const float3& scales) {
    Matrix4x4 mat;
    mat[0][0] = scales.x;
    mat[1][1] = scales.y;
    mat[2][2] = scales.z;
    return mat;
}

Matrix4x4 Matrix4x4::createRotation(float radians, const float3& axis) {
    const float3 a = normalize(axis);
    const float c = cos(radians);
    const float o = 1.0f - c;
    const float s = sin(radians);

    return {
        c + o * (a.x * a.x),        (o * a.x * a.y) - s * a.z,  (o * a.x * a.z) + s * a.y,  0.0f,
        (o * a.y * a.x) + s * a.z,  c + o * (a.y * a.y),        (o * a.y * a.z) - s * a.x,  0.0f,
        (o * a.z * a.x) - s * a.y,  (o * a.z * a.y) + s * a.x,  c + o * (a.z * a.z),        0.0f,
        0.0f,                       0.0f,                       0.0f,                       1.0f };
}

Matrix4x4 Matrix4x4::createRotation(const float3& euler) {
    // Intrinsic ZYX multiplication
    const float3 c = { cos(euler.x), cos(euler.y), cos(euler.z) };
    const float3 s = { sin(euler.x), sin(euler.y), sin(euler.z) };
    return {
        c.z * c.y,  -s.z,       s.y,        0.0f,
        s.z,        c.z * c.x,  -s.x,       0.0f,
        -s.y,       s.x,        c.y * c.x,  0.0f,
        0.0f,       0.0f,       0.0f,       1.0f };
}

Matrix4x4 Matrix4x4::createRotation(const Quaternion& rotation) {
    const float& w = rotation.w;
    const float& x = rotation.x;
    const float& y = rotation.y;
    const float& z = rotation.z;
    return {
        1.0f - 2.0f * (y * y + z * z),  2.0f * (x * y - z * w),         2.0f * (x * z + y * w),         0.0f,
        2.0f * (x * y - z + w),         1.0f - 2.0f * (x * x + z * z),  2.0f * (y * z - x * w),         0.0f,
        2.0f * (x * z - y * w),         2.0f * (y * z + x * w),         1.0f - 2.0f * (x * x + y * y),  0.0f,
        0.0f,                           0.0f,                           0.0f,                           1.0f };
}

Matrix4x4 Matrix4x4::createShearX(float shearing) {
    Matrix4x4 mat;
    mat[1][0] = shearing;
    mat[2][0] = shearing;
    return mat;
}

Matrix4x4 Matrix4x4::createShearY(float shearing) {
    Matrix4x4 mat;
    mat[0][1] = shearing;
    mat[2][1] = shearing;
    return mat;
}

Matrix4x4 Matrix4x4::createShearZ(float shearing) {
    Matrix4x4 mat;
    mat[0][2] = shearing;
    mat[1][2] = shearing;
    return mat;
}

Matrix4x4 Matrix4x4::createShear(float shearing, const float3& axis) {
    const float3 perpendicular = abs(axis.x) < 0.9f ? float3::right() : float3::up();
    const float4 plane = float4(normalize(cross(axis, perpendicular)), 1.0f);

    Matrix4x4 mat;
    mat.r1 += plane * axis.x * shearing;
    mat.r2 += plane * axis.y * shearing;
    mat.r3 += plane * axis.z * shearing;
    return mat;
}

Matrix4x4 Matrix4x4::createView(const float3& viewOrigin, const float3& viewForward, const float3& worldUp) {
	const float3& f = viewForward;
	const float3 r = normalize(cross(f, worldUp));
	const float3 u = cross(r, f);
	return {
		r.x, u.x, -f.x, -dot(viewOrigin, r),
		r.y, u.y, -f.y, -dot(viewOrigin, u),
		r.z, u.z, -f.z, -dot(viewOrigin, f),
		0.0f, 0.0f, 0.0f, 1.0f };
}

Matrix4x4 Matrix4x4::createOrthographicProjection(const float3& boundMin, const float3& boundMax) {
    Matrix4x4 mat;
    const float bw = boundMax.x - boundMin.x;
    const float bh = boundMax.y - boundMin.y;
    const float bd = boundMax.z - boundMin.z;
    mat[0][0] = bw * 0.5f; mat[0][3] = -((boundMax.x + boundMin.x) / bw);
    mat[1][1] = bh * 0.5f; mat[1][3] = -((boundMax.y + boundMin.y) / bh);
    mat[2][2] = bd * 0.5f; mat[2][3] = -((boundMax.z + boundMin.z) / bd);
    return mat;
}

Matrix4x4 Matrix4x4::createPerspectiveProjection(float fov, float aspectRatio, float nearClip, float farClip) {
    Matrix4x4 mat;
    const float fovT2 = tan(fov * 0.5f);
    const float depth = farClip - nearClip;
    mat[0][0] = 1.0f / (aspectRatio * fovT2);
    mat[1][1] = 1.0f / fovT2;
    mat[2][2] = -((farClip + nearClip) / depth);    mat[2][3] = -((2.0f * farClip * nearClip) / depth);
    mat[3][2] = -1.0f;
}

Matrix4x4 Matrix4x4::createLookAt(const float3& from, const float3& to, const float3& up) {
    const float3 f = normalize(to - from);
    const float3 r = normalize(cross(f, up));
    const float3 u = cross(r, f);
    return {
        r.x,  r.y,  r.z,  dot(-r, from),
        u.x,  u.y,  u.z,  dot(-u, from),
        -f.x, -f.y, -f.z, dot(f, from),
        0.0f, 0.0f, 0.0f, 1.0f };
}

Matrix4x4 Matrix4x4::createViewportTransformation() {

}