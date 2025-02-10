#include <Maths/Transform3D.h>

using namespace eloo::Math;

float3 Transform3D::forward() const {
	return float3::forward() * mRotation;
}

float3 Transform3D::right() const {
	return float3::right() * mRotation;
}

float3 Transform3D::up() const {
	return float3::up() * mRotation;
}