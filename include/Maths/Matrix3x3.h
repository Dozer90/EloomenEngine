#pragma once

#include <Maths/MatrixRow.h>
#include <Maths/Vector2.h>
#include <Maths/Vector3.h>

#include <EASTL/array.h>

// ROW MAJOR
//
// Operations are left to right
// mat = trans * rot * scale

namespace eloo::Math {
class Matrix3x3 {
public:
    constexpr Matrix3x3() = default;
    Matrix3x3() = default;
    Matrix3x3(std::initializer_list<float> data);
    Matrix3x3(const Vector2& scalar, float rotRadians, const Vector2& translation);

    static const Matrix3x3& identity();
    static const Matrix3x3& zero();

    static Matrix3x3 createTranslation(float x, float y);
    static Matrix3x3 createScale(float x, float y);
    static Matrix3x3 createRotation(float radians);
    static Matrix3x3 createShear(float x, float y);

    inline static Matrix3x3& translate(float x, float y, Matrix3x3& mat) { mat *= createTranslation(x, y); return mat; }
    inline static Matrix3x3& scale(float x, float y, Matrix3x3& mat) { mat *= createScale(x, y); return mat; }
    inline static Matrix3x3& rotate(float radians, Matrix3x3& mat) { mat *= createRotation(radians); return mat; }
    inline static Matrix3x3& shear(float x, float y, Matrix3x3& mat) { mat *= createShear(x, y); return mat; }

    inline static Matrix3x3 createTranslation(const Vector2& translation) { return createTranslation(translation.x, translation.y); }
    inline static Matrix3x3 createScale(const Vector2& scalar) { return createScale(scalar.x, scalar.y); }
    inline static Matrix3x3 createScale(float scalar) { return createScale(scalar, scalar); }
    inline static Matrix3x3 createShear(const Vector2& shearing) { return createShear(shearing.x, shearing.y); }

    inline static Matrix3x3& translate(const Vector2& translation, Matrix3x3& mat) { return translate(translation.x, translation.y, mat); }
    inline static Matrix3x3& scale(const Vector2& scalar, Matrix3x3& mat) { return scale(scalar.x, scalar.y, mat); }
    inline static Matrix3x3& shear(const Vector2& shearing, Matrix3x3& mat) { return shear(shearing.x, shearing.y, mat); }

    inline void setIdentity() { *this = identity(); }

public:
    friend bool operator!=(const Matrix3x3& lhs, const Matrix3x3& rhs);
    inline friend bool operator==(const Matrix3x3& lhs, const Matrix3x3& rhs) { return !(lhs != rhs); }

    friend Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs);
    friend Matrix3x3 operator*(const Matrix3x3& mat, float scalar);
    friend Matrix3x3 operator*(const Matrix3x3& mat, int scalar);
    friend Vector2 operator*(const Vector2& v, const Matrix3x3& mat);
    friend Vector3 operator*(const Vector3& v, const Matrix3x3& mat);

    friend Matrix3x3 operator/(Matrix3x3 lhs, const Matrix3x3& rhs);
    friend Matrix3x3 operator/(Matrix3x3 mat, float scalar);
    friend Matrix3x3 operator/(Matrix3x3 mat, int scalar);

    friend Matrix3x3 operator+(Matrix3x3 lhs, const Matrix3x3& rhs);
    friend Matrix3x3 operator+(Matrix3x3 mat, float val);
    friend Matrix3x3 operator+(Matrix3x3 mat, int val);

    friend Matrix3x3 operator-(Matrix3x3 lhs, const Matrix3x3& rhs);
    friend Matrix3x3 operator-(Matrix3x3 mat, float val);
    friend Matrix3x3 operator-(Matrix3x3 mat, int val);

    inline Matrix3x3& operator*=(const Matrix3x3& rhs) { return (*this = *this * rhs); }
    inline Matrix3x3& operator*=(float scalar) { return (*this = *this * scalar); }
    inline Matrix3x3& operator*=(int scalar) { return (*this = *this * scalar); }

    inline Matrix3x3& operator/=(const Matrix3x3& rhs) { return (*this = *this / rhs); }
    inline Matrix3x3& operator/=(float scalar) { return (*this = *this / scalar); }
    inline Matrix3x3& operator/=(int scalar) { return (*this = *this / scalar); }

    inline Matrix3x3& operator+=(const Matrix3x3& rhs) { return (*this = *this + rhs); }
    inline Matrix3x3& operator+=(float scalar) { return (*this = *this + scalar); }
    inline Matrix3x3& operator+=(int scalar) { return (*this = *this + scalar); }

    inline Matrix3x3& operator-=(const Matrix3x3& rhs) { return (*this = *this - rhs); }
    inline Matrix3x3& operator-=(float scalar) { return (*this = *this - scalar); }
    inline Matrix3x3& operator-=(int scalar) { return (*this = *this - scalar); }

    inline MatrixRow<float> operator[](uint8_t rowIndex) { return MatrixRow(&mData[rowIndex], 3); }
    inline MatrixRow<const float> operator[](uint8_t rowIndex) const { return MatrixRow(&mData[rowIndex], 3); }

private:
    eastl::array<float, 9> mData = { 1.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f };
};
} // namespace Math