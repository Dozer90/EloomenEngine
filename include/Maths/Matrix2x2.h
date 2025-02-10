#pragma once

#include <Maths/Math.h>
#include <Maths/float2.h>

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale

namespace eloo::Math {
class Matrix3x3;
class Matrix2x2 {
public:
    constexpr Matrix2x2() : r1({ 1.0f, 0.0f }), r2({ 0.0f, 1.0f }) {}
    constexpr Matrix2x2(const float2& r1, const float2& r2) : r1(r1), r2(r2) {}
    constexpr Matrix2x2(float v11, float v12, float v21, float v22) : data({ v11, v12, v21, v22 }) {}
    Matrix2x2(const Matrix3x3& m, uint8_t row, uint8_t col);

    static const Matrix2x2& zero();
    static const Matrix2x2& one();
    static const Matrix2x2& identity();

    inline float determinant() const                                            { return a11 * a22 - a12 * a21; }

    inline float2& getRow(uint8_t r)                                            { return row[r]; }
    inline const float2& getRow(uint8_t r) const                                { return row[r]; }
    inline void setRow(uint8_t r, const float2& xy)                             { row[r] = xy; }
    inline void setRow(uint8_t r, float x, float y)                             { row[r] = { x, y }; }

    inline const float2 getColumn(uint8_t c) const                              { return { cell[0][c], cell[1][c] }; }
    inline void setColumn(uint8_t c, const float2& xy)                          { cell[0][c] = xy.x; cell[1][c] = xy.y; }
    inline void setColumn(uint8_t c, float x, float y)                          { cell[0][c] = x;    cell[1][c] = y; }

public:
    inline float2& operator [] (uint8_t r)                                      { return row[r]; }
    inline const float2& operator [] (uint8_t r) const                          { return row[r]; }

    friend bool operator!=(const Matrix2x2& m1, const Matrix2x2& m2)            { return m1.cell != m2.cell; }
    inline friend bool operator == (const Matrix2x2& m1, const Matrix2x2& m2)   { return m1.cell == m2.cell; }

    friend float2 operator * (const float2& v, const Matrix2x2& m)              { return { m.r1 * v.x + m.r2 * v.y }; }
    friend Matrix2x2 operator * (const Matrix2x2& m1, const Matrix2x2& m2)      { return { m1.r1 * m2.r1, m1.r2 * m2.r2 }; }
    friend Matrix2x2 operator * (const Matrix2x2& m, float f)                   { return { m.r1 * f, m.r2 * f }; }
    inline Matrix2x2& operator *= (const Matrix2x2& m)                          { return (*this = *this * m); }
    inline Matrix2x2& operator *= (float f)                                     { return (*this = *this * f); }

    friend Matrix2x2 operator / (const Matrix2x2& m1, const Matrix2x2& m2)      { return { m1.r1 / m2.r1, m1.r2 / m2.r2 }; }
    friend Matrix2x2 operator / (const Matrix2x2& m, float f)                   { return m * (1.0f / f); }
    inline Matrix2x2& operator /= (const Matrix2x2& m)                          { return (*this = *this / m); }
    inline Matrix2x2& operator /= (float f)                                     { return (*this = *this / f); }

    friend Matrix2x2 operator + (const Matrix2x2& m1, const Matrix2x2& m2)      { return { m1.r1 + m2.r1, m1.r2 + m2.r2 }; }
    friend Matrix2x2 operator + (const Matrix2x2& m, float f)                   { return { m.r1 + f, m.r2 + f }; }
    inline Matrix2x2& operator += (const Matrix2x2& m)                          { return (*this = *this + m); }
    inline Matrix2x2& operator += (float f)                                     { return (*this = *this + f); }

    friend Matrix2x2 operator - (const Matrix2x2& m1, const Matrix2x2& m2)      { return { m1.r1 - m2.r1, m1.r2 - m2.r2 }; }
    friend Matrix2x2 operator - (const Matrix2x2& m, float f)                   { return { m.r1 - f, m.r2 - f }; }
    inline Matrix2x2& operator -= (const Matrix2x2& m)                          { return (*this = *this - m); }
    inline Matrix2x2& operator -= (float f)                                     { return (*this = *this - f); }

private:
    union {
        struct {
            float a11, a12;
            float a21, a22;
        };
        struct {
            float2 r1, r2;
        };
        eastl::array<float2, 2> row;
        eastl::array<eastl::array<float, 2>, 2> cell;
        eastl::array<float, 4> data;
    };
};
} // namespace eloo::Math