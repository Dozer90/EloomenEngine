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


namespace eloo {
    namespace mesh          { struct id_t; }
    namespace entity        { struct id_t; }
    namespace vertex_buffer { ELOO_DECLARE_ID_T; }
    namespace index_buffer  { ELOO_DECLARE_ID_T; }

    class window_interface;
    class renderer_interface {
    public:
        virtual ~renderer_interface() = default;

        virtual constexpr const char* renderer_name() const = 0;
        virtual constexpr const char* platform_name() const = 0;
        virtual constexpr const char* vendor_name() const = 0;
        virtual constexpr unsigned int major_version() const = 0;
        virtual constexpr unsigned int minor_version() const = 0;
        virtual constexpr unsigned int patch_version() const = 0;

        virtual vertex_buffer::id_t create_vertex_buffer(const void* data, size_t byteSize, bool dynamic = false) = 0;
        virtual index_buffer::id_t create_index_buffer(const void* data, size_t byteSize, bool dynamic = false) = 0;
        virtual void destroy_vertex_buffer(vertex_buffer::id_t id) = 0;
        virtual void destroy_index_buffer(index_buffer::id_t id) = 0;

        virtual bool register_mesh(mesh::id_t meshID) = 0;
        virtual bool deregister_mesh(mesh::id_t meshID) = 0;

        virtual bool submit_for_draw(mesh::id_t meshID) = 0;
        virtual bool submit_for_draw(mesh::id_t meshID, entity::id_t ownerTransformID) = 0;

        virtual void render(const window_interface* wnd) = 0;

        unsigned long long current_frame() const { return mFrameIndex; }

    protected:
        void increment_frame() { mFrameIndex++; }

        virtual void on_window_resized(int width, int height) = 0;

    private:
        unsigned long long mFrameIndex = 0;
    };
}