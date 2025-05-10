#pragma once

#include <bit>
#include <cstdint>
#include <limits>

#include <EASTL/type_traits.h>
#include <EASTL/numeric_limits.h>

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
        constexpr inline half(float32_t f) : mBits(float32_to_16(f)) {}
        constexpr inline half(float64_t d) : mBits(float32_to_16(static_cast<float>(d))) {}

        constexpr inline operator float32_t() const { return float16_to_32(*this); }

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
            int32_t exponent = ((bits >> 23) & 0xFF) - 0x70;
            uint32_t mantissa = (bits & 0x007FFFFF) >> 13;

            if (exponent <= 0) {
                if (exponent < -10) {
                    return from_bits(sign); // Too small to be represented as subnormal
                }
                mantissa = (mantissa | 0x0800) >> (1 - exponent);
                return from_bits(sign | mantissa);
            }
            if (exponent >= 31) {
                return from_bits(sign | 0x7C00 | (mantissa ? 1 : 0)); // Inf/NaN
            }
            return from_bits(sign | (exponent << 10) | mantissa);
        }

        inline constexpr static float32_t float16_to_32(uint16_t f16bits) {
            uint32_t f32bits = 0;
            uint32_t sign = (f16bits & 0x8000) << 16;
            uint32_t exponent = (f16bits & 0x7C00) >> 10;
            uint32_t mantissa = f16bits & 0x03FF;

            if (exponent == 31) {
                // Infinity/NaN
                f32bits = sign | 0x7F800000 | (mantissa << 13);
            } else if (exponent == 0) {
                // Subnormal numbers (denormals)
                if (mantissa == 0) {
                    // Signed zero
                    f32bits = sign;
                }
                else {
                    exponent = 1;
                    while ((mantissa & 0x0400) == 0) {
                        mantissa <<= 1;
                        exponent--;
                    }
                    f32bits = sign | ((exponent + 0x70) << 23) | ((mantissa & 0x03FF) << 13);
                }
            } else {
                f32bits = sign | ((exponent + 0x70) << 23) | (mantissa << 13);
            }
            return std::bit_cast<float32_t>(f32bits);
        }

        inline constexpr static float32_t float16_to_32(half h) {
            return float16_to_32(h.mBits);
        }

        constexpr uint16_t to_bits() const noexcept {
            return mBits;
        }

        constexpr static half from_bits(uint16_t bits) noexcept {
            half h;
            h.mBits = bits;
            return h;
        }

        constexpr float as_float32() const noexcept {
            return float16_to_32(*this);
        }

    public:
        friend constexpr inline half operator + (half lhs) noexcept {
            return lhs;
        }
        friend constexpr inline half operator - (half lhs) noexcept {
            return half::from_bits(lhs.mBits ^ 0x8000);
        }

        friend constexpr inline bool operator == (half lhs, half rhs) noexcept {
            return lhs.mBits == rhs.mBits;
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator == (half lhs, T rhs) noexcept {
            return static_cast<float>(lhs) == static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator == (T lhs, half rhs) noexcept {
            return static_cast<float>(lhs) == static_cast<float>(rhs);
        }

        friend constexpr inline bool operator != (half lhs, half rhs) noexcept {
            return lhs.mBits != rhs.mBits;
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator != (half lhs, T rhs) noexcept {
            return static_cast<float>(lhs) != static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator != (T lhs, half rhs) noexcept {
            return static_cast<float>(lhs) != static_cast<float>(rhs);
        }

        friend constexpr inline bool operator < (half lhs, half rhs) noexcept {
            return static_cast<float>(lhs) < static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator < (half lhs, T rhs) noexcept {
            return static_cast<float>(lhs) < static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator < (T lhs, half rhs) noexcept {
            return static_cast<float>(lhs) < static_cast<float>(rhs);
        }

        friend constexpr inline bool operator > (half lhs, half rhs) noexcept {
            return static_cast<float>(lhs) > static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator > (half lhs, T rhs) noexcept {
            return static_cast<float>(lhs) > static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator > (T lhs, half rhs) noexcept {
            return static_cast<float>(lhs) > static_cast<float>(rhs);
        }

        friend constexpr inline bool operator <= (half lhs, half rhs) noexcept {
            return static_cast<float>(lhs) <= static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator <= (half lhs, T rhs) noexcept {
            return static_cast<float>(lhs) <= static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator <= (T lhs, half rhs) noexcept {
            return static_cast<float>(lhs) <= static_cast<float>(rhs);
        }

        friend constexpr inline bool operator >= (half lhs, half rhs) noexcept {
            return static_cast<float>(lhs) >= static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator >= (half lhs, T rhs) noexcept {
            return static_cast<float>(lhs) >= static_cast<float>(rhs);
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline bool operator >= (T lhs, half rhs) noexcept {
            return static_cast<float>(lhs) >= static_cast<float>(rhs);
        }

        friend constexpr inline half operator / (half lhs, half rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) / static_cast<float>(rhs));
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline half operator / (half lhs, T rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) / static_cast<float>(rhs));
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline half operator / (T lhs, half rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) / static_cast<float>(rhs));
        }

        friend constexpr inline half operator * (half lhs, half rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) * static_cast<float>(rhs));
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline half operator * (half lhs, T rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) * static_cast<float>(rhs));
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline half operator * (T lhs, half rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) * static_cast<float>(rhs));
        }

        friend constexpr inline half operator + (half lhs, half rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) + static_cast<float>(rhs));
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline half operator + (half lhs, T rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) + static_cast<float>(rhs));
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline half operator + (T lhs, half rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) + static_cast<float>(rhs));
        }

        friend constexpr inline half operator - (half lhs, half rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) - static_cast<float>(rhs));
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline half operator - (half lhs, T rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) - static_cast<float>(rhs));
        }
        template <typename T, typename = eastl::enable_if_t<eastl::is_arithmetic_v<T>>>
        friend constexpr inline half operator - (T lhs, half rhs) noexcept {
            return half::float32_to_16(static_cast<float>(lhs) - static_cast<float>(rhs));
        }
    };

    constexpr half operator"" _h(long double v) {
        return half::float32_to_16(static_cast<float32_t>(v));
    }
}

static_assert(sizeof(eloo::half) == sizeof(uint16_t));
static_assert(std::is_trivially_copyable_v<eloo::half>);

// Include half (float16_t) in the floating point checks
namespace std {
    template<> struct is_floating_point<float16_t> : eastl::true_type {};
}
namespace eastl {
    template<> struct is_floating_point<float16_t> : eastl::true_type {};
}

// Add numeric_limits for float16_t
#define HLF_DECIMAL_DIG  5                 // # of decimal digits of rounding precision
#define HLF_DIG          3                 // # of decimal digits of precision
#define HLF_EPSILON      9.765625e-04f     // smallest such that 1.0+HLF_EPSILON != 1.0
#define HLF_HAS_SUBNORM  1                 // type does support subnormal numbers
#define HLF_MANT_DIG     11                // # of bits in mantissa
#define HLF_MAX          65504.0f          // max value
#define HLF_MAX_10_EXP   4                 // max decimal exponent
#define HLF_MAX_EXP      16                // max binary exponent
#define HLF_MIN          6.1035156e-05f    // min positive value
#define HLF_MIN_10_EXP   (-4)              // min decimal exponent
#define HLF_MIN_EXP      (-13)             // min binary exponent
#define _HLF_RADIX       2                 // exponent radix
#define HLF_TRUE_MIN     5.9604645e-08f    // min positive value

namespace std {
    template<> struct numeric_limits<float16_t> {
        static constexpr bool is_specialized = true;

        static constexpr float16_t min() noexcept { return float16_t::float32_to_16(HLF_MIN); }             // smallest normalized positive
        static constexpr float16_t max() noexcept { return float16_t::float32_to_16(HLF_MAX); }             // max value
        static constexpr float16_t lowest() noexcept { return float16_t::float32_to_16(-HLF_MAX); }

        static constexpr float16_t epsilon() noexcept { return float16_t::float32_to_16(HLF_EPSILON); }
        static constexpr float16_t round_error() noexcept { return float16_t::float32_to_16(0.5f); }

        static constexpr float16_t infinity() noexcept { return float16_t::from_bits(0x7C00); }          // Inf
        static constexpr float16_t quiet_NaN() noexcept { return float16_t::from_bits(0x7E00); }         // Quiet NaN
        static constexpr float16_t signaling_NaN() noexcept { return float16_t::from_bits(0x7D00); }     // Signaling NaN
        static constexpr float16_t denorm_min() noexcept { return float16_t::float32_to_16(HLF_TRUE_MIN); }

        static constexpr int digits = HLF_MANT_DIG;
        static constexpr int digits10 = HLF_DIG;
        static constexpr int max_digits10 = HLF_DECIMAL_DIG;

        static constexpr int min_exponent = HLF_MIN_EXP;
        static constexpr int min_exponent10 = HLF_MIN_10_EXP;
        static constexpr int max_exponent = HLF_MAX_EXP;
        static constexpr int max_exponent10 = HLF_MAX_10_EXP;

        static constexpr bool has_infinity = true;
        static constexpr bool has_quiet_NaN = true;
        static constexpr bool has_signaling_NaN = true;
        static constexpr float_denorm_style has_denorm = denorm_present;
        static constexpr bool has_denorm_loss = false;

        static constexpr bool is_iec559 = true;
        static constexpr bool is_bounded = true;
        static constexpr bool is_modulo = false;

        static constexpr bool traps = true;
        static constexpr bool tinyness_before = false;
        static constexpr float_round_style round_style = round_to_nearest;
    };
}
namespace eastl {
    template<> struct numeric_limits<float16_t> {
        static constexpr bool is_specialized = true;

        static constexpr float16_t min() noexcept { return float16_t::float32_to_16(HLF_MIN); }             // smallest normalized positive
        static constexpr float16_t max() noexcept { return float16_t::float32_to_16(HLF_MAX); }             // max value
        static constexpr float16_t lowest() noexcept { return float16_t::float32_to_16(-HLF_MAX); }

        static constexpr float16_t epsilon() noexcept { return float16_t::float32_to_16(HLF_EPSILON); }
        static constexpr float16_t round_error() noexcept { return float16_t::float32_to_16(0.5f); }

        static constexpr float16_t infinity() noexcept { return float16_t::from_bits(0x7C00); }          // Inf
        static constexpr float16_t quiet_NaN() noexcept { return float16_t::from_bits(0x7E00); }         // Quiet NaN
        static constexpr float16_t signaling_NaN() noexcept { return float16_t::from_bits(0x7D00); }     // Signaling NaN
        static constexpr float16_t denorm_min() noexcept { return float16_t::float32_to_16(HLF_TRUE_MIN); }

        static constexpr int digits = HLF_MANT_DIG;
        static constexpr int digits10 = HLF_DIG;
        static constexpr int max_digits10 = HLF_DECIMAL_DIG;

        static constexpr int min_exponent = HLF_MIN_EXP;
        static constexpr int min_exponent10 = HLF_MIN_10_EXP;
        static constexpr int max_exponent = HLF_MAX_EXP;
        static constexpr int max_exponent10 = HLF_MAX_10_EXP;

        static constexpr bool has_infinity = true;
        static constexpr bool has_quiet_NaN = true;
        static constexpr bool has_signaling_NaN = true;
        static constexpr float_denorm_style has_denorm = denorm_present;
        static constexpr bool has_denorm_loss = false;

        static constexpr bool is_iec559 = true;
        static constexpr bool is_bounded = true;
        static constexpr bool is_modulo = false;

        static constexpr bool traps = true;
        static constexpr bool tinyness_before = false;
        static constexpr float_round_style round_style = round_to_nearest;
    };
}