#pragma once

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale

namespace eloo {

class float2;
class float3;
class float4;

namespace Math {

class Quaternion;
class Matrix2x2;
class Matrix4x4;

class Matrix3x3 {
public:
    struct Row {
        float x, y, z;

        float& operator [] (uint8_t colIndex);
        const float& operator [] (uint8_t colIndex) const;
    };

public:
    inline Matrix3x3(const Matrix3x3&) = default;
    Matrix3x3(Matrix3x3&&) = default;

    constexpr Matrix3x3()
        : data({1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f}) {}

    constexpr Matrix3x3(float v11, float v12, float v13,
                        float v21, float v22, float v23,
                        float v31, float v32, float v33)
        : data({v11, v12, v13,
                v21, v22, v23,
                v31, v32, v33}) {}

    Matrix3x3(const float3& r1, const float3& r2, const float3& r3);
    Matrix3x3(const Matrix4x4& m, uint8_t rowToRemove, uint8_t colToRemove);

public:
    static const Matrix3x3& zero();
    static const Matrix3x3& one();
    static const Matrix3x3& identity();

    float determinant() const;
    Matrix3x3 cofactor() const;
    Matrix3x3 adjugate() const;

    static Matrix3x3 transpose(const Matrix3x3& mat);
    static Matrix3x3 inverse(const Matrix3x3& mat);

    static Matrix3x3 createTranslation(const float2& translation);
    static Matrix3x3 createScale(const float2& scalar);
    static Matrix3x3 createRotation(float radians);
    static Matrix3x3 createRotation(float radians, const float3& axis);
    static Matrix3x3 createRotation(const float3& euler);
    static Matrix3x3 createRotation(const Quaternion& rotation);
    static Matrix3x3 createShear(const float2& shearing);

    inline static Matrix3x3& translate(const float2& translation, Matrix3x3& mat)   { return (mat *= createTranslation(translation)); }
    inline static Matrix3x3& scale(const float2& scalar, Matrix3x3& mat)            { return (mat *= createScale(scalar)); }
    inline static Matrix3x3& rotate(float radians, Matrix3x3& mat)                  { return (mat *= createRotation(radians)); }
    inline static Matrix3x3& shear(const float2& shearing, Matrix3x3& mat)          { return (mat *= createShear(shearing)); }

    void setRow(uint8_t rowIndex, const float3& xyz);
    void setRow(uint8_t rowIndex, const float2& xy, float z);
    void setRow(uint8_t rowIndex, float x, const float2& yz);
    void setRow(uint8_t rowIndex, float x, float y, float z);

    inline const Row column(uint8_t colIndex)                                       { return Row{ cell[0][colIndex], cell[1][colIndex], cell[2][colIndex] }; }
    void setColumn(uint8_t colIndex, const float3& xyz);
    void setColumn(uint8_t colIndex, const float2& xy, float z);
    void setColumn(uint8_t colIndex, float x, const float2& yz);
    void setColumn(uint8_t colIndex, float x, float y, float z);

public:
    inline Row& operator [] (uint8_t rowIndex)                                      { return row[rowIndex]; }
    inline const Row& operator [] (uint8_t rowIndex) const                          { return row[rowIndex]; }

    friend bool operator!=(const Matrix3x3& m1, const Matrix3x3& m2)                { return !eastl::equal(m1.cell.begin(), m1.cell.end(), m2.cell.begin()); }
    inline friend bool operator == (const Matrix3x3& m1, const Matrix3x3& m2)       { return eastl::equal(m1.cell.begin(), m1.cell.end(), m2.cell.begin()); }
    
    friend Matrix3x3 operator / (const Matrix3x3& m1, const Matrix3x3& m2) {
        return {
            m1.a11 / m2.a11, m1.a12 / m2.a12, m1.a13 / m2.a13,
            m1.a21 / m2.a21, m1.a22 / m2.a22, m1.a23 / m2.a23,
            m1.a31 / m2.a31, m1.a32 / m2.a32, m1.a33 / m2.a33 };
    }
    friend Matrix3x3 operator / (const Matrix3x3& m, float f) {
        return {
            m.a11 / f, m.a12 / f, m.a13 / f,
            m.a21 / f, m.a22 / f, m.a23 / f,
            m.a31 / f, m.a32 / f, m.a33 / f };
    }

    friend Matrix3x3 operator * (const Matrix3x3& m1, const Matrix3x3& m2) {
        return {
            m1.a11 * m2.a11, m1.a12 * m2.a12, m1.a13 * m2.a13,
            m1.a21 * m2.a21, m1.a22 * m2.a22, m1.a23 * m2.a23,
            m1.a31 * m2.a31, m1.a32 * m2.a32, m1.a33 * m2.a33 };
    }
    friend Matrix3x3 operator * (const Matrix3x3& m, float f) {
        return {
            m.a11 * f, m.a12 * f, m.a13 * f,
            m.a21 * f, m.a22 * f, m.a23 * f,
            m.a31 * f, m.a32 * f, m.a33 * f };
    }

    friend Matrix3x3 operator + (const Matrix3x3& m1, const Matrix3x3& m2) {
        return {
            m1.a11 + m2.a11, m1.a12 + m2.a12, m1.a13 + m2.a13,
            m1.a21 + m2.a21, m1.a22 + m2.a22, m1.a23 + m2.a23,
            m1.a31 + m2.a31, m1.a32 + m2.a32, m1.a33 + m2.a33 };
    }
    friend Matrix3x3 operator + (const Matrix3x3& m, float f) {
        return {
            m.a11 + f, m.a12 + f, m.a13 + f,
            m.a21 + f, m.a22 + f, m.a23 + f,
            m.a31 + f, m.a32 + f, m.a33 + f };
    }

    friend Matrix3x3 operator - (const Matrix3x3& m1, const Matrix3x3& m2) {
        return {
            m1.a11 - m2.a11, m1.a12 - m2.a12, m1.a13 - m2.a13,
            m1.a21 - m2.a21, m1.a22 - m2.a22, m1.a23 - m2.a23,
            m1.a31 - m2.a31, m1.a32 - m2.a32, m1.a33 - m2.a33 };
    }
    friend Matrix3x3 operator - (const Matrix3x3& m, float f) {
        return {
            m.a11 - f, m.a12 - f, m.a13 - f,
            m.a21 - f, m.a22 - f, m.a23 - f,
            m.a31 - f, m.a32 - f, m.a33 - f };
    }

    inline Matrix3x3& operator /= (const Matrix3x3& m) { return (*this = *this / m); }
    inline Matrix3x3& operator *= (const Matrix3x3& m) { return (*this = *this * m); }
    inline Matrix3x3& operator += (const Matrix3x3& m) { return (*this = *this + m); }
    inline Matrix3x3& operator -= (const Matrix3x3& m) { return (*this = *this - m); }
    inline Matrix3x3& operator /= (float f)            { return (*this = *this / f); }
    inline Matrix3x3& operator *= (float f)            { return (*this = *this * f); }
    inline Matrix3x3& operator += (float f)            { return (*this = *this + f); }
    inline Matrix3x3& operator -= (float f)            { return (*this = *this - f); }

private:
    union {
        struct {
            float a11, a12, a13;
            float a21, a22, a23;
            float a31, a32, a33;
        };
        eastl::array<Row, 3> row;
        eastl::array<eastl::array<float, 3>, 3> cell;
        eastl::array<float, 9> data;
    };
};
} // namespace Math
} // namespace eloo