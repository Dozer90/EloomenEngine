#pragma once

#include <Maths/float3.h>
#include <Maths/Quaternion.h>

namespace eloo::Math {
class Transform3D {
public:
	float3 forward() const;
	float3 right() const;
	float3 up() const;

	float3& position()                 { return mPosition; }
	const float3& position() const     { return mPosition; }
	Quaternion& rotation()              { return mRotation; }
	const Quaternion& rotation() const  { return mRotation; }
	float3& scale()                    { return mScale; }
	const float3& scale() const        { return mScale; }

private:
    float3 mPosition;
    Quaternion mRotation;
    float3 mScale;
};
} // eloo::Math