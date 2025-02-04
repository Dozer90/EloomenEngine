#pragma once

#include <Maths/Math.h>
#include <Maths/MatrixRow.h>
#include <Maths/Vector2.h>
#include <Maths/Vector3.h>

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale

namespace eloo::Math {
class Matrix4x4;
class Matrix3x3 {
public:
    constexpr Matrix3x3() = default;
    constexpr Matrix3x3(float v) : mData({ v }) {}
    constexpr Matrix3x3(std::initializer_list<float> values) {
        mData.fill(0.0f);
        memcpy(mData.data(), values.begin(), sizeof(float) * min(values.size(), mData.size()));
    }
    Matrix3x3(const Matrix4x4& m, uint8_t row, uint8_t col);

    float determinant() const;
    Matrix3x3 cofactor() const;
    Matrix3x3 adjugate() const;

    static Matrix3x3 transpose(const Matrix3x3& mat);
    static Matrix3x3 inverse(const Matrix3x3& mat);

    static const Matrix3x3& zero();
    static const Matrix3x3& one();
    static const Matrix3x3& identity();

    static Matrix3x3 createTranslation(const Vector2& translation);
    static Matrix3x3 createScale(const Vector2& scalar);
    static Matrix3x3 createRotation(float radians);
    static Matrix3x3 createShear(const Vector2& shearing);

    inline static Matrix3x3& translate(const Vector2& translation, Matrix3x3& mat)  { return (mat *= createTranslation(translation)); }
    inline static Matrix3x3& scale(const Vector2& scalar, Matrix3x3& mat)           { return (mat *= createScale(scalar)); }
    inline static Matrix3x3& rotate(float radians, Matrix3x3& mat)                  { return (mat *= createRotation(radians)); }
    inline static Matrix3x3& shear(const Vector2& shearing, Matrix3x3& mat)         { return (mat *= createShear(shearing)); }

    inline float& get(uint8_t row, uint8_t col)             { return mData[row * 3 + col]; }
    inline const float& get(uint8_t row, uint8_t col) const { return mData[row * 3 + col]; }

public:

    //////////////////////////////////////////////
    // Accessors

    inline MatrixRow operator[](uint8_t rowIndex) {
        ASSERT(rowIndex < 3, "Index out of range: Trying to access non-existant row %i in Matrix3x3", rowIndex);
        return MatrixRow(&mData[rowIndex], 3);
    }
    inline ConstMatrixRow operator[](uint8_t rowIndex) const {
        ASSERT(rowIndex < 3, "Index out of range: Trying to access non-existant row %i in Matrix3x3", rowIndex);
        return ConstMatrixRow(&mData[rowIndex], 3);
    }


    //////////////////////////////////////////////
    // Equality

    friend bool operator!=(const Matrix3x3& m1, const Matrix3x3& m2) {
        return !isEqual(m1[0][0], m2[0][0]) && !isEqual(m1[0][1], m2[0][1]) && !isEqual(m1[0][2], m2[0][2])
            && !isEqual(m1[1][0], m2[1][0]) && !isEqual(m1[1][1], m2[1][1]) && !isEqual(m1[1][2], m2[1][2])
            && !isEqual(m1[2][0], m2[2][0]) && !isEqual(m1[2][1], m2[2][1]) && !isEqual(m1[2][2], m2[2][2]);
    }
    inline friend bool operator==(const Matrix3x3& m1, const Matrix3x3& m2) { return !(m1 != m2); }


    //////////////////////////////////////////////
    // Multiplication

    friend Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2) {
        return {
            m1[0][0] * m2[0][0], m1[0][1] * m2[0][1], m1[0][2] * m2[0][2],
            m1[1][0] * m2[1][0], m1[1][1] * m2[1][1], m1[1][2] * m2[1][2],
            m1[2][0] * m2[2][0], m1[2][1] * m2[2][1], m1[2][2] * m2[2][2] };
    }

    friend Matrix3x3 operator*(const Matrix3x3& m, float f) {
        return {
            m[0][0] * f, m[0][1] * f, m[0][2] * f,
            m[1][0] * f, m[1][1] * f, m[1][2] * f,
            m[2][0] * f, m[2][1] * f, m[2][2] * f };
    }


    //////////////////////////////////////////////
    // Division

    friend Matrix3x3 operator/(const Matrix3x3& m1, const Matrix3x3& m2) {
        return {
            m1[0][0] / m2[0][0], m1[0][1] / m2[0][1], m1[0][2] / m2[0][2],
            m1[1][0] / m2[1][0], m1[1][1] / m2[1][1], m1[1][2] / m2[1][2],
            m1[2][0] / m2[2][0], m1[2][1] / m2[2][1], m1[2][2] / m2[2][2] };
    }

    friend Matrix3x3 operator/(const Matrix3x3& m, float f) {
        if (isEqual(f, 0.0f)) { return zero(); }
        return m * (1.0f / f);
    }


    //////////////////////////////////////////////
    // Addition

    friend Matrix3x3 operator+(const Matrix3x3& m1, const Matrix3x3& m2) {
        return {
            m1[0][0] + m2[0][0], m1[0][1] + m2[0][1], m1[0][2] + m2[0][2],
            m1[1][0] + m2[1][0], m1[1][1] + m2[1][1], m1[1][2] + m2[1][2],
            m1[2][0] + m2[2][0], m1[2][1] + m2[2][1], m1[2][2] + m2[2][2] };
    }

    friend Matrix3x3 operator+(const Matrix3x3& m, float f) {
        return {
            m[0][0] + f, m[0][1] + f, m[0][2] + f,
            m[1][0] + f, m[1][1] + f, m[1][2] + f,
            m[2][0] + f, m[2][1] + f, m[2][2] + f };
    }


    //////////////////////////////////////////////
    // Subtraction

    friend Matrix3x3 operator-(const Matrix3x3& m1, const Matrix3x3& m2) {
        return {
            m1[0][0] - m2[0][0], m1[0][1] - m2[0][1], m1[0][2] - m2[0][2],
            m1[1][0] - m2[1][0], m1[1][1] - m2[1][1], m1[1][2] - m2[1][2],
            m1[2][0] - m2[2][0], m1[2][1] - m2[2][1], m1[2][2] - m2[2][2] };
    }

    friend Matrix3x3 operator-(const Matrix3x3& m, float f) {
        return {
            m[0][0] - f, m[0][1] - f, m[0][2] - f,
            m[1][0] - f, m[1][1] - f, m[1][2] - f,
            m[2][0] - f, m[2][1] - f, m[2][2] - f };
    }


    //////////////////////////////////////////////
    // Vector multiplication

    friend Vector2 operator*(const Vector2& v, const Matrix3x3& m) {
        return {
            m[0][0] * v.x + m[0][1] * v.y + m[0][2],
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] };
    }

    friend Vector3 operator*(const Vector3& v, const Matrix3x3& m) {
        return {
            m[0][0] * v.x + m[0][1] * v.y + m[0][2],
            m[1][0] * v.x + m[1][1] * v.y + m[1][2],
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] };
    }


    //////////////////////////////////////////////
    // Class method operators

    inline Matrix3x3& operator*=(const Matrix3x3& m) { return (*this = *this * m); }
    inline Matrix3x3& operator/=(const Matrix3x3& m) { return (*this = *this / m); }
    inline Matrix3x3& operator+=(const Matrix3x3& m) { return (*this = *this + m); }
    inline Matrix3x3& operator-=(const Matrix3x3& m) { return (*this = *this - m); }

    inline Matrix3x3& operator*=(float f)            { return (*this = *this * f); }
    inline Matrix3x3& operator/=(float f)            { return (*this = *this / f); }
    inline Matrix3x3& operator+=(float f)            { return (*this = *this + f); }
    inline Matrix3x3& operator-=(float f)            { return (*this = *this - f); }

private:
    eastl::array<float, 9> mData = { 1.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f };
};
} // namespace Math