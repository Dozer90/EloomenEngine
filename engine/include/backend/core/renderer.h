#pragma once

#include "utility/defines.h"

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


/*

Renderer

Uses the CRT (Curiously Recursive Template) pattern to define a common interface for all rendering backends.

*/

namespace eloo::core {
    template<typename T>
    class renderer {
        friend T;

    protected:
        explicit renderer(const char* name, const char* platform, const char* vendor, unsigned int majorVersion, unsigned int minorVersion, unsigned int patchVersion) {
            mName = name;
            mPlatform = platform;
            mVendor = vendor;
            mMajorVersion = majorVersion;
            mMinorVersion = minorVersion;
            mPatchVersion = patchVersion;
        }
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
        renderer(renderer&&) = delete;
        renderer& operator=(renderer&&) = delete;

        ~renderer() {
            ELOO_CRPT_CALL_T->cleanup();
        }

    public:
        const char* renderer_name() const   { return mName; }
        const char* platform_name() const   { return mPlatform; }
        const char* vendor_name() const     { return mVendor; }
        unsigned int major_version() const  { return mMajorVersion; }
        unsigned int minor_version() const  { return mMinorVersion; }
        unsigned int patch_version() const  { return mPatchVersion; }

        void render() {
            ELOO_CRPT_CALL_T->render();
            mFrameIndex++;
        }

        unsigned long long current_frame() const {
            return mFrameIndex;
        }

    protected:
        void on_window_resized(int width, int height) {
            ELOO_CRPT_CALL_T->on_window_resized(width, height);
        }

    private:
        const char* mName;
        const char* mPlatform;
        const char* mVendor;
        const unsigned int mMajorVersion;
        const unsigned int mMinorVersion;
        const unsigned int mPatchVersion;
        unsigned long long mFrameIndex = 0;
    };
}