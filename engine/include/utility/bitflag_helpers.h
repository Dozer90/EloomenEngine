#pragma once

#include "utility/defines.h"

#include <EASTL/type_traits.h>

namespace eloo {
    // ==========================================================================================================
    // Bitflag support for enums
    // ==========================================================================================================

    template <typename TEnum>
    concept is_bitflag_enum =
        eastl::is_enum_v<TEnum> &&
        eastl::is_integral_v<std::underlying_type_t<TEnum>> &&
        eastl::is_unsigned_v<std::underlying_type_t<TEnum>>;

    template <is_bitflag_enum T>
    ELOO_NODISCARD ELOO_FORCE_INLINE constexpr T operator | (T lhs, T rhs) ELOO_NOEXCEPT {
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) | static_cast<std::underlying_type_t<T>>(rhs));
    }

    template <is_bitflag_enum T>
    ELOO_NODISCARD ELOO_FORCE_INLINE constexpr T operator & (T lhs, T rhs) ELOO_NOEXCEPT {
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) & static_cast<std::underlying_type_t<T>>(rhs));
    }

    template <is_bitflag_enum T>
    ELOO_NODISCARD ELOO_FORCE_INLINE constexpr T operator ^ (T lhs, T rhs) ELOO_NOEXCEPT {
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) ^ static_cast<std::underlying_type_t<T>>(rhs));
    }

    template <is_bitflag_enum T>
    ELOO_NODISCARD ELOO_FORCE_INLINE constexpr T operator ~ (T value) ELOO_NOEXCEPT {
        return static_cast<T>(~static_cast<std::underlying_type_t<T>>(value));
    }

    template <is_bitflag_enum T>
    ELOO_NODISCARD ELOO_FORCE_INLINE constexpr T& operator |= (T& lhs, T rhs) ELOO_NOEXCEPT {
        lhs = lhs | rhs;
        return lhs;
    }

    template <is_bitflag_enum T>
    ELOO_NODISCARD ELOO_FORCE_INLINE constexpr T& operator &= (T& lhs, T rhs) ELOO_NOEXCEPT {
        lhs = lhs & rhs;
        return lhs;
    }

    template <is_bitflag_enum T>
    ELOO_NODISCARD ELOO_FORCE_INLINE constexpr T& operator ^= (T& lhs, T rhs) ELOO_NOEXCEPT {
        lhs = lhs ^ rhs;
        return lhs;
    }

    template <is_bitflag_enum T>
    ELOO_NODISCARD ELOO_FORCE_INLINE constexpr bool operator ? (T lhs, T rhs) ELOO_NOEXCEPT {
        return static_cast<bool>(static_cast<std::underlying_type_t<T>>(lhs) & static_cast<std::underlying_type_t<T>>(rhs));
    }
}