#include <Maths/Transform3D.h>

#include <Maths/Matrix4x4.h>
#include <Maths/Quaternion.h>
#include <Maths/Vector3.h>

using namespace eloo::Math;

Matrix4x4 Transform2D::toMatrix() const {
    Matrix4x4 m = Matrix4x4::createScale(mScale);
    Matrix4x4::rotate(mRotation.euler(), m);
    Matrix4x4::translate(mPosition, m);
    return m;
}

void Transform2D::fromMatrix(const Matrix4x4& mat) {
    mPosition = { mat[0][3], mat[1][3], mat[2][3] };
    mScale.x = sqrt(mat[0][0] * mat[0][0] + mat[1][0] * mat[1][0]);
    mScale.y = sqrt(mat[0][1] * mat[0][1] + mat[1][1] * mat[1][1]);
    mRotation = atan2(mat[1][0] / mScale.x, mat[0][0] / mScale.x);
}