#pragma once

#include "datatypes/float3.h"
#include "datatypes/float4.h"
#include "rendering/texture.h"
#include "utility/colour.h"
#include "utility/defines.h"


namespace eloo::material {
    ELOO_DECLARE_ID_T;

    struct properties {
        colour_t albedo;
        colour_t emissive;
        float3::values emissiveIntensity;
        texture::id_t baseColorTexture = texture::id_t::INVALID;
        texture::id_t normalTexture = texture::id_t::INVALID;
        texture::id_t metallicRoughnessTexture = texture::id_t::INVALID;
        texture::id_t emissiveTexture = texture::id_t::INVALID;
        float metallicFactor;
        float roughnessFactor;
    };

    id_t create(const properties& props) ELOO_NOEXCEPT;
    bool release(id_t id) ELOO_NOEXCEPT;

    bool is_valid(id_t id) ELOO_NOEXCEPT;

    bool try_get_properties(id_t id, properties& props) ELOO_NOEXCEPT;
}