#pragma once

namespace eloo::Math {
class Vector3;
class Quaternion;
class Matrix4x4;

class Transform2D {
public:
    Matrix4x4 toMatrix() const;
    void fromMatrix(const Matrix4x4& mat);

private:
    Vector3 mPosition;
    Quaternion mRotation;
    Vector3 mScale;
};
} // eloo::Math