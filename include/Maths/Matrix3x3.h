#pragma once

#include <Maths/Math.h>
#include <Maths/float2.h>
#include <Maths/float3.h>

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale

namespace eloo::Math {
class Matrix4x4;
class Matrix3x3 {
public:
    constexpr Matrix3x3()
        : r1({ 1.0f, 0.0f, 0.0f })
        , r2({ 0.0f, 1.0f, 0.0f })
        , r3({ 0.0f, 0.0f, 1.0f }) {}
    constexpr Matrix3x3(const float3& r1,
                        const float3& r2,
                        const float3& r3)
        : r1(r1), r2(r2), r3(r3) {}
    constexpr Matrix3x3(float v11, float v12, float v13,
                        float v21, float v22, float v23,
                        float v31, float v32, float v33)
        : data({v11, v12, v13,
                v21, v22, v23,
                v31, v32, v33}) {}
    Matrix3x3(const Matrix4x4& m, uint8_t row, uint8_t col);

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

    inline float3& getRow(uint8_t r)                                                { return row[r]; }
    inline const float3& getRow(uint8_t r) const                                    { return row[r]; }
    inline void setRow(uint8_t r, const float3& xyz)                                { row[r] = xyz; }
    inline void setRow(uint8_t r, const float2& xy, float z)                        { row[r] = { xy.x, xy.y, z }; }
    inline void setRow(uint8_t r, float x, const float2& yz)                        { row[r] = { x, yz.x, yz.y }; }
    inline void setRow(uint8_t r, float x, float y, float z)                        { row[r] = { x, y, z }; }

    inline const float3 getColumn(uint8_t c) const                                  { return { cell[0][c], cell[1][c], cell[2][c] }; }
    inline void setColumn(uint8_t c, const float3& xyz)                             { cell[0][c] = xyz.x;  cell[1][c] = xyz.y;  cell[2][c] = xyz.z; }
    inline void setColumn(uint8_t c, const float2& xy, float z)                     { cell[0][c] = xy.x;   cell[1][c] = xy.y;   cell[2][c] = z;     }
    inline void setColumn(uint8_t c, float x, const float2& yz)                     { cell[0][c] = x;      cell[1][c] = yz.x;   cell[2][c] = yz.y;  }
    inline void setColumn(uint8_t c, float x, float y, float z)                     { cell[0][c] = x;      cell[1][c] = y;      cell[2][c] = z;     }

public:
    inline float3& operator [] (uint8_t rowIndex)                                   { return row[rowIndex]; }
    inline const float3& operator [] (uint8_t rowIndex) const                       { return row[rowIndex]; }

    friend bool operator!=(const Matrix3x3& m1, const Matrix3x3& m2)                { return m1.cell != m2.cell; }
    inline friend bool operator == (const Matrix3x3& m1, const Matrix3x3& m2)       { return m1.cell == m2.cell; }

    friend float3 operator * (const float3& v, const Matrix3x3& m)                  { return { m.r1 * v.x + m.r2 * v.y + m.r3 * v.z }; }
    friend Matrix3x3 operator * (const Matrix3x3& m1, const Matrix3x3& m2)          { return { m1.r1 * m2.r1, m1.r2 * m2.r2, m1.r3 * m2.r3 }; }
    friend Matrix3x3 operator * (const Matrix3x3& m, float f)                       { return { m.r1 * f, m.r2 * f, m.r3 * f }; }
    inline Matrix3x3& operator *= (const Matrix3x3& m)                              { return (*this = *this * m); }
    inline Matrix3x3& operator *= (float f)                                         { return (*this = *this * f); }

    friend Matrix3x3 operator / (const Matrix3x3& m1, const Matrix3x3& m2)          { return { m1.r1 / m2.r1, m1.r2 / m2.r2, m1.r3 / m2.r3 }; }
    friend Matrix3x3 operator / (const Matrix3x3& m, float f)                       { return m * (1.0f / f); }
    inline Matrix3x3& operator /= (const Matrix3x3& m)                              { return (*this = *this / m); }
    inline Matrix3x3& operator /= (float f)                                         { return (*this = *this / f); }

    friend Matrix3x3 operator + (const Matrix3x3& m1, const Matrix3x3& m2)          { return { m1.r1 + m2.r1, m1.r2 + m2.r2, m1.r3 + m2.r3 }; }
    friend Matrix3x3 operator + (const Matrix3x3& m, float f)                       { return { m.r1 + f, m.r2 + f, m.r3 + f}; }
    inline Matrix3x3& operator += (const Matrix3x3& m)                              { return (*this = *this + m); }
    inline Matrix3x3& operator += (float f)                                         { return (*this = *this + f); }

    friend Matrix3x3 operator - (const Matrix3x3& m1, const Matrix3x3& m2)          { return { m1.r1 - m2.r1, m1.r2 - m2.r2, m1.r3 - m2.r3 }; }
    friend Matrix3x3 operator - (const Matrix3x3& m, float f)                       { return { m.r1 - f, m.r2 - f, m.r3 - f }; }
    inline Matrix3x3& operator -= (const Matrix3x3& m)                              { return (*this = *this - m); }
    inline Matrix3x3& operator -= (float f)                                         { return (*this = *this - f); }

private:
    union {
        struct {
            float a11, a12, a13;
            float a21, a22, a23;
            float a31, a32, a33;
        };
        struct {
            float3 r1, r2, r3;
        };
        eastl::array<float3> row;
        eastl::array<eastl::array<float, 3>, 3> cell;
        eastl::array<float, 9> data;
    };
};
} // namespace Math