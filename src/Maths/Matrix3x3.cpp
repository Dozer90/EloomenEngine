#include <Maths/Matrix3x3.h>

#include <Maths/Maths.h>
#include <Maths/Vector2.h>
#include <Maths/Vector3.h>

#include <Generic/Utilities.h>

using namespace eloo::Math;

namespace {
const Matrix3x3 gZeroMatrix3x3 = Matrix3x3() * 0;
}

Matrix3x3::Matrix3x3(const Vector2& scalar, float rotRadians, const Vector2& translation) {
    scale(scalar.x, scalar.y, *this);
    rotate(rotRadians, *this);
    translate(translation, *this);
}

Matrix3x3::Matrix3x3(std::initializer_list<float> data) {
    if (data.size() == 1) {
        eastl::fill(mData.begin(), mData.end(), *data.begin());
        return;
    }
    if (data.size() == 9) {
        eastl::copy(data.begin(), data.end(), mData.begin());
        return;
    }
    ASSERT_FALSE("Initilizer list for 'Matrix3x3' is not 1 or 9 in length. Cannot set values properly.");
}

constexpr Matrix3x3 Matrix3x3::zero() {
    return Matrix3x3()
}

Matrix3x3 Matrix3x3::createTranslation(float x, float y) {
    Matrix3x3 mat;
    mat.m13 = x;
    mat.m23 = y;
    return mat;
}

Matrix3x3 Matrix3x3::createScale(float x, float y) {
    Matrix3x3 mat;
    mat.m11 = x;
    mat.m22 = y;
    return mat;
}

Matrix3x3 Matrix3x3::createRotation(float radians) {
    Matrix3x3 mat;
    mat.m11 = cos(radians);  mat.m12 = -sin(radians);
    mat.m21 = sin(radians);  mat.m22 = cos(radians);
    return mat;
}

Matrix3x3 Matrix3x3::createShear(float x, float y) {
    Matrix3x3 mat;
    mat.m12 = x;
    mat.m21 = y;
    return mat;
}

bool Matrix3x3::operator!=(const Matrix3x3& lhs, const Matrix3x3& rhs) {
    static constexpr size_t sizeOfFloat = sizeof(float);
    for (size_t i = 0; i < sizeOfFloat * 9; i += sizeOfFloat) {
        if (abs(*(&lhs.m11 + i) - *(&rhs.m11 + i)) > FLT_EPSILON) {
            return true;
        }
    }
    return false;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& lhs, const Matrix3x3& rhs) {
    Matrix3x3 mat = lhs;
    mat.m11 *= rhs.m11; mat.m12 *= rhs.m12; mat.m13 *= rhs.m13;
    mat.m21 *= rhs.m21; mat.m22 *= rhs.m22; mat.m23 *= rhs.m23;
    mat.m31 *= rhs.m31; mat.m32 *= rhs.m32; mat.m33 *= rhs.m33;
    return mat;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& mat, float scalar) {
    Matrix3x3 matOut = mat;
    if (scalar == 0) {
        return zero();
    }
    if (scalar == 1) {
        return matOut;
    }
    matOut.m11 *= scalar; matOut.m12 *= scalar; matOut.m13 *= scalar;
    matOut.m21 *= scalar; matOut.m22 *= scalar; matOut.m23 *= scalar;
    matOut.m31 *= scalar; matOut.m32 *= scalar; matOut.m33 *= scalar;
    return matOut;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& mat, int scalar) {
    Matrix3x3 matOut = mat;
    if (scalar == 0) {
        return zero();
    }
    if (scalar == 1) {
        return matOut;
    }
    matOut.m11 *= scalar; matOut.m12 *= scalar; matOut.m13 *= scalar;
    matOut.m21 *= scalar; matOut.m22 *= scalar; matOut.m23 *= scalar;
    matOut.m31 *= scalar; matOut.m32 *= scalar; matOut.m33 *= scalar;
    return matOut;
}

Vector2 Matrix3x3::operator*(const Vector2& v, const Matrix3x3& mat) {
    Vector2 vOut;
    vOut.x = mat.m11 * v.x + mat.m12 * v.y + mat.m13;
    vOut.y = mat.m21 * v.x + mat.m22 * v.y + mat.m23;
    return v;
}

Vector3 Matrix3x3::operator*(const Vector3& v, const Matrix3x3& mat) {
    Vector3 vOut;
    vOut.x = mat.m11 * v.x + mat.m12 * v.y + mat.m13;
    vOut.y = mat.m21 * v.x + mat.m22 * v.y + mat.m23;
    vOut.z = mat.m31 * v.x + mat.m32 * v.y + mat.m33;
    return v;
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3& rhs) {
    Matrix3x3 mat;
    mat.m11 = m11 + rhs.m11;  mat.m12 = m12 + rhs.m12;  mat.m13 = m13 + rhs.m13;
    mat.m21 = m21 + rhs.m21;  mat.m22 = m22 + rhs.m22;  mat.m23 = m23 + rhs.m23;
    mat.m31 = m31 + rhs.m31;  mat.m32 = m32 + rhs.m32;  mat.m33 = m33 + rhs.m33;
    return mat;
}

Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& rhs) {
    m11 += rhs.m11;  m12 += rhs.m12;  m13 += rhs.m13;
    m21 += rhs.m21;  m22 += rhs.m22;  m23 += rhs.m23;
    m31 += rhs.m31;  m32 += rhs.m32;  m33 += rhs.m33;
    return *this;
}

Matrix3x3 Matrix3x3::operator-(const Matrix3x3& rhs) {
    Matrix3x3 mat;
    mat.m11 = m11 - rhs.m11;  mat.m12 = m12 - rhs.m12;  mat.m13 = m13 - rhs.m13;
    mat.m21 = m21 - rhs.m21;  mat.m22 = m22 - rhs.m22;  mat.m23 = m23 - rhs.m23;
    mat.m31 = m31 - rhs.m31;  mat.m32 = m32 - rhs.m32;  mat.m33 = m33 - rhs.m33;
    return mat;
}

Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& rhs) {
    m11 -= rhs.m11;  m12 -= rhs.m12;  m13 -= rhs.m13;
    m21 -= rhs.m21;  m22 -= rhs.m22;  m23 -= rhs.m23;
    m31 -= rhs.m31;  m32 -= rhs.m32;  m33 -= rhs.m33;
    return *this;
}