#include <Camera.h>

using namespace eloo;

math::Matrix4x4 Camera::getViewMatrix() const {
	return math::Matrix4x4::createView(mTransform.position(), mTransform.forward(), math::float3::up());
}

math::Matrix4x4 Camera::getOrthoProjMatrix() const {
	const math::float3 boundsMin = { -mViewSize.x * 0.5f, -mViewSize.y * 0.5f, mClippingPlanes.x };
	const math::float3 boundsMax = { mViewSize.x * 0.5f, mViewSize.y * 0.5f, mClippingPlanes.y };
	const math::Matrix4x4 viewMatrix = math::Matrix4x4::createView(mTransform.position(), mTransform.forward(), math::float3::up());
	const math::Matrix4x4 projMatrix = math::Matrix4x4::createOrthographicProjection(boundsMin, boundsMax);
	return projMatrix * viewMatrix;
}

math::Matrix4x4 Camera::getPerspectiveProjMatrix() const {
	const math::float3 boundsMin = { -mViewSize.x * 0.5f, -mViewSize.y * 0.5f, mClippingPlanes.x };
	const math::float3 boundsMax = { mViewSize.x * 0.5f, mViewSize.y * 0.5f, mClippingPlanes.y };
	const math::Matrix4x4 viewMatrix = math::Matrix4x4::createView(mTransform.position(), mTransform.forward(), math::float3::up());
	const math::Matrix4x4 projMatrix = math::Matrix4x4::createPerspectiveProjection(mFOV, mViewSize.x / mViewSize.y, mClippingPlanes.x, mClippingPlanes.y);
	return projMatrix * viewMatrix;
}