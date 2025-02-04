#include <Camera.h>

using namespace eloo;

Math::Matrix4x4 Camera::getViewMatrix() const {
	return Math::Matrix4x4::createView(mTransform.position(), mTransform.forward(), Math::Vector3::up());
}

Math::Matrix4x4 Camera::getOrthoProjMatrix() const {
	const Math::Vector3 boundsMin = { -mViewSize.x * 0.5f, -mViewSize.y * 0.5f, mClippingPlanes.x };
	const Math::Vector3 boundsMax = { mViewSize.x * 0.5f, mViewSize.y * 0.5f, mClippingPlanes.y };
	const Math::Matrix4x4 viewMatrix = Math::Matrix4x4::createView(mTransform.position(), mTransform.forward(), Math::Vector3::up());
	const Math::Matrix4x4 projMatrix = Math::Matrix4x4::createOrthographicProjection(boundsMin, boundsMax);
	return projMatrix * viewMatrix;
}

Math::Matrix4x4 Camera::getPerspectiveProjMatrix() const {
	const Math::Vector3 boundsMin = { -mViewSize.x * 0.5f, -mViewSize.y * 0.5f, mClippingPlanes.x };
	const Math::Vector3 boundsMax = { mViewSize.x * 0.5f, mViewSize.y * 0.5f, mClippingPlanes.y };
	const Math::Matrix4x4 viewMatrix = Math::Matrix4x4::createView(mTransform.position(), mTransform.forward(), Math::Vector3::up());
	const Math::Matrix4x4 projMatrix = Math::Matrix4x4::createPerspectiveProjection(mFOV, mViewSize.x / mViewSize.y, mClippingPlanes.x, mClippingPlanes.y);
	return projMatrix * viewMatrix;
}