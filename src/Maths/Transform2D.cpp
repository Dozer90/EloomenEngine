#include <Maths/Transform2D.h>

using namespace eloo::Math;

Matrix3x3 Transform2D::toMatrix() const {
    Matrix3x3 m = Matrix3x3::createScale(mScale);
    Matrix3x3::rotate(mRotation, m);
    Matrix3x3::translate(mPosition, m);
    return m;
}

void Transform2D::fromMatrix(const Matrix3x3& mat) {
    mPosition = { mat[0][2], mat[1][2] };
    mScale.x = sqrt(mat[0][0] * mat[0][0] + mat[1][0] * mat[1][0]);
    mScale.y = sqrt(mat[0][1] * mat[0][1] + mat[1][1] * mat[1][1]);
    mRotation = atan2(mat[1][0] / mScale.x, mat[0][0] / mScale.x);
}