#pragma once

#include "window_interface.h"

#include <EASTL/unique_ptr.h>


/* RENDER PIPELINE

[Local -> World]    - Iterate over all active Transform components and create their model matrix (M)
[World -> Camera]   - Compute the view matrix (V) using Matrix4x4::lookAt(...)
[Camera -> Clip]    - Compute the projection matrix (P) using Matrix4x4::perspective(...) or Matrix4x4::orthographic(...)
                       ~ NOTE: The last two steps can likely be simplified to just activeCamera.project()
                    - Transform each vertex position: vP_clip = P * V * M * vP_local
[Clip -> NDC]       - Perform perspective division by dividing vP_clip by its w component (this is called Normalized Device Coordinates (NDC))
[NDC -> Screen]     - Remap NDC positions from the ranges [-1,1] to [0,screenWidth] and [0,screenHeight]

Result:
======================================================================================================================
    vP_screen = viewport * (vP_clip / vP_clip.w) = viewport * ((P * V * M * vP_local) / (P * V * M * vP_local).w)
======================================================================================================================
*/


namespace eloo {
    class renderer_interface {
    public:
        virtual ~renderer_interface() = default;

        virtual constexpr const char* renderer_name() const = 0;
        virtual constexpr const char* platform_name() const = 0;
        virtual constexpr const char* vendor_name() const = 0;
        virtual constexpr unsigned int major_version() const = 0;
        virtual constexpr unsigned int minor_version() const = 0;
        virtual constexpr unsigned int patch_version() const = 0;

        virtual void render() = 0;

        unsigned long long current_frame() const { return mFrameIndex; }

    protected:
        void increment_frame() { mFrameIndex++; }

        virtual void on_window_resized(int width, int height) = 0;

    private:
        unsigned long long mFrameIndex = 0;
    };
}