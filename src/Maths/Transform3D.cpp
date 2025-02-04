#include <Maths/Transform3D.h>

#include <Maths/Matrix4x4.h>
#include <Maths/Quaternion.h>
#include <Maths/Vector3.h>

using namespace eloo::Math;

Vector3 Transform3D::forward() const {
	return Vector3::forward() * mRotation;
}

Vector3 Transform3D::right() const {
	return Vector3::right() * mRotation;
}

Vector3 Transform3D::up() const {
	return Vector3::up() * mRotation;
}