#pragma once

#include <Maths/Math.h>
#include <Maths/MatrixRow.h>
#include <Maths/Vector3.h>
#include <Maths/Vector4.h>

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale

namespace eloo::Math {
class Quaternion;

class Matrix4x4 {
public:
    constexpr Matrix4x4() = default;
    constexpr Matrix4x4(float v) : mData({ v }) {}
    constexpr Matrix4x4(std::initializer_list<float> values) {
        mData.fill(0.0f);
        memcpy(mData.data(), values.begin(), sizeof(float) * min(values.size(), mData.size()));
    }

    float determinant() const;
    Matrix4x4 cofactor() const;
    Matrix4x4 adjugate() const;

    static Matrix4x4 transpose(const Matrix4x4& mat);
    static Matrix4x4 inverse(const Matrix4x4& mat);

    static const Matrix4x4& zero();
    static const Matrix4x4& one();
    static const Matrix4x4& identity();

    static Matrix4x4 createTranslation(const Vector3& translation);
    static Matrix4x4 createScale(const Vector3& scalar);
    static Matrix4x4 createRotation(float radians, const Vector3& axis);
    static Matrix4x4 createRotation(const Vector3& euler);
    static Matrix4x4 createRotation(const Quaternion& rotations);

    inline static Matrix4x4& translate(const Vector3& translation, Matrix4x4& mat)      { return (mat *= createTranslation(translation)); }
    inline static Matrix4x4& scale(const Vector3& scalar, Matrix4x4& mat)               { return (mat *= createScale(scalar)); }
    inline static Matrix4x4& rotate(float radians, const Vector3& axis, Matrix4x4& mat) { return (mat *= createRotation(radians, axis)); }
    inline static Matrix4x4& rotate(const Vector3& euler, Matrix4x4& mat)               { return (mat *= createRotation(euler)); }

    static Matrix4x4 createView(const Vector3& viewOrigin, const Vector3& viewForward, const Vector3& worldUp);
    static Matrix4x4 createOrthographicProjection(const Vector3& boundMin, const Vector3& boundMax);
    static Matrix4x4 createPerspectiveProjection(float fov, float aspectRatio, float nearClip, float farClip);
    static Matrix4x4 createLookAt(const Vector3& from, const Vector3& to, const Vector3& upAxis);
    static Matrix4x4 createViewportTransformation();

	inline MatrixRow getRow(uint8_t row) { return (*this)[row]; }
	inline ConstMatrixRow getRow(uint8_t row) const { return (*this)[row]; }
	inline MatrixRow getColumn(uint8_t col) {
		ASSERT(col < 4, "Index out of range: Trying to access non-existant column %i in Matrix4x4", col);
		return MatrixRow(&mData[col], 4);
	}
	inline ConstMatrixRow getColumn(uint8_t col) const {
		ASSERT(col < 4, "Index out of range: Trying to access non-existant column %i in Matrix4x4", col);
		return ConstMatrixRow(&mData[col], 4);
	}

    inline float& get(uint8_t row, uint8_t col)             { return mData[row * 4 + col]; }
    inline const float& get(uint8_t row, uint8_t col) const { return mData[row * 4 + col]; }

public:

    //////////////////////////////////////////////
    // Accessors

    inline MatrixRow operator[](uint8_t rowIndex) {
        ASSERT(rowIndex < 4, "Index out of range: Trying to access non-existant row %i in Matrix4x4", rowIndex);
        return MatrixRow(&mData[rowIndex], 4);
    }
    inline ConstMatrixRow operator[](uint8_t rowIndex) const {
        ASSERT(rowIndex < 4, "Index out of range: Trying to access non-existant row %i in Matrix4x4", rowIndex);
        return ConstMatrixRow(&mData[rowIndex], 4);
    }


    //////////////////////////////////////////////
    // Equality

    friend bool operator!=(const Matrix4x4& m1, const Matrix4x4& m2) {
        return !isEqual(m1[0][0], m2[0][0]) && !isEqual(m1[0][1], m2[0][1]) && !isEqual(m1[0][2], m2[0][2]) && !isEqual(m1[0][3], m2[0][3])
            && !isEqual(m1[1][0], m2[1][0]) && !isEqual(m1[1][1], m2[1][1]) && !isEqual(m1[1][2], m2[1][2]) && !isEqual(m1[1][3], m2[1][3])
            && !isEqual(m1[2][0], m2[2][0]) && !isEqual(m1[2][1], m2[2][1]) && !isEqual(m1[2][2], m2[2][2]) && !isEqual(m1[2][3], m2[2][3])
            && !isEqual(m1[3][0], m2[3][0]) && !isEqual(m1[3][1], m2[3][1]) && !isEqual(m1[3][2], m2[3][2]) && !isEqual(m1[3][3], m2[3][3]);
    }
    inline friend bool operator==(const Matrix4x4& m1, const Matrix4x4& m2) { return !(m1 != m2); }


    //////////////////////////////////////////////
    // Multiplication

    friend Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
        return {
            m1[0][0] * m2[0][0], m1[0][1] * m2[0][1], m1[0][2] * m2[0][2], m1[0][3] * m2[0][3],
            m1[1][0] * m2[1][0], m1[1][1] * m2[1][1], m1[1][2] * m2[1][2], m1[1][3] * m2[1][3],
            m1[2][0] * m2[2][0], m1[2][1] * m2[2][1], m1[2][2] * m2[2][2], m1[2][3] * m2[2][3],
            m1[3][0] * m2[3][0], m1[3][1] * m2[3][1], m1[3][2] * m2[3][2], m1[3][3] * m2[3][3] };
    }

    friend Matrix4x4 operator*(const Matrix4x4& m, float f) {
        return {
            m[0][0] * f, m[0][1] * f, m[0][2] * f, m[0][3] * f,
            m[1][0] * f, m[1][1] * f, m[1][2] * f, m[1][3] * f,
            m[2][0] * f, m[2][1] * f, m[2][2] * f, m[2][3] * f,
            m[3][0] * f, m[3][1] * f, m[3][2] * f, m[3][3] * f };
    }


    //////////////////////////////////////////////
    // Division

    friend Matrix4x4 operator/(const Matrix4x4& m1, const Matrix4x4& m2) {
        return {
            m1[0][0] / m2[0][0], m1[0][1] / m2[0][1], m1[0][2] / m2[0][2], m1[0][3] / m2[0][3],
            m1[1][0] / m2[1][0], m1[1][1] / m2[1][1], m1[1][2] / m2[1][2], m1[1][3] / m2[1][3],
            m1[2][0] / m2[2][0], m1[2][1] / m2[2][1], m1[2][2] / m2[2][2], m1[2][3] / m2[2][3],
            m1[3][0] / m2[3][0], m1[3][1] / m2[3][1], m1[3][2] / m2[3][2], m1[3][3] / m2[3][3] };
    }

    friend Matrix4x4 operator/(const Matrix4x4& m, float f) {
        if (isEqual(f, 0.0f)) { return zero(); }
        return m * (1.0f / f);
    }


    //////////////////////////////////////////////
    // Addition

    friend Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
        return {
            m1[0][0] + m2[0][0], m1[0][1] + m2[0][1], m1[0][2] + m2[0][2], m1[0][3] + m2[0][3],
            m1[1][0] + m2[1][0], m1[1][1] + m2[1][1], m1[1][2] + m2[1][2], m1[1][3] + m2[1][3],
            m1[2][0] + m2[2][0], m1[2][1] + m2[2][1], m1[2][2] + m2[2][2], m1[2][3] + m2[2][3],
            m1[3][0] + m2[3][0], m1[3][1] + m2[3][1], m1[3][2] + m2[3][2], m1[3][3] + m2[3][3] };
    }

    friend Matrix4x4 operator+(const Matrix4x4& m, float f) {
        return {
            m[0][0] + f, m[0][1] + f, m[0][2] + f, m[0][3] + f,
            m[1][0] + f, m[1][1] + f, m[1][2] + f, m[1][3] + f,
            m[2][0] + f, m[2][1] + f, m[2][2] + f, m[2][3] + f,
            m[3][0] + f, m[3][1] + f, m[3][2] + f, m[3][3] + f };
    }


    //////////////////////////////////////////////
    // Subtraction

    friend Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
        return {
            m1[0][0] - m2[0][0], m1[0][1] - m2[0][1], m1[0][2] - m2[0][2], m1[0][3] - m2[0][3],
            m1[1][0] - m2[1][0], m1[1][1] - m2[1][1], m1[1][2] - m2[1][2], m1[1][3] - m2[1][3],
            m1[2][0] - m2[2][0], m1[2][1] - m2[2][1], m1[2][2] - m2[2][2], m1[2][3] - m2[2][3],
            m1[3][0] - m2[3][0], m1[3][1] - m2[3][1], m1[3][2] - m2[3][2], m1[3][3] - m2[3][3] };
    }

    friend Matrix4x4 operator-(const Matrix4x4& m, float f) {
        return {
            m[0][0] - f, m[0][1] - f, m[0][2] - f, m[0][3] - f,
            m[1][0] - f, m[1][1] - f, m[1][2] - f, m[1][3] - f,
            m[2][0] - f, m[2][1] - f, m[2][2] - f, m[2][3] - f,
            m[3][0] - f, m[3][1] - f, m[3][2] - f, m[3][3] - f };
    }

    //////////////////////////////////////////////
    // Vector multiplication

    friend Vector3 operator*(const Vector3& v, const Matrix4x4& m) {
        return {
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] };
    }

    friend Vector4 operator*(const Vector4& v, const Matrix4x4& m) {
        return {
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
            m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w };
    }


    //////////////////////////////////////////////
    // Class method operators

    inline Matrix4x4& operator*=(const Matrix4x4& m) { return (*this = *this * m); }
    inline Matrix4x4& operator/=(const Matrix4x4& m) { return (*this = *this / m); }
    inline Matrix4x4& operator+=(const Matrix4x4& m) { return (*this = *this + m); }
    inline Matrix4x4& operator-=(const Matrix4x4& m) { return (*this = *this - m); }

    inline Matrix4x4& operator*=(float f) { return (*this = *this * f); }
    inline Matrix4x4& operator/=(float f) { return (*this = *this / f); }
    inline Matrix4x4& operator+=(float f) { return (*this = *this + f); }
    inline Matrix4x4& operator-=(float f) { return (*this = *this - f); }

private:
    eastl::array<float, 16> mData = { 1.0f, 0.0f, 0.0f, 0.0f, 
                                      0.0f, 1.0f, 0.0f, 0.0f, 
                                      0.0f, 0.0f, 1.0f, 0.0f,
                                      0.0f, 0.0f, 0.0f, 1.0f };
};
} // namespace Math