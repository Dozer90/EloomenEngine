#pragma once

#include <Maths/Math.h>
#include <Maths/MatrixRow.h>
#include <Maths/Vector2.h>

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale

namespace eloo::Math {
class Matrix3x3;
class Matrix2x2 {
public:
    constexpr Matrix2x2() = default;
    constexpr Matrix2x2(float v) : mData({ v }) {}
    constexpr Matrix2x2(std::initializer_list<float> values) {
        mData.fill(0.0f);
        memcpy(mData.data(), values.begin(), sizeof(float) * min(values.size(), mData.size()));
    }
    Matrix2x2(const Matrix3x3& m, uint8_t row, uint8_t col);

    inline float determinant() const { return get(0,0) * get(1,1) - get(0,1) * get(1,0); }

    inline float& get(uint8_t row, uint8_t col) { return mData[row * 2 + col]; }
    inline const float& get(uint8_t row, uint8_t col) const { return mData[row * 2 + col]; }

public:

    //////////////////////////////////////////////
    // Accessors

    inline MatrixRow operator[](uint8_t rowIndex) {
        ASSERT(rowIndex < 2, "Index out of range: Trying to access non-existant row %i in Matrix2x2", rowIndex);
        return MatrixRow(&mData[rowIndex], 2);
    }
    inline ConstMatrixRow operator[](uint8_t rowIndex) const {
        ASSERT(rowIndex < 2, "Index out of range: Trying to access non-existant row %i in Matrix2x2", rowIndex);
        return ConstMatrixRow(&mData[rowIndex], 2);
    }


    //////////////////////////////////////////////
    // Equality

    friend bool operator!=(const Matrix2x2& m1, const Matrix2x2& m2) {
        return !isEqual(m1[0][0], m2[0][0]) && !isEqual(m1[0][1], m2[0][1])
            && !isEqual(m1[1][0], m2[1][0]) && !isEqual(m1[1][1], m2[1][1]);
    }
    inline friend bool operator==(const Matrix2x2& m1, const Matrix2x2& m2) { return !(m1 != m2); }


    //////////////////////////////////////////////
    // Multiplication

    friend Matrix2x2 operator*(const Matrix2x2& m1, const Matrix2x2& m2) {
        return {
            m1[0][0] * m2[0][0], m1[0][1] * m2[0][1],
            m1[1][0] * m2[1][0], m1[1][1] * m2[1][1] };
    }

    friend Matrix2x2 operator*(const Matrix2x2& m, float f) {
        return {
            m[0][0] * f, m[0][1] * f,
            m[1][0] * f, m[1][1] * f };
    }


    //////////////////////////////////////////////
    // Division

    friend Matrix2x2 operator/(const Matrix2x2& m1, const Matrix2x2& m2) {
        return {
            m1[0][0] / m2[0][0], m1[0][1] / m2[0][1],
            m1[1][0] / m2[1][0], m1[1][1] / m2[1][1] };
    }

    friend Matrix2x2 operator/(const Matrix2x2& m, float f) {
        if (isEqual(f, 0.0f)) { return zero(); }
        return m * (1.0f / f);
    }


    //////////////////////////////////////////////
    // Addition

    friend Matrix2x2 operator+(const Matrix2x2& m1, const Matrix2x2& m2) {
        return {
            m1[0][0] + m2[0][0], m1[0][1] + m2[0][1],
            m1[1][0] + m2[1][0], m1[1][1] + m2[1][1] };
    }

    friend Matrix2x2 operator+(const Matrix2x2& m, float f) {
        return {
            m[0][0] + f, m[0][1] + f, m[0][2] + f,
            m[1][0] + f, m[1][1] + f, m[1][2] + f,
            m[2][0] + f, m[2][1] + f, m[2][2] + f };
    }


    //////////////////////////////////////////////
    // Subtraction

    friend Matrix2x2 operator-(const Matrix2x2& m1, const Matrix2x2& m2) {
        return {
            m1[0][0] - m2[0][0], m1[0][1] - m2[0][1],
            m1[1][0] - m2[1][0], m1[1][1] - m2[1][1] };
    }

    friend Matrix2x2 operator-(const Matrix2x2& m, float f) {
        return {
            m[0][0] - f, m[0][1] - f,
            m[1][0] - f, m[1][1] - f };
    }


    //////////////////////////////////////////////
    // Vector multiplication

    friend Vector2 operator*(const Vector2& v, const Matrix2x2& m) {
        return {
            m[0][0] * v.x + m[0][1] * v.y,
            m[1][0] * v.x + m[1][1] * v.y };
    }


    //////////////////////////////////////////////
    // Class method operators

    inline Matrix2x2& operator*=(const Matrix2x2& m) { return (*this = *this * m); }
    inline Matrix2x2& operator/=(const Matrix2x2& m) { return (*this = *this / m); }
    inline Matrix2x2& operator+=(const Matrix2x2& m) { return (*this = *this + m); }
    inline Matrix2x2& operator-=(const Matrix2x2& m) { return (*this = *this - m); }

    inline Matrix2x2& operator*=(float f) { return (*this = *this * f); }
    inline Matrix2x2& operator/=(float f) { return (*this = *this / f); }
    inline Matrix2x2& operator+=(float f) { return (*this = *this + f); }
    inline Matrix2x2& operator-=(float f) { return (*this = *this - f); }

private:
    eastl::array<float, 4> mData = { 1.0f, 0.0f,
                                     0.0f, 1.0f };
};
} // namespace Math