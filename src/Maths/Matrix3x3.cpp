#include <Maths/CoreMath.h>

#include <Generic/Utilities.h>

using namespace eloo::Math;

namespace {
static constexpr Matrix3x3 gZero({ 0.0f }, { 0.0f }, { 0.0f });
static constexpr Matrix3x3 gOne({ 1.0f }, { 1.0f }, { 1.0f });
static constexpr Matrix3x3 gIdentity;
}

Matrix3x3::Matrix3x3(const Matrix4x4& m, uint8_t row, uint8_t col) {
    uint8_t i = 0;
    for (uint8_t r = 0; r < 4; ++r) {
        if (r == row) continue;
        for (uint8_t c = 0; c < 4; ++c) {
            if (c == col) continue;
            data[i++] = m[r][c];
        }
    }
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

Matrix3x3 Matrix3x3::transpose(const Matrix3x3& m) {
    return { m.getColumn(0), m.getColumn(1), m.getColumn(2) };
}

float Matrix3x3::determinant() const {
    return a11 * (a22 * a33 - a23 * a32) -
           a12 * (a21 * a33 - a23 * a31) +
           a13 * (a21 * a32 - a22 * a31);
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

Matrix3x3 Matrix3x3::createTranslation(const float2& translation) {
    Matrix3x3 mat;
    mat[0][2] = translation.x;
    mat[1][2] = translation.y;
    return mat;
}

Matrix3x3 Matrix3x3::createScale(const float2& scales) {
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

Matrix3x3 Matrix3x3::createShear(const float2& shearing) {
    Matrix3x3 mat;
    mat[0][1] = shearing.x;
    mat[1][0] = shearing.y;
    return mat;
}


float3 Matrix3x3::getRow(uint8_t r) { return { cell[r][0], cell[r][1], cell[r][2] }; }

const float3& getRow(uint8_t r) const                                    { return row[r]; }
void setRow(uint8_t r, const float3& xyz)                                { row[r] = xyz; }
void setRow(uint8_t r, const float2& xy, float z)                        { row[r] = { xy.x, xy.y, z }; }
void setRow(uint8_t r, float x, const float2& yz)                        { row[r] = { x, yz.x, yz.y }; }
void setRow(uint8_t r, float x, float y, float z)                        { row[r] = { x, y, z }; }

const float3 getColumn(uint8_t c) const                                  { return { cell[0][c], cell[1][c], cell[2][c] }; }
void setColumn(uint8_t c, const float3& xyz)                             { cell[0][c] = xyz.x;  cell[1][c] = xyz.y;  cell[2][c] = xyz.z; }
void setColumn(uint8_t c, const float2& xy, float z)                     { cell[0][c] = xy.x;   cell[1][c] = xy.y;   cell[2][c] = z;     }
void setColumn(uint8_t c, float x, const float2& yz)                     { cell[0][c] = x;      cell[1][c] = yz.x;   cell[2][c] = yz.y;  }
void setColumn(uint8_t c, float x, float y, float z)                     { cell[0][c] = x;      cell[1][c] = y;      cell[2][c] = z;     }

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