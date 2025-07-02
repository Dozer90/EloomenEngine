#include "rendering/texture.h"

#include "utility/managed_memory_block.h"


using namespace eloo;

namespace {
    constexpr int MEMORY_BLOCK_INITIAL_SIZE = 1000;
    constexpr float MEMORY_BLOCK_EXPANSION = 0.4f;
    static managed_memory_block<texture::properties, MEMORY_BLOCK_INITIAL_SIZE, MEMORY_BLOCK_EXPANSION> gTextureBlock;
}

texture::id_t texture::create(const properties& props) ELOO_NOEXCEPT {
    const size_t id = gTextureBlock.emplace(props);
    return texture::id_t(id);
}

bool texture::release(id_t id) ELOO_NOEXCEPT {
    if (!is_valid(id)) {
        return false;
    }
    gTextureBlock.remove(static_cast<size_t>(id));
    return true;
}

bool texture::is_valid(id_t id) ELOO_NOEXCEPT {
    return gTextureBlock.is_valid(static_cast<size_t>(id));
}

bool texture::try_get_properties(id_t id, properties& props) ELOO_NOEXCEPT {
    return gTextureBlock.try_get(static_cast<size_t>(id), props);
}