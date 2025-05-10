#pragma once

#include "datatype/matrix4x4.h"
#include "datatype/float2.h"

namespace eloo {

	class Camera {
	public:
		matrix4x4::values get_view_matrix() const;
		matrix4x4::values get_orthographic_projection_matrix() const;
		matrix4x4::values get_perspective_projection_matrix() const;

	private:

		float mFOV = 70.0f;
		float2::values mViewSize;
		float2::values mClippingPlanes;
	};
}