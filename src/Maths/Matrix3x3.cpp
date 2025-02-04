#include <Maths/Matrix3x3.h>

#include <Maths/Math.h>
#include <Maths/Matrix2x2.h>
#include <Maths/Matrix4x4.h>
#include <Maths/Vector2.h>
#include <Maths/Vector3.h>

#include <Generic/Utilities.h>

using namespace eloo::Math;

namespace {
static constexpr Matrix3x3 gZero(0.0f);
static constexpr Matrix3x3 gOne(1.0f);
static constexpr Matrix3x3 gIdentity;
}

Matrix3x3::Matrix3x3(const Matrix4x4& m, uint8_t row, uint8_t col) {
    uint8_t i = 0;
    for (uint8_t r = 0; r < 4; ++r) {
        if (r == row) continue;
        for (uint8_t c = 0; c < 4; ++c) {
            if (c == col) continue;
            mData[i++] = m[r][c];
        }
    }
}

Matrix3x3 Matrix3x3::transpose(const Matrix3x3& mat) {
    return {
        mat[0][0], mat[1][0], mat[2][0],
        mat[0][1], mat[1][1], mat[2][1],
        mat[0][2], mat[1][2], mat[2][2],
    };
}

float Matrix3x3::determinant() const {
    return get(0,0) * (get(1,1) * get(2,2) - get(1,2) * get(2,1)) -
           get(0,1) * (get(1,0) * get(2,2) - get(1,2) * get(2,0)) +
           get(0,2) * (get(1,0) * get(2,1) - get(1,1) * get(2,0));
}

Matrix3x3 Matrix3x3::cofactor() const {
    Matrix3x3 cofactorMatrix;
    for (uint8_t r = 0; r < 3; ++r) {
        for (uint8_t c = 0; c < 3; ++c) {
            cofactorMatrix[r][c] = Matrix2x2(*this, r, c).determinant();
        }
    }
    return cofactorMatrix;
}

Matrix3x3 Matrix3x3::adjugate() const {
    return transpose(cofactor());
}

Matrix3x3 Matrix3x3::inverse(const Matrix3x3& mat) {
    const float det = mat.determinant();
    if (isEqual(det, 0.0f)) {
        return gZero;
    }
    return mat.adjugate() * (1.0f / det);
}

const Matrix3x3& Matrix3x3::zero() {
    return gZero;
}

const Matrix3x3& Matrix3x3::one() {
    return gOne;
}

const Matrix3x3& Matrix3x3::identity() {
    return gIdentity;
}

Matrix3x3 Matrix3x3::createTranslation(const Vector2& translation) {
    Matrix3x3 mat;
    mat[0][2] = translation.x;
    mat[1][2] = translation.y;
    return mat;
}

Matrix3x3 Matrix3x3::createScale(const Vector2& scales) {
    Matrix3x3 mat;
    mat[0][0] = scales.x;
    mat[1][1] = scales.y;
    return mat;
}

Matrix3x3 Matrix3x3::createRotation(float radians) {
    Matrix3x3 mat;
    mat[0][0] = cos(radians);  mat[0][1] = -sin(radians);
    mat[1][0] = sin(radians);  mat[1][1] = cos(radians);
    return mat;
}

Matrix3x3 Matrix3x3::createShear(const Vector2& shearing) {
    Matrix3x3 mat;
    mat[0][1] = shearing.x;
    mat[1][0] = shearing.y;
    return mat;
}