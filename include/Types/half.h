#pragma once

#include <bit>
#include <cstdint>
#include <EASTL/type_traits.h>

namespace eloo {
    struct half;
}
typedef double float64_t;
typedef float float32_t;
typedef eloo::half float16_t;

namespace eloo {

struct half {
private:
    uint16_t mBits = 0;

public:
    half() = default;
    constexpr inline half(uint32_t bits) : mBits(bits) {}
    constexpr inline half(float32_t f) : mBits(float32_to_16(f)) {}
    constexpr inline half(float64_t d) : mBits(float32_to_16(static_cast<float>(d))) {}

    constexpr inline operator float32_t() const { return float16_to_32(*this); }

    inline half& operator = (float32_t f) {
        mBits = float32_to_16(f);
        return *this;
    }

    constexpr static half float32_to_16(float32_t f) {
        // IEEE 754 single-precision (float) bit layout:
        //  1 bit  : Sign
        //  8 bits : Exponent
        // 23 bits : Mantissa (only 10 used in half-float)
        constexpr uint32_t f_sign_mask = 0x80000000U;
        constexpr uint32_t f_exp_mask  = 0x7F800000U;
        constexpr uint32_t f_mant_mask = 0x007FFFFFU;
        constexpr uint32_t f_half_bias = 0x38000000U;

        uint32_t bits = std::bit_cast<uint32_t>(f);
        uint32_t sign = (bits >> 16) & 0x8000;
        uint32_t exponent = ((bits >> 23) & 0xFF) - 0x70;
        uint32_t mantissa = (bits & 0x007FFFFF) >> 13;

        if (exponent <= 0) {
            // Subnormal numbers (denormals)
            return sign | (mantissa >> (1 - exponent));
        }
        if (exponent >= 31) {
            // Infinity/NaN
            return sign | 0x7C00 | (mantissa ? 1 : 0);
        }
        return half(sign | (exponent << 10) | mantissa);
    }

    constexpr static float32_t float16_to_32(half h) {
        uint32_t bits = 0;
        uint32_t sign = (h.mBits & 0x8000) << 16;
        uint32_t exponent = (h.mBits & 0x7C00) >> 10;
        uint32_t mantissa = h.mBits & 0x03FF;

        if (exponent == 31) {
            // Infinity/NaN
            bits = sign | 0x7F800000 | (mantissa << 13);
        } else if (exponent == 0) {
            // Subnormal numbers (denormals)
            if (mantissa == 0) {
                // Signed zero
                bits = sign;
            } else {
                exponent = 1;
                while ((mantissa & 0x0400) == 0) {
                    mantissa <<= 1;
                    exponent--;
                }
                bits = sign | ((exponent + 0x70) << 23) | ((mantissa & 0x03FF) << 13);
            }
        } else {
            bits = sign | ((exponent + 0x70) << 23) | (mantissa << 13);
        }
        return std::bit_cast<float32_t>(bits);
    }
};

constexpr half operator"" _h(long double v) {
    return half(static_cast<float32_t>(v));
}
}

// Include half (float16_t) in the floating point checks
namespace eastl {
template<> struct is_floating_point<float16_t> : eastl::true_type {};
}