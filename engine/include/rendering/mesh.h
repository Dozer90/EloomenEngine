#pragma once

#include "datatypes/float2.h"
#include "datatypes/float3.h"
#include "datatypes/float4.h"
#include "datatypes/matrix4x4.h"
#include "utility/colour.h"
#include "utility/defines.h"
#include "rendering/renderer.h"

#include <EASTL/vector.h>

// #define MATERIALS_IMPLEMENTED


namespace eloo::mesh {
    ELOO_DECLARE_ID_T;

    struct vertex {
        float3::values position;            // Position in 3D space
        float3::values normal;              // Normal vector for lighting calculations
        float2::values texCoord;            // Texture coordinates
        float4::values tangent;             // Tangent vector for normal mapping
        colour_t color;                     // Vertex color
    };

    struct submesh_section {
        eastl::vector<uint32_t> indices;    // Indices for this section
#if defined(MATERIALS_IMPLEMENTED)
        material::id_t materialID;          // ID of the material used for this section
#endif
    };

    struct data {
        eastl::vector<vertex> vertices;     // Vertices of the mesh
        eastl::vector<submesh_section> sections; // Sections of the mesh, each with its own material and indices
    };

    id_t create(const mesh::data& meshData, renderer_interface* renderer) ELOO_NOEXCEPT;
    bool release(id_t id, renderer_interface* renderer) ELOO_NOEXCEPT;

    bool is_valid(id_t id, int submesh = 0) ELOO_NOEXCEPT;

    bool try_get_data(id_t id, data& meshData) ELOO_NOEXCEPT;

#if defined(MATERIALS_IMPLEMENTED)
    bool try_set_material(id_t id, id_t materialID, int submeshIndex) ELOO_NOEXCEPT;
#endif
}