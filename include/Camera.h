#pragma once

#include <Maths/Transform3D.h>
#include <Maths/Matrix4x4.h>
#include <Maths/Vector2.h>

namespace eloo {

class Camera {
public:
	Math::Matrix4x4 getViewMatrix() const;
	Math::Matrix4x4 getOrthoProjMatrix() const;
	Math::Matrix4x4 getPerspectiveProjMatrix() const;

private:
	Math::Transform3D mTransform;
	float mFOV = 70.0f;
	Math::Vector2 mViewSize;
	Math::Vector2 mClippingPlanes;
};
}