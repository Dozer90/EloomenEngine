#pragma once

#include "utility/defines.h"

#include <filesystem>


namespace sge::texture {
    SGE_DECLARE_ID_T;

    struct properties {
        uint32_t width;
        uint32_t height;
        uint8_t channels;
        const uint8_t* data;
        bool isSRGB = false;
    };
 
    id_t load_from_file(std::filesystem::path filePath) SGE_NOEXCEPT;

    id_t create(const properties& props) SGE_NOEXCEPT;
    bool release(id_t id) SGE_NOEXCEPT;

    bool is_valid(id_t id) SGE_NOEXCEPT;

    bool try_get_properties(id_t id, properties& props) SGE_NOEXCEPT;
}