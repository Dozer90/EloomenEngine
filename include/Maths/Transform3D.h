#pragma once

#include <Maths/Quaternion.h>

namespace eloo::Math {
class Vector3;
class Quaternion;
class Matrix4x4;

class Transform3D {
public:
	Vector3 forward() const;
	Vector3 right() const;
	Vector3 up() const;

	Vector3& position() { return mPosition; }
	Quaternion& rotation() { return mRotation; }
	Vector3& scale() { return mScale; }
	const Vector3& position() const { return mPosition; }
	const Quaternion& rotation() const { return mRotation; }
	const Vector3& scale() const { return mScale; }

private:
    Vector3 mPosition;
    Quaternion mRotation;
    Vector3 mScale;
};
} // eloo::Math