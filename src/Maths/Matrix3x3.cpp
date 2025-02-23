#include <Maths/CoreMath.h>

#include <Generic/Utilities.h>

using namespace eloo::Math;

namespace {
static constexpr Matrix3x3 gZero({ 0.0f }, { 0.0f }, { 0.0f });
static constexpr Matrix3x3 gOne({ 1.0f }, { 1.0f }, { 1.0f });
static constexpr Matrix3x3 gIdentity;
}

float& Matrix3x3::Row::operator [] (uint8_t index) {
    ASSERT(index < 3, "Attempting to get invalid column %d from Matrix3x3::Row.", index);
    return *((&x) + index);
}

const float& Matrix3x3::Row::operator [] (uint8_t index) const {
    ASSERT(index < 3, "Attempting to get invalid column %d from Matrix3x3::Row.", index);
    return *((&x) + index);
}

const float& Matrix3x3::Column::operator [] (uint8_t index) const {
    ASSERT(index < 3, "Attempting to get invalid row %d from Matrix3x3::Column.", index);
    return *((&x) + index);
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
    return { m.a11, m.a21, m.a31,
             m.a12, m.a22, m.a32, 
             m.a13, m.a23, m.a33 };
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

MatrixRow3x3<float> Matrix3x3::row(uint8_t rowIndex) {
    return MatrixRow3x3(rowIndex, data.data());
}

const MatrixRow3x3<const float> Matrix3x3::row(uint8_t rowIndex) const {
    return MatrixRow3x3(rowIndex, data.data());
}

MatrixColumn3x3<float> Matrix3x3::column(uint8_t colIndex) {
    return MatrixColumn3x3(colIndex, data.data());
}

const MatrixColumn3x3<const float> Matrix3x3::column(uint8_t colIndex) const {
    return MatrixColumn3x3(colIndex, data.data());
}

void setRow(uint8_t r, const float3& xyz)                                { row[r] = xyz; }
void setRow(uint8_t r, const float2& xy, float z)                        { row[r] = { xy.x, xy.y, z }; }
void setRow(uint8_t r, float x, const float2& yz)                        { row[r] = { x, yz.x, yz.y }; }
void setRow(uint8_t r, float x, float y, float z)                        { row[r] = { x, y, z }; }

MatrixColumn3x3 column(uint8_t c) const                                  { return { cell[0][c], cell[1][c], cell[2][c] }; }
void setColumn(uint8_t c, const float3& xyz)                             { cell[0][c] = xyz.x;  cell[1][c] = xyz.y;  cell[2][c] = xyz.z; }
void setColumn(uint8_t c, const float2& xy, float z)                     { cell[0][c] = xy.x;   cell[1][c] = xy.y;   cell[2][c] = z;     }
void setColumn(uint8_t c, float x, const float2& yz)                     { cell[0][c] = x;      cell[1][c] = yz.x;   cell[2][c] = yz.y;  }
void setColumn(uint8_t c, float x, float y, float z)                     { cell[0][c] = x;      cell[1][c] = y;      cell[2][c] = z;     }