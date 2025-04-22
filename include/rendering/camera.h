#pragma once

#include <Maths/Transform3D.h>
#include <Maths/Matrix4x4.h>
#include <Maths/float2.h>

namespace eloo {

class Camera {
public:
	math::Matrix4x4 getViewMatrix() const;
	math::Matrix4x4 getOrthoProjMatrix() const;
	math::Matrix4x4 getPerspectiveProjMatrix() const;

private:
	math::Transform3D mTransform;
	float mFOV = 70.0f;
	math::float2 mViewSize;
	math::float2 mClippingPlanes;
};
}