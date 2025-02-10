#pragma once

#include <Maths/float2.h>
#include <Maths/Matrix3x3.h>

namespace eloo::Math {
class Transform2D {
public:
    Matrix3x3 toMatrix() const;
    void fromMatrix(const Matrix3x3& mat);



private:
    float2 mPosition;
    float mRotation;
    float2 mScale;
};
} // eloo::Math