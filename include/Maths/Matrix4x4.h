#pragma once

#include <Maths/Math.h>
#include <Maths/Quaternion.h>
#include <Maths/float3.h>
#include <Maths/float4.h>

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale

namespace eloo::math {
class Matrix4x4 {
public:
    constexpr Matrix4x4()
        : r1({ 1.0f, 0.0f, 0.0f, 0.0f })
        , r2({ 0.0f, 1.0f, 0.0f, 0.0f })
        , r3({ 0.0f, 0.0f, 1.0f, 0.0f })
        , r4({ 0.0f, 0.0f, 0.0f, 1.0f }) {}
    constexpr Matrix4x4(const float4& r1,
                        const float4& r2,
                        const float4& r3,
                        const float4& r4)
        : r1(r1), r2(r2), r3(r3), r4(r4) {}
    constexpr Matrix4x4(float v11, float v12, float v13, float v14,
                        float v21, float v22, float v23, float v24,
                        float v31, float v32, float v33, float v34,
                        float v41, float v42, float v43, float v44)
        : data({ v11, v12, v13, v14,
                 v21, v22, v23, v24,
                 v31, v32, v33, v34,
                 v41, v42, v43, v44 }) {}

    float determinant() const;
    Matrix4x4 cofactor() const;
    Matrix4x4 adjugate() const;

    static Matrix4x4 transpose(const Matrix4x4& mat);
    static Matrix4x4 inverse(const Matrix4x4& mat);

    static const Matrix4x4& zero();
    static const Matrix4x4& one();
    static const Matrix4x4& identity();

    static Matrix4x4 createTranslation(const float3& translation);
    static Matrix4x4 createScale(const float3& scalar);
    static Matrix4x4 createRotation(float radians, const float3& axis);
    static Matrix4x4 createRotation(const float3& euler);
    static Matrix4x4 createRotation(const Quaternion& rotations);
    static Matrix4x4 createShearX(float shearing);
    static Matrix4x4 createShearY(float shearing);
    static Matrix4x4 createShearZ(float shearing);
    static Matrix4x4 createShear(float shearing, const float3& axis);

    inline static Matrix4x4& translate(const float3& translation, Matrix4x4& mat)       { return (mat *= createTranslation(translation)); }
    inline static Matrix4x4& scale(const float3& scalar, Matrix4x4& mat)                { return (mat *= createScale(scalar)); }
    inline static Matrix4x4& rotate(float radians, const float3& axis, Matrix4x4& mat)  { return (mat *= createRotation(radians, axis)); }
    inline static Matrix4x4& rotate(const float3& euler, Matrix4x4& mat)                { return (mat *= createRotation(euler)); }
    inline static Matrix4x4& rotate(const Quaternion& rotations, Matrix4x4& mat)        { return (mat *= createRotation(rotations)); }
    inline static Matrix4x4& shear(const float3& shearing, Matrix4x4& mat)              { return (mat *= createShear(shearing)); }

    static Matrix4x4 createView(const float3& viewOrigin, const float3& viewForward, const float3& worldUp);
    static Matrix4x4 createOrthographicProjection(const float3& boundMin, const float3& boundMax);
    static Matrix4x4 createPerspectiveProjection(float fov, float aspectRatio, float nearClip, float farClip);
    static Matrix4x4 createLookAt(const float3& from, const float3& to, const float3& upAxis);
    static Matrix4x4 createViewportTransformation();
    
    inline float4& getRow(uint8_t r)                                            { return row[r]; }
    inline const float4& getRow(uint8_t r) const                                { return row[r]; }
    inline void setRow(uint8_t r, const float4& val)                            { row[r] = val; }
    inline void setRow(uint8_t r, const float3& xyz, float w)                   { row[r] = { xyz.x, xyz.y, xyz.z, w }; }
    inline void setRow(uint8_t r, float x, const float3& yzw)                   { row[r] = { x, yzw.x, yzw.y, yzw.z }; }
    inline void setRow(uint8_t r, const float2& xy, const float2& zw)           { row[r] = { xy.x, xy.y, zw.x, zw.y }; }
    inline void setRow(uint8_t r, const float2& xy, float z, float w)           { row[r] = { xy.x, xy.y, z, w }; }
    inline void setRow(uint8_t r, float x, const float2& yz, float w)           { row[r] = { x, yz.x, yz.y, w }; }
    inline void setRow(uint8_t r, float x, float y, const float2& zw)           { row[r] = { x, y, zw.x, zw.y }; }
    inline void setRow(uint8_t r, float x, float y, float z, float w)           { row[r] = { x, y, z, w }; }

    inline const float4 getColumn(uint8_t c) const                              { return { cell[0][c], cell[1][c], cell[2][c], cell[3][c] }; }
    inline void setColumn(uint8_t c, const float4& val)                         { cell[0][c] = val.x;  cell[1][c] = val.y;  cell[2][c] = val.z;  cell[3][c] = val.w ; }
    inline void setColumn(uint8_t c, const float3& xyz, float w)                { cell[0][c] = xyz.x;  cell[1][c] = xyz.y;  cell[2][c] = xyz.z;  cell[3][c] = w; }
    inline void setColumn(uint8_t c, float x, const float3& yzw)                { cell[0][c] = x;      cell[1][c] = yzw.x;  cell[2][c] = yzw.y;  cell[3][c] = yzw.z; }
    inline void setColumn(uint8_t c, const float2& xy, const float2& zw)        { cell[0][c] = xy.x;   cell[1][c] = xy.y;   cell[2][c] = zw.x;   cell[3][c] = zw.y; }
    inline void setColumn(uint8_t c, const float2& xy, float z, float w)        { cell[0][c] = xy.x;   cell[1][c] = xy.y;   cell[2][c] = z;      cell[3][c] = w; }
    inline void setColumn(uint8_t c, float x, const float2& yz, float w)        { cell[0][c] = x;      cell[1][c] = yz.x;   cell[2][c] = yz.y;   cell[3][c] = w; }
    inline void setColumn(uint8_t c, float x, float y, const float2& zw)        { cell[0][c] = x;      cell[1][c] = y;      cell[2][c] = zw.x;   cell[3][c] = zw.y; }
    inline void setColumn(uint8_t c, float x, float y, float z, float w)        { cell[0][c] = x;      cell[1][c] = y;      cell[2][c] = z;      cell[3][c] = w; }

public:
    inline float4& operator [] (uint8_t rowIndex)                               { return row[rowIndex]; }
    inline const float4& operator [] (uint8_t rowIndex) const                   { return row[rowIndex]; }

    friend bool operator!=(const Matrix4x4& m1, const Matrix4x4& m2)            { return m1.cell != m2.cell; }
    inline friend bool operator == (const Matrix4x4& m1, const Matrix4x4& m2)   { return m1.cell == m2.cell; }

    friend float4 operator * (const float4& v, const Matrix4x4& m)              { return { m.r1 * v.x + m.r2 * v.y + m.r3 * v.z + m.r4 * v.w }; }
    friend Matrix4x4 operator * (const Matrix4x4& m1, const Matrix4x4& m2)      { return { m1.r1 * m2.r1, m1.r2 * m2.r2, m1.r3 * m2.r3, m1.r4 * m2.r4 }; }
    friend Matrix4x4 operator * (const Matrix4x4& m, float f)                   { return { m.r1 * f, m.r2 * f, m.r3 * f, m.r4 * f }; }
    inline Matrix4x4& operator *= (const Matrix4x4& m)                          { return (*this = *this * m); }
    inline Matrix4x4& operator *= (float f)                                     { return (*this = *this * f); }

    friend Matrix4x4 operator / (const Matrix4x4& m1, const Matrix4x4& m2)      { return { m1.r1 / m2.r1, m1.r2 / m2.r2, m1.r3 / m2.r3, m1.r4 / m2.r4 }; }
    friend Matrix4x4 operator / (const Matrix4x4& m, float f)                   { return m * (1.0f / f); }
    inline Matrix4x4& operator /= (const Matrix4x4& m)                          { return (*this = *this / m); }
    inline Matrix4x4& operator /= (float f)                                     { return (*this = *this / f); }

    friend Matrix4x4 operator + (const Matrix4x4& m1, const Matrix4x4& m2)      { return { m1.r1 + m2.r1, m1.r2 + m2.r2, m1.r3 + m2.r3, m1.r4 + m2.r4 }; }
    friend Matrix4x4 operator + (const Matrix4x4& m, float f)                   { return { m.r1 + f, m.r2 + f, m.r3 + f, m.r4 + f }; }
    inline Matrix4x4& operator += (const Matrix4x4& m)                          { return (*this = *this + m); }
    inline Matrix4x4& operator += (float f)                                     { return (*this = *this + f); }

    friend Matrix4x4 operator - (const Matrix4x4& m1, const Matrix4x4& m2)      { return { m1.r1 - m2.r1, m1.r2 - m2.r2, m1.r3 - m2.r3, m1.r4 - m2.r4 }; }
    friend Matrix4x4 operator - (const Matrix4x4& m, float f)                   { return { m.r1 - f, m.r2 - f, m.r3 - f, m.r4 - f }; }
    inline Matrix4x4& operator -= (const Matrix4x4& m)                          { return (*this = *this - m); }
    inline Matrix4x4& operator -= (float f)                                     { return (*this = *this - f); }

private:
    union {
        struct {
            float a11, a12, a13, a14;
            float a21, a22, a23, a24;
            float a31, a32, a33, a34;
            float a41, a42, a43, a44;
        };
        struct {
            float4 r1, r2, r3, r4;
        };
        eastl::array<float4> row;
        eastl::array<eastl::array<float, 4>, 4> cell;
        eastl::array<float, 16> data;
    };
};
} // namespace Math