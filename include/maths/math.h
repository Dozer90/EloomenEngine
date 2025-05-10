#pragma once

#include "utility/defines.h"

#include "maths/constants.h"
#include "maths/interpolation.h"

#include "datatype/float2.h"
#include "datatype/float3.h"
#include "datatype/float4.h"
#include "datatype/quaternion.h"
#include "datatype/matrix2x2.h"
#include "datatype/matrix3x3.h"
#include "datatype/matrix4x4.h"

#include <EASTL/type_traits.h>

#include <cmath>
#include <bit>

namespace eloo::math {

    namespace {
        template <typename T> concept is_numeric_t = ::eastl::is_arithmetic_v<T>;
        template <typename T> concept is_floating_t = ::eastl::is_floating_point_v<T>;
        template <typename T> concept is_integral_t = ::eastl::is_integral_v<T>;
        template <typename T> concept is_signed_integral_t = ::eastl::is_integral_v<T> && ::eastl::is_signed_v<T> && !eastl::is_same_v<T, bool>;
        template <typename T> concept is_unsigned_integral_t = ::eastl::is_integral_v<T> && !::eastl::is_signed_v<T>;
        template <is_numeric_t T1, is_numeric_t T2> using get_shared_common_t = ::eastl::common_type_t<T1, T2>;

        template <is_floating_t T>
        using uint_equiv_t =
            eastl::conditional_t<sizeof(T) == 2, uint16_t,
            eastl::conditional_t<sizeof(T) == 4, uint32_t,
            eastl::conditional_t<sizeof(T) == 8, uint64_t,
            void>>>;

        template <is_floating_t T>
        inline constexpr int32_t float_exponent_bits =
            ::eastl::is_same_v<T, float64_t> ? 0x03FF : // 1023
            ::eastl::is_same_v<T, float32_t> ? 0x007F : // 127
            ::eastl::is_same_v<T, float16_t> ? 0x000F : // 15
            0x0000;
        template <is_floating_t T>
        inline constexpr int32_t float_mantissa_bits =
            ::eastl::is_same_v<T, float64_t> ? 0x0034 : // 52
            ::eastl::is_same_v<T, float32_t> ? 0x0017 : // 23
            ::eastl::is_same_v<T, float16_t> ? 0x000A : // 10
            0x0000;

#define DECLARE_FUNC_FOR_FLOAT16(f) \
        template <> ELOO_FORCE_INLINE float16_t f(float16_t v) { \
            return float16_t(f(float16_t::float16_to_32(v))); \
        }
#define DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(f) \
        template <> ELOO_FORCE_INLINE constexpr float16_t f(float16_t v) { \
            return float16_t(f(float16_t::float16_to_32(v))); \
        }

#define DECLARE_FUNC_FOR_FLOAT16_2ARGS(f) \
        template <> ELOO_FORCE_INLINE float16_t f(float16_t x, float16_t y) { \
            return float16_t(f(float16_t::float16_to_32(x), float16_t::float16_to_32(y))); \
        }
#define DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16_2ARGS(f) \
        template <> ELOO_FORCE_INLINE constexpr float16_t f(float16_t x, float16_t y) { \
            return float16_t(f(float16_t::float16_to_32(x), float16_t::float16_to_32(y))); \
        }
    }


    /////////////////////////////////////////////////////////
    // Signs

    template <is_signed_integral_t T>
    ELOO_FORCE_INLINE constexpr T abs(T v) {
        return std::abs(v);
    }

    template <is_numeric_t T>
    requires eastl::is_unsigned_v<T>
    ELOO_FORCE_INLINE constexpr T abs(T v) {
        return v;
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T abs(T v) {
        return std::abs(v);
    }


    /////////////////////////////////////////////////////////
    // Comparison

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr bool is_inf(T val) {
        return std::isinf(val);
    }

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr bool is_nan(T val) {
        return std::isnan(val);
    }

    template <is_numeric_t T1, is_numeric_t T2, typename TOut = get_shared_common_t<T1, T2>>
    ELOO_FORCE_INLINE constexpr TOut min(T1 v1, T2 v2) {
        return static_cast<TOut>(v1 < v2 ? v1 : v2);
    }

    template <is_numeric_t T1, is_numeric_t T2, typename TOut = get_shared_common_t<T1, T2>>
    ELOO_FORCE_INLINE constexpr TOut max(T1 v1, T2 v2) {
        return static_cast<TOut>(v1 > v2 ? v1 : v2);
    }

    template <is_floating_t T1, is_numeric_t T2>
    ELOO_FORCE_INLINE constexpr bool is_close_to_with_tolerance(T1 val1, T2 val2, T1 relTolerance, T1 absTolerance) {
        T1 diff = abs(val1 - static_cast<T1>(val2));
        return diff <= absTolerance || diff <= relTolerance * max(abs(val1), abs(static_cast<T1>(val2)));
    }

    template <is_floating_t T1, is_numeric_t T2>
    ELOO_FORCE_INLINE constexpr bool is_close_to(T1 val1, T2 val2) {
        if constexpr (sizeof(T1) == 8) {
            return is_close_to_with_tolerance(val1, val2, f64::CLOSE_REL_TOLERANCE, f64::CLOSE_ABS_TOLERANCE);
        } else if constexpr (sizeof(T1) == 4) {
            return is_close_to_with_tolerance(val1, val2, f32::CLOSE_REL_TOLERANCE, f32::CLOSE_ABS_TOLERANCE);
        } else {
            return is_close_to_with_tolerance(val1, val2, f16::CLOSE_REL_TOLERANCE, f16::CLOSE_ABS_TOLERANCE);
        }
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr bool is_close_to_zero(T val) {
        return is_close_to(val, 0);
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr bool is_close_to_zero_with_tolerance(T val, T relTolerance, T absTolerance) {
        return is_close_to_with_tolerance(val, 0, relTolerance, absTolerance);
    }

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr int sign(T v) {
        if constexpr (is_floating_t<T>) {
            return std::signbit(v) ? (is_close_to_zero(v) ? 0 : -1) : (v > 0);
        } else {
            return static_cast<int>((v > 0) - (v < 0));
        }
    }


    /////////////////////////////////////////////////////////
    // Logarithm

    // Provides ~99.7% accuracy, significantly faster than log2()
    ELOO_FORCE_INLINE constexpr float64_t fast_log2(float64_t v) {
        return std::bit_cast<float64_t>(std::bit_cast<uint64_t>(v) * 1.4426950408889634e-16 - float_exponent_bits<float64_t>);
    }
    ELOO_FORCE_INLINE constexpr float32_t fast_log2(float32_t v) {
        return std::bit_cast<float32_t>(std::bit_cast<uint32_t>(v) * 1.1920928955078125e-7f - float_exponent_bits<float32_t>);
    }
    ELOO_FORCE_INLINE constexpr float16_t fast_log2(float16_t v) {
        return float16_t::from_bits(v.to_bits() * 5.960464477539063e-8_h - float_exponent_bits<float16_t>);
    }

    // Provides ~99.7% accuracy, significantly faster than ln()
    ELOO_FORCE_INLINE constexpr float64_t fast_ln(float64_t v) {
        return fast_log2(v) * f64::LN2;
    }
    ELOO_FORCE_INLINE constexpr float32_t fast_ln(float32_t v) {
        return fast_log2(v) * f32::LN2;
    }
    ELOO_FORCE_INLINE constexpr float16_t fast_ln(float16_t v) {
        return fast_log2(v) * f16::LN2;
    }

    // Provides ~99.99% accuracy, slightly slower than fast_ln
    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T fast_ln_poly(T v) {
        constexpr float64_t _1_3rd = 0.33333333333333333;
        constexpr float64_t _1_5th = 0.2;
        constexpr float64_t _1_7th = 0.14285714285714285;
        const float64_t y = (static_cast<float64_t>(v) - 1.0) / (static_cast<float64_t>(v) + 1.0);
        const float64_t y2 = y * y;
        const float64_t r = 2.0 * y * (1.0 + y2 * (_1_3rd + y2 * (_1_5th + y2 * _1_7th)));
        if constexpr (::eastl::is_same<T, float64_t>::value) {
            return r;
        } else {
            return static_cast<T>(r);
        }
    }

    template <is_floating_t T1, is_floating_t T2, typename TOut = get_shared_common_t<T1, T2>>
    ELOO_FORCE_INLINE constexpr TOut fast_log(T1 v, T2 base) {
        return static_cast<TOut>(fast_log2(v) / fast_log2(base));
    }

    // Provides complete accuracy, slow to calculate
    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr T log(T v) {
        return std::log(v);
    }


    /////////////////////////////////////////////////////////
    // Exponentials

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T fast_exp2_int(int v) {
        using uintX_t = uint_equiv_t<T>;
        uintX_t bits = static_cast<uintX_t>((v + float_exponent_bits<T>) << float_mantissa_bits<T>);
        if constexpr (eastl::is_same_v<T, float16_t>) {
            return float16_t::float16_to_32(bits);
        } else {
            return std::bit_cast<T>(bits);
        }
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T fast_exp2(T v) {
        constexpr float64_t c1 = 0.69314718;
        constexpr float64_t c2 = 0.24022651;
        constexpr float64_t c3 = 0.05550411;
        const int i = static_cast<int>(v);
        const T d = v - i;
        return fast_exp2_int<T>(i) * static_cast<T>(1.0 + d * (c1 + d * (c2 + d * c3)));
    }

    ELOO_FORCE_INLINE constexpr float64_t fast_exp(float64_t v) {
        return fast_exp2(v * f64::LOG2E);
    }
    ELOO_FORCE_INLINE constexpr float32_t fast_exp(float32_t v) {
        return fast_exp2(v * f32::LOG2E);
    }
    ELOO_FORCE_INLINE constexpr float16_t fast_exp(float16_t v) {
        return fast_exp2(v * f16::LOG2E);
    }

    template <is_floating_t T1, is_floating_t T2, typename TOut = get_shared_common_t<T1, T2>>
    ELOO_FORCE_INLINE constexpr TOut fast_exp(T1 v, T2 base) {
        return fast_exp2<TOut>(v * fast_log2(base));
    }

    ELOO_FORCE_INLINE constexpr float64_t fast_exp10(float64_t v) {
        return fast_exp(v * f64::LOG10E);
    }
    ELOO_FORCE_INLINE constexpr float32_t fast_exp10(float32_t v) {
        return fast_exp(v * f32::LOG10E);
    }
    ELOO_FORCE_INLINE constexpr float16_t fast_exp10(float16_t v) {
        return fast_exp(v * f16::LOG10E);
    }

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr T sqr(T v) {
        return v * v;
    }

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr T cube(T v) {
        return v * v * v;
    }

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr T quad(T v) {
        return v * v * v * v;
    }

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr T quint(T v) {
        return v * v * v * v * v;
    }

    template <is_numeric_t T1, is_numeric_t T2, typename TOut = get_shared_common_t<T1, T2>>
    ELOO_FORCE_INLINE constexpr TOut pow(T1 v, T2 p) {
        return static_cast<TOut>(std::pow(static_cast<long double>(v),
                                          static_cast<long double>(p)));
    }


    /////////////////////////////////////////////////////////
    // Square roots

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T sqrt(T v) {
        if (is_close_to_zero(v)) {
            return static_cast<T>(0.0);
        }
        return static_cast<T>(std::sqrt(static_cast<float64_t>(v)));
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T inv_sqrt(T v) {
        if (is_close_to_zero(v)) {
            return static_cast<T>(0.0);
        }
        return static_cast<T>(1.0 / std::sqrt(static_cast<float64_t>(v)));
    }

    ELOO_FORCE_INLINE constexpr float64_t fast_inv_sqrt(float64_t v) {
        const int64_t i = 0x5FE6EB50C7B537A9 - (std::bit_cast<int64_t>(v) >> 1);
        const float64_t y = std::bit_cast<float64_t>(i);
        return y * (1.5 - (v * 0.5 * y * y));
    }

    ELOO_FORCE_INLINE constexpr float32_t fast_inv_sqrt(float32_t v) {
        const int32_t i = 0x5F3759DF - (std::bit_cast<int32_t>(v) >> 1);
        const float32_t y = std::bit_cast<float32_t>(i);
        return y * (1.5f - (v * 0.5f * y * y));
    }

    ELOO_FORCE_INLINE constexpr float16_t fast_inv_sqrt(float16_t v) {
        const uint16_t i = 0x5F30 - (std::bit_cast<uint16_t>(v) >> 1);
        const float32_t y = float16_t::float16_to_32(i);
        return y * (1.5_h - (v * 0.5_h * y * y));
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T fast_sqrt(T v, uint16_t iterations = 1) {
        const float64_t v64 = static_cast<float64_t>(v);
        if (is_close_to_zero(v64) || iterations == 0) {
            return eastl::numeric_limits<T>::quiet_NaN();
        }

        float64_t guess = v64 * 0.5;
        do {
            guess = (guess + v64 / guess) * 0.5;
        } while (--iterations > 0);
        return static_cast<T>(guess);
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T fast_rsqrt(T v, uint16_t iterations = 1) {
        const float64_t v64 = static_cast<float64_t>(v);
        if (is_close_to_zero(v64) || iterations == 0) {
            return eastl::numeric_limits<T>::quiet_NaN();
        }
        return static_cast<T>(1.0 / fast_sqrt(v, iterations));
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T rsqrt(T v) {
        const float64_t v64 = static_cast<float64_t>(v);
        if (is_close_to_zero(v64)) {
            return eastl::numeric_limits<T>::quiet_NaN();
        }
        return static_cast<T>(1.0 / std::sqrt(v64));
    }


    /////////////////////////////////////////////////////////
    // Clamping

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr T clamp(T v, T m, T M) {
        return v < m ? m : v > M ? M : v;
    }

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr T saturate(T v) {
        return clamp(v, static_cast<T>(0), static_cast<T>(1));
    }


    /////////////////////////////////////////////////////////
    // Rounding

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T round(T v) {
        return std::round(v);
    }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(round);

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T floor(T v) {
        return std::floor(v);
    }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(floor);

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T ceil(T v) {
        return std::ceil(v);
    }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(ceil);

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T frac(T v) {
        return v - floor(v);
    }


    /////////////////////////////////////////////////////////
    // Interpolation

    template <is_floating_t T1, is_floating_t T2, typename TOut = get_shared_common_t<T1, T2>>
    ELOO_FORCE_INLINE constexpr TOut lerp(T1 from, T2 to, T2 t) {
        return static_cast<TOut>(from + (to - from) * t);
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T remap(T v, T om, T oM, T nm, T nM) {
        return nm + (v - om) * (nM - nm) / (oM - om);
    }


    /////////////////////////////////////////////////////////
    // Trigonometry

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr T deg_to_rad(T v) {
        return static_cast<T>(static_cast<float64_t>(v) * f64::DEG_TO_RAD);
    }

    template <is_numeric_t T>
    ELOO_FORCE_INLINE constexpr T rad_to_deg(T v) {
        return static_cast<T>(static_cast<float64_t>(v) * f64::RAD_TO_DEG);
    }

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T sin(T rads) { return std::sin(rads); }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(sin);

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T cos(T rads) { return std::cos(rads); }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(cos);

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T tan(T rads) { return std::tan(rads); }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(tan);

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T asin(T rads) { return std::asin(rads); }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(asin);

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T acos(T rads) { return std::acos(rads); }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(acos);

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T atan(T rads) { return std::atan(rads); }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16(atan);

    template <is_floating_t T>
    ELOO_FORCE_INLINE constexpr T atan2(T y, T x) { return std::atan2(y, x); }
    DECLARE_CONSTEXPR_FUNC_FOR_FLOAT16_2ARGS(atan2);


    /////////////////////////////////////////////////////////
    // Modulo

    template <is_numeric_t T1, is_numeric_t T2, typename TOut = get_shared_common_t<T1, T2>>
    ELOO_FORCE_INLINE constexpr TOut mod(T1 v, T2 d) {
        if constexpr (::eastl::is_integral<TOut>::value) {
            // Regular integral modulo
            return d == 0 ? 0 : static_cast<TOut>(v % d);
        } else {
            // Floating modulo
            using uintX_t = uint_equiv_t<TOut>;

            const uintX_t bits = std::bit_cast<uintX_t>(v);
            const uintX_t expo_bits =
                ::eastl::is_same_v<TOut, float64_t> ? 0x7FF :
                ::eastl::is_same_v<TOut, float32_t> ? 0xFF :
                                                      0x1F;                       // float16
            const uintX_t expo =
                ((bits >> float_mantissa_bits<TOut>) & expo_bits) - float_exponent_bits<TOut>;

            if (expo <= 0) {
                return static_cast<TOut>(0);
            }
            TOut wholePart = std::bit_cast<TOut>(bits & ~((1LL << (float_mantissa_bits<TOut> -expo)) - 1));
            return static_cast<TOut>(v) - wholePart * d;
        }
    }


    // ==================================================================
    // ==================================================================
    //
    // Vector math
    //
    // ==================================================================
    // ==================================================================

    namespace vector {

        /////////////////////////////////////////////////////////////////////
        // Magnitude Squared

        ELOO_FORCE_INLINE constexpr float magnitude_sqr(float x, float y) {
            return x * x + y * y;
        }

        ELOO_FORCE_INLINE float magnitude_sqr(const float2::values& xy) {
            return magnitude_sqr(xy.x(), xy.y());
        }

        ELOO_FORCE_INLINE constexpr float magnitude_sqr(float x, float y, float z) {
            return x * x + y * y + z * z;
        }

        ELOO_FORCE_INLINE float magnitude_sqr(const float3::values& xyz) {
            return magnitude_sqr(xyz.x(), xyz.y(), xyz.z());
        }

        ELOO_FORCE_INLINE constexpr float magnitude_sqr(float x, float y, float z, float w) {
            return x * x + y * y + z * z + w * w;
        }

        ELOO_FORCE_INLINE float magnitude_sqr(const float4::values& xyzw) {
            return magnitude_sqr(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w());
        }



        /////////////////////////////////////////////////////////////////////
        // Magnitude

        ELOO_FORCE_INLINE float magnitude(float x, float y) {
            return sqrtf(magnitude_sqr(x, y));
        }

        ELOO_FORCE_INLINE float magnitude(const float2::values& xy) {
            return sqrtf(magnitude_sqr(xy.x(), xy.y()));
        }

        ELOO_FORCE_INLINE float magnitude(float x, float y, float z) {
            return sqrtf(magnitude_sqr(x, y, z));
        }

        ELOO_FORCE_INLINE float magnitude(const float3::values& xyz) {
            return sqrtf(magnitude_sqr(xyz.x(), xyz.y(), xyz.z()));
        }

        ELOO_FORCE_INLINE float magnitude(float x, float y, float z, float w) {
            return sqrtf(magnitude_sqr(x, y, z, w));
        }

        ELOO_FORCE_INLINE float magnitude(const float4::values& xyzw) {
            return sqrtf(magnitude_sqr(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w()));
        }


        /////////////////////////////////////////////////////////////////////
        // Normalization check

        ELOO_FORCE_INLINE constexpr bool is_normalized(float x, float y) {
            return is_close_to(magnitude_sqr(x, y), 1.0f);
        }

        ELOO_FORCE_INLINE bool is_normalized(const float2::values& xy) {
            return is_normalized(xy.x(), xy.y());
        }

        ELOO_FORCE_INLINE constexpr bool is_normalized(float x, float y, float z) {
            return is_close_to(magnitude_sqr(x, y, z), 1.0f);
        }

        ELOO_FORCE_INLINE bool is_normalized(const float3::values& xyz) {
            return is_normalized(xyz.x(), xyz.y(), xyz.z());
        }

        ELOO_FORCE_INLINE constexpr bool is_normalized(float x, float y, float z, float w) {
            return is_close_to(magnitude_sqr(x, y, z, w), 1.0f);
        }

        ELOO_FORCE_INLINE bool is_normalized(const float4::values& xyzw) {
            return is_normalized(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Magnitude (fast)

        ELOO_FORCE_INLINE constexpr float magnitude_fast(float x, float y, unsigned int iterationCount = 2) {
            if (vector::is_normalized(x, y)) {
                return 1.0f;
            }
            const float magSqr = magnitude_sqr(x, y);
            if (is_close_to_zero(magSqr)) {
                return 0.0f;
            }
            return 1.0f / fast_rsqrt(magSqr, iterationCount);
        }

        ELOO_FORCE_INLINE float magnitude_fast(const float2::values& xy, unsigned int iterationCount = 2) {
            return magnitude_fast(xy.x(), xy.y(), iterationCount);
        }

        ELOO_FORCE_INLINE constexpr float magnitude_fast(float x, float y, float z, unsigned int iterationCount = 2) {
            if (vector::is_normalized(x, y, z)) {
                return 1.0f;
            }
            const float magSqr = magnitude_sqr(x, y, z);
            if (is_close_to_zero(magSqr)) {
                return 0.0f;
            }
            return 1.0f / fast_rsqrt(magSqr, iterationCount);
        }

        ELOO_FORCE_INLINE float magnitude_fast(const float3::values& xyz, unsigned int iterationCount = 2) {
            return magnitude_fast(xyz.x(), xyz.y(), xyz.z(), iterationCount);
        }

        ELOO_FORCE_INLINE constexpr float magnitude_fast(float x, float y, float z, float w, unsigned int iterationCount = 2) {
            if (vector::is_normalized(x, y, z, w)) {
                return 1.0f;
            }
            const float magSqr = magnitude_sqr(x, y, z, w);
            if (is_close_to_zero(magSqr)) {
                return 0.0f;
            }
            return 1.0f / fast_rsqrt(magSqr, iterationCount);
        }

        ELOO_FORCE_INLINE float magnitude_fast(const float4::values& xyzw, unsigned int iterationCount = 2) {
            return magnitude_fast(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w(), iterationCount);
        }


        /////////////////////////////////////////////////////////////////////
        // Normalization

        ELOO_FORCE_INLINE float2::values normalize(float x, float y) {
            const float mag = magnitude(x, y);
            return { x / mag, y / mag };
        }

        ELOO_FORCE_INLINE float2::values normalize(const float2::values& xy) {
            return normalize(xy.x(), xy.y());
        }

        ELOO_FORCE_INLINE float3::values normalize(float x, float y, float z) {
            const float mag = magnitude(x, y, z);
            return { x / mag, y / mag, z / mag };
        }

        ELOO_FORCE_INLINE float3::values normalize(const float3::values& xyz) {
            return normalize(xyz.x(), xyz.y(), xyz.z());
        }

        ELOO_FORCE_INLINE float4::values normalize(float x, float y, float z, float w) {
            const float mag = magnitude(x, y, z, w);
            return { x / mag, y / mag, z / mag, w / mag };
        }

        ELOO_FORCE_INLINE float4::values normalize(const float4::values& xyzw) {
            return normalize(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Normalization (fast)

        ELOO_FORCE_INLINE constexpr float2::values normalize_fast(float x, float y, unsigned int iterationCount = 2) {
            if (is_normalized(x, y)) {
                return { x, y };
            }
            const float magSqr = magnitude_sqr(x, y);
            if (is_close_to(x, y)) {
                return { x * f32::RSQRT2, y * f32::RSQRT2 };
            }
            const float mag = fast_rsqrt(magSqr, iterationCount);
            return { x * mag, y * mag };
        }

        ELOO_FORCE_INLINE float2::values normalize_fast(const float2::values& xy, unsigned int iterationCount = 2) {
            return normalize_fast(xy.x(), xy.y(), iterationCount);
        }

        ELOO_FORCE_INLINE constexpr float3::values normalize_fast(float x, float y, float z, unsigned int iterationCount = 2) {
            if (is_normalized(x, y, z)) {
                return { x, y, z };
            }
            const float magSqr = magnitude_sqr(x, y, z);
            const float mag = fast_rsqrt(magSqr, iterationCount);
            return { x * mag, y * mag, z * mag };
        }

        ELOO_FORCE_INLINE float3::values normalize_fast(const float3::values& xyz, unsigned int iterationCount = 2) {
            return normalize_fast(xyz.x(), xyz.y(), xyz.z(), iterationCount);
        }

        ELOO_FORCE_INLINE constexpr float4::values normalize_fast(float x, float y, float z, float w, unsigned int iterationCount = 2) {
            if (is_normalized(x, y, z, w)) {
                return { x, y, z, w };
            }
            const float magSqr = magnitude_sqr(x, y, z, w);
            const float mag = fast_rsqrt(magSqr, iterationCount);
            return { x * mag, y * mag, z * mag, w * mag };
        }

        ELOO_FORCE_INLINE float4::values normalize_fast(const float4::values& xyzw, unsigned int iterationCount = 2) {
            return normalize_fast(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w(), iterationCount);
        }


        /////////////////////////////////////////////////////////////////////
        // Dot product

        ELOO_FORCE_INLINE constexpr float dot(float x1, float y1, float x2, float y2) {
            return x1 * x2 + y1 * y2;
        }

        ELOO_FORCE_INLINE float dot(const float2::values& xy1, const float2::values& xy2) {
            return dot(xy1.x(), xy1.y(), xy2.x(), xy2.y());
        }

        ELOO_FORCE_INLINE constexpr float dot(float x1, float y1, float z1, float x2, float y2, float z2) {
            return x1 * x2 + y1 * y2 + z1 * z2;
        }

        ELOO_FORCE_INLINE float dot(const float3::values& xyz1, const float3::values& xyz2) {
            return dot(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
        }

        ELOO_FORCE_INLINE constexpr float dot(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) {
            return x1 * x2 + y1 * y2 + z1 * z2 + w1 * w2;
        }

        ELOO_FORCE_INLINE float dot(const float4::values& xyzw1, const float4::values& xyzw2) {
            return dot(xyzw1.x(), xyzw1.y(), xyzw1.z(), xyzw1.w(), xyzw2.x(), xyzw2.y(), xyzw2.z(), xyzw2.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Distance between two points (squared)

        ELOO_FORCE_INLINE constexpr float distance_sqr(float x1, float y1, float x2, float y2) {
            return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
        }

        ELOO_FORCE_INLINE float distance_sqr(const float2::values& xy1, const float2::values& xy2) {
            return distance_sqr(xy1.x(), xy1.y(), xy2.x(), xy2.y());
        }

        ELOO_FORCE_INLINE constexpr float distance_sqr(float x1, float y1, float z1, float x2, float y2, float z2) {
            return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
        }

        ELOO_FORCE_INLINE float distance_sqr(const float3::values& xyz1, const float3::values& xyz2) {
            return distance_sqr(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
        }

        ELOO_FORCE_INLINE constexpr float distance_sqr(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) {
            return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) + (w1 - w2) * (w1 - w2);
        }

        ELOO_FORCE_INLINE float distance_sqr(const float4::values& xyzw1, const float4::values& xyzw2) {
            return distance_sqr(xyzw1.x(), xyzw1.y(), xyzw1.z(), xyzw1.w(), xyzw2.x(), xyzw2.y(), xyzw2.z(), xyzw2.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Distance between two points

        ELOO_FORCE_INLINE float distance(float x1, float y1, float x2, float y2) {
            return sqrtf(distance_sqr(x1, y1, x2, y2));
        }

        ELOO_FORCE_INLINE float distance(const float2::values& xy1, const float2::values& xy2) {
            return distance(xy1.x(), xy1.y(), xy2.x(), xy2.y());
        }

        ELOO_FORCE_INLINE float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
            return sqrtf(distance_sqr(x1, y1, z1, x2, y2, z2));
        }

        ELOO_FORCE_INLINE float distance(const float3::values& xyz1, const float3::values& xyz2) {
            return distance(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
        }

        ELOO_FORCE_INLINE float distance(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) {
            return sqrtf(distance_sqr(x1, y1, z1, w1, x2, y2, z2, w2));
        }

        ELOO_FORCE_INLINE float distance(const float4::values& xyzw1, const float4::values& xyzw2) {
            return distance(xyzw1.x(), xyzw1.y(), xyzw1.z(), xyzw1.w(), xyzw2.x(), xyzw2.y(), xyzw2.z(), xyzw2.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Perpendicular (2D only)

        ELOO_FORCE_INLINE constexpr float2::values perpendicular(float x, float y, bool clockwise = true) {
            return clockwise
                ? float2::values{ y, -x }
            : float2::values{ -y, x };
        }

        ELOO_FORCE_INLINE float2::values perpendicular(const float2::values& xy, bool clockwise = true) {
            return perpendicular(xy.x(), xy.y(), clockwise);
        }


        /////////////////////////////////////////////////////////////////////
        // Cross product (2D)

        ELOO_FORCE_INLINE constexpr float cross(float x1, float y1, float x2, float y2) {
            return x1 * y2 - y1 * x2;
        }

        ELOO_FORCE_INLINE float cross(const float2::values& xy1, const float2::values& xy2) {
            return cross(xy1.x(), xy1.y(), xy2.x(), xy2.y());
        }


        /////////////////////////////////////////////////////////////////////
        // Cross product (3D)

        ELOO_FORCE_INLINE constexpr float3::values cross(float x1, float y1, float z1, float x2, float y2, float z2) {
            return {
                y1 * z2 - z1 * y2,
                z1 * x2 - x1 * z2,
                x1 * y2 - y1 * x2
            };
        }

        ELOO_FORCE_INLINE float3::values cross(const float3::values& xyz1, const float3::values& xyz2) {
            return cross(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Angle between two vectors (2D)

        ELOO_FORCE_INLINE constexpr float angle(float x1, float y1, float x2, float y2) {
            const float combinedMagSqr = magnitude_sqr(x1, y1) * magnitude_sqr(x2, y2);
            if (is_close_to_zero(combinedMagSqr)) {
                return 0.0f;
            }
            const float dotProduct = dot(x1, y1, x2, y2);
            return acosf(dotProduct / sqrt(combinedMagSqr));
        }

        ELOO_FORCE_INLINE float angle(const float2::values& xy1, const float2::values& xy2) {
            return angle(xy1.x(), xy1.y(), xy2.x(), xy2.y());
        }


        /////////////////////////////////////////////////////////////////////
        // Angle between two vectors (3D)

        ELOO_FORCE_INLINE constexpr float angle(float x1, float y1, float z1, float x2, float y2, float z2) {
            const float combinedMagSqr = magnitude_sqr(x1, y1, z1) * magnitude_sqr(x2, y2, z2);
            if (is_close_to_zero(combinedMagSqr)) {
                return 0.0f;
            }
            const float dotProduct = dot(x1, y1, z1, x2, y2, z2);
            return acosf(dotProduct / sqrt(combinedMagSqr));
        }

        ELOO_FORCE_INLINE float angle(const float3::values& xyz1, const float3::values& xyz2) {
            return angle(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Signed angle between two vectors (2D)

        ELOO_FORCE_INLINE constexpr float angle_signed(float x1, float y1, float x2, float y2) {
            const float combinedMagSqr = magnitude_sqr(x1, y1) * magnitude_sqr(x2, y2);
            if (is_close_to_zero(combinedMagSqr)) {
                return 0.0f;
            }
            const float dotProduct = dot(x1, y1, x2, y2);
            const float angle = acosf(dotProduct / sqrt(combinedMagSqr));
            return cross(x1, y1, x2, y2) < 0.0f ? -angle : angle;
        }

        ELOO_FORCE_INLINE float angle_signed(const float2::values& xy1, const float2::values& xy2) {
            return angle_signed(xy1.x(), xy1.y(), xy2.x(), xy2.y());
        }


        /////////////////////////////////////////////////////////////////////
        // Signed angle between two vectors (3D)

        ELOO_FORCE_INLINE constexpr float angle_signed(float x1, float y1, float z1, float x2, float y2, float z2, float normalX, float normalY, float normalZ) {
            const float combinedMagSqr = magnitude_sqr(x1, y1, z1) * magnitude_sqr(x2, y2, z2);
            if (is_close_to_zero(combinedMagSqr)) {
                return 0.0f;
            }
            const float dotProduct = dot(x1, y1, z1, x2, y2, z2);
            const float angle = acosf(dotProduct / sqrt(combinedMagSqr));
            const float3::values crossProduct = cross(x1, y1, z1, x2, y2, z2);
            const float3::values normalizedNormal = normalize(normalX, normalY, normalZ);
            return angle * sign(dot(crossProduct, normalizedNormal));
        }

        ELOO_FORCE_INLINE float angle_signed(const float3::values& xyz1, const float3::values& xyz2, const float3::values& normal) {
            return angle_signed(
                xyz1.x(), xyz1.y(), xyz1.z(),
                xyz2.x(), xyz2.y(), xyz2.z(),
                normal.x(), normal.y(), normal.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Reflect a vector off a normal (2D)

        ELOO_FORCE_INLINE constexpr float2::values reflect(float x, float y, float normalX, float normalY) {
            const float dotProductBy2 = 2.0f * dot(x, y, normalX, normalY);
            return {
                x - dotProductBy2 * normalX,
                y - dotProductBy2 * normalY
            };
        }

        ELOO_FORCE_INLINE float2::values reflect(const float2::values& xy1, const float2::values& xy2) {
            return reflect(xy1.x(), xy1.y(), xy2.x(), xy2.y());
        }


        /////////////////////////////////////////////////////////////////////
        // Reflect a vector off a normal (3D)

        ELOO_FORCE_INLINE constexpr float3::values reflect(float x, float y, float z, float normalX, float normalY, float normalZ) {
            const float dotProductBy2 = 2.0f * dot(x, y, z, normalX, normalY, normalZ);
            return {
                x - dotProductBy2 * normalX,
                y - dotProductBy2 * normalY,
                z - dotProductBy2 * normalZ };
        }

        ELOO_FORCE_INLINE float3::values reflect(const float3::values& xyz1, const float3::values& xyz2) {
            return reflect(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Project a vector onto another vector (2D)

        ELOO_FORCE_INLINE constexpr float2::values project(float x1, float y1, float x2, float y2) {
            const float magSqr = magnitude_sqr(x2, y2);
            if (is_close_to_zero(magSqr)) {
                return float2::ZERO;
            }
            const float dotProduct = dot(x1, y1, x2, y2);
            return {
                (dotProduct / magSqr) * x2,
                (dotProduct / magSqr) * y2
            };
        }

        ELOO_FORCE_INLINE float2::values project(const float2::values& xy1, const float2::values& xy2) {
            return project(xy1.x(), xy1.y(), xy2.x(), xy2.y());
        }


        /////////////////////////////////////////////////////////////////////
        // Project a vector onto another vector (3D)

        ELOO_FORCE_INLINE constexpr float3::values project(float x1, float y1, float z1, float x2, float y2, float z2) {
            const float magSqr = magnitude_sqr(x2, y2, z2);
            if (is_close_to_zero(magSqr)) {
                return float3::ZERO;
            }
            const float dotProduct = dot(x1, y1, z1, x2, y2, z2);
            return {
                (dotProduct / magSqr) * x2,
                (dotProduct / magSqr) * y2,
                (dotProduct / magSqr) * z2
            };
        }

        ELOO_FORCE_INLINE float3::values project(const float3::values& xyz1, const float3::values& xyz2) {
            return project(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Rotate a vector (2D)

        ELOO_FORCE_INLINE float2::values rotate(float x, float y, float radians) {
            const float cosTheta = cosf(radians);
            const float sinTheta = sinf(radians);
            return {
                x * cosTheta - y * sinTheta,
                x * sinTheta + y * cosTheta
            };
        }

        ELOO_FORCE_INLINE float2::values rotate(const float2::values& xy, float radians) {
            return rotate(xy.x(), xy.y(), radians);
        }

        ELOO_FORCE_INLINE float2::values rotate_offset(float x, float y, float xOffset, float yOffset, float radians) {
            const float cosTheta = cosf(radians);
            const float sinTheta = sinf(radians);
            return {
                (x - xOffset) * cosTheta - (y - yOffset) * sinTheta + xOffset,
                (x - xOffset) * sinTheta + (y - yOffset) * cosTheta + yOffset
            };
        }

        ELOO_FORCE_INLINE float2::values rotate_offset(const float2::values& xy, const float2::values& offset, float radians) {
            return rotate_offset(xy.x(), xy.y(), offset.x(), offset.y(), radians);
        }


        /////////////////////////////////////////////////////////////////////
        // Rotate a vector (3D)

        ELOO_FORCE_INLINE float3::values rotate(float x, float y, float z, float radians, float axisX, float axisY, float axisZ) {
            const float cosTheta = cosf(radians);
            const float sinTheta = sinf(radians);
            const float oneMinusCos = 1.0f - cosTheta;

            if (!is_normalized(axisX, axisY, axisZ)) {
                const float3::values normalizedAxis = normalize(axisX, axisY, axisZ);
                axisX = normalizedAxis.x();
                axisY = normalizedAxis.y();
                axisZ = normalizedAxis.z();
            }

            return {
                x * (cosTheta + axisX * axisX * oneMinusCos) +
                y * (axisX * axisY * oneMinusCos - axisZ * sinTheta) +
                z * (axisX * axisZ * oneMinusCos + axisY * sinTheta),

                x * (axisY * axisX * oneMinusCos + axisZ * sinTheta) +
                y * (cosTheta + axisY * axisY * oneMinusCos) +
                z * (axisY * axisZ * oneMinusCos - axisX * sinTheta),

                x * (axisZ * axisX * oneMinusCos - axisY * sinTheta) +
                y * (axisZ * axisY * oneMinusCos + axisX * sinTheta) +
                z * (cosTheta + axisZ * axisZ * oneMinusCos)
            };
        }

        ELOO_FORCE_INLINE float3::values rotate(const float3::values& xyz, float radians, const float2::values& axis) {
            return rotate(
                xyz.x(), xyz.y(), xyz.z(),
                radians,
                axis.x(), axis.y(), 0.0f);
        }


        /////////////////////////////////////////////////////////////////////
        // Project a 4D vector to 3D (perspective divide)

        ELOO_FORCE_INLINE constexpr float3::values project_to_3d(float x, float y, float z, float w) {
            return { x / w, y / w, z / w };
        }

        ELOO_FORCE_INLINE float3::values project_to_3d(const float4::values& xyzw) {
            return project_to_3d(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Project a 3D point to a plane defined by a 4D vector (plane equation: Ax + By + Cz + D = 0)

        ELOO_FORCE_INLINE constexpr float3::values project_onto_plane(float x, float y, float z, float planeA, float planeB, float planeC, float planeD) {
            const float denom = planeA * planeA + planeB * planeB + planeC * planeC;
            if (is_close_to_zero(denom)) {
                return { x, y, z };
            }
            const float t = -(planeA * x + planeB * y + planeC * z + planeD) / denom;
            return {
                x + t * planeA,
                y + t * planeB,
                z + t * planeC
            };
        }

        ELOO_FORCE_INLINE float3::values project_onto_plane(const float3::values& xyz, float planeA, float planeB, float planeC, float planeD) {
            return project_onto_plane(
                xyz.x(), xyz.y(), xyz.z(),
                planeA, planeB, planeC, planeD);
        }

        ELOO_FORCE_INLINE float3::values project_onto_plane(const float3::values& xyz, const float4::values& plane) {
            return project_onto_plane(
                xyz.x(), xyz.y(), xyz.z(),
                plane.x(), plane.y(), plane.z(), plane.w());
        }
    }


    /////////////////////////////////////////////////////////////////////
    // Interpolation between two points (2D)

    namespace interpolation {
        ELOO_FORCE_INLINE float2::values interpolate(float xStart, float yStart, float xEnd, float yEnd, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return {
                interpolate(xStart, xEnd, t, type, options),
                interpolate(yStart, yEnd, t, type, options)
            };
        }

        ELOO_FORCE_INLINE float2::values interpolate(const float2::values& start, const float2::values& end, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return interpolate(start.x(), start.y(), end.x(), end.y(), t, type, options);
        }
    }


    /////////////////////////////////////////////////////////////////////
    // Interpolation between two points (3D)

    namespace interpolation {
        ELOO_FORCE_INLINE float3::values interpolate(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return {
                interpolate(xStart, xEnd, t, type, options),
                interpolate(yStart, yEnd, t, type, options),
                interpolate(zStart, zEnd, t, type, options)
            };
        }

        ELOO_FORCE_INLINE float3::values interpolate(const float3::values& start, const float3::values& end, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return interpolate(start.x(), start.y(), start.z(), end.x(), end.y(), end.z(), t, type, options);
        }
    }


    /////////////////////////////////////////////////////////////////////
    // Interpolation between two points (4D)

    namespace interpolation {
        ELOO_FORCE_INLINE float4::values interpolate(float xStart, float yStart, float zStart, float wStart, float xEnd, float yEnd, float zEnd, float wEnd, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return {
                interpolate(xStart, xEnd, t, type, options),
                interpolate(yStart, yEnd, t, type, options),
                interpolate(zStart, zEnd, t, type, options),
                interpolate(wStart, wEnd, t, type, options)
            };
        }

        ELOO_FORCE_INLINE float4::values interpolate(const float4::values& start, const float4::values& end, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return interpolate(start.x(), start.y(), start.z(), start.w(), end.x(), end.y(), end.z(), end.w(), t, type, options);
        }
    }


    // ==================================================================
    // ==================================================================
    //
    // Matrix math
    //
    // ==================================================================
    // ==================================================================

    namespace matrix {

        /////////////////////////////////////////////////////////////////////
        // Transpose

        ELOO_FORCE_INLINE constexpr matrix2x2::values transpose(MATRIX2X2_DECLARE_PARAMS(v)) {
            return {
                v11, v21,
                v12, v22
            };
        }
        ELOO_FORCE_INLINE matrix2x2::values transpose(const matrix2x2::values& m) {
            return transpose(MATRIX2X2_UNPACK(m));
        }

        ELOO_FORCE_INLINE constexpr matrix3x3::values transpose(MATRIX3X3_DECLARE_PARAMS(v)) {
            return {
                v11, v21, v31,
                v12, v22, v32,
                v13, v23, v33
            };
        }
        ELOO_FORCE_INLINE matrix3x3::values transpose(const matrix3x3::values& m) {
            return transpose(MATRIX3X3_UNPACK(m));
        }

        ELOO_FORCE_INLINE constexpr matrix4x4::values transpose(MATRIX4X4_DECLARE_PARAMS(v)) {
            return {
                v11, v21, v31, v41,
                v12, v22, v32, v42,
                v13, v23, v33, v43,
                v14, v24, v34, v44
            };
        }
        ELOO_FORCE_INLINE matrix4x4::values transpose(const matrix4x4::values& m) {
            return transpose(MATRIX4X4_UNPACK(m));
        }


        /////////////////////////////////////////////////////////////////////
        // Determinant

        ELOO_FORCE_INLINE constexpr float determinant(MATRIX2X2_DECLARE_PARAMS(v)) {
            return v11 * v22 - v12 * v21;
        }
        ELOO_FORCE_INLINE float determinant(const matrix2x2::values& m) {
            return determinant(MATRIX2X2_UNPACK(m));
        }

        ELOO_FORCE_INLINE constexpr float determinant(MATRIX3X3_DECLARE_PARAMS(v)) {
            return
                v11 * (v22 * v33 - v23 * v32) -
                v12 * (v21 * v33 - v23 * v31) +
                v13 * (v21 * v32 - v22 * v31);
        }
        ELOO_FORCE_INLINE float determinant(const matrix3x3::values& m) {
            return determinant(MATRIX3X3_UNPACK(m));
        }

        ELOO_FORCE_INLINE constexpr float determinant(MATRIX4X4_DECLARE_PARAMS(v)) {
            return
                v11 * (v22 * (v33 * v44 - v34 * v43) - v23 * (v32 * v44 - v34 * v42) + v24 * (v32 * v43 - v33 * v42)) -
                v12 * (v21 * (v33 * v44 - v34 * v43) - v23 * (v31 * v44 - v34 * v41) + v24 * (v31 * v43 - v33 * v41)) +
                v13 * (v21 * (v32 * v44 - v34 * v42) - v22 * (v31 * v44 - v34 * v41) + v24 * (v31 * v42 - v32 * v41)) -
                v14 * (v21 * (v32 * v43 - v33 * v42) - v22 * (v31 * v43 - v33 * v41) + v23 * (v31 * v42 - v32 * v41));
        }
        ELOO_FORCE_INLINE float determinant(const matrix4x4::values& m) {
            return determinant(MATRIX4X4_UNPACK(m));
        }


        /////////////////////////////////////////////////////////////////////
        // Cofactor

        ELOO_FORCE_INLINE constexpr matrix2x2::values cofactor(MATRIX2X2_DECLARE_PARAMS(v)) {
            return {
                v22, -v12,
                -v21, v11
            };
        }
        ELOO_FORCE_INLINE matrix2x2::values cofactor(const matrix2x2::values& m) {
            return cofactor(MATRIX2X2_UNPACK(m));
        }

        ELOO_FORCE_INLINE constexpr matrix3x3::values cofactor(MATRIX3X3_DECLARE_PARAMS(v)) {
            return {
                v22 * v33 - v23 * v32, v13 * v32 - v12 * v33, v12 * v23 - v13 * v22,
                v23 * v31 - v21 * v33, v11 * v33 - v13 * v31, v13 * v21 - v11 * v23,
                v21 * v32 - v22 * v31, v12 * v31 - v11 * v32, v11 * v22 - v12 * v21
            };
        }
        ELOO_FORCE_INLINE matrix3x3::values cofactor(const matrix3x3::values& m) {
            return cofactor(MATRIX3X3_UNPACK(m));
        }

        ELOO_FORCE_INLINE constexpr matrix4x4::values cofactor(MATRIX4X4_DECLARE_PARAMS(v)) {
            return {
                  v22 * (v33 * v44 - v34 * v43) - v23 * (v32 * v44 - v34 * v42) + v24 * (v32 * v43 - v33 * v42),
                -(v12 * (v33 * v44 - v34 * v43) - v13 * (v32 * v44 - v34 * v42) + v14 * (v32 * v43 - v33 * v42)),
                 (v12 * (v23 * v44 - v24 * v43) - v13 * (v22 * v44 - v24 * v42) + v14 * (v22 * v43 - v23 * v42)),
                -(v12 * (v23 * v34 - v24 * v33) - v13 * (v22 * v34 - v24 * v32) + v14 * (v22 * v33 - v23 * v32))
            };
        }
        ELOO_FORCE_INLINE matrix4x4::values cofactor(const matrix4x4::values& m) {
            return cofactor(MATRIX4X4_UNPACK(m));
        }


        /////////////////////////////////////////////////////////////////////
        // Adjugate

        ELOO_FORCE_INLINE matrix2x2::values adjugate(MATRIX2X2_DECLARE_PARAMS(v)) {
            return transpose(cofactor(MATRIX2X2_FORWARD_PARAMS(v)));
        }
        ELOO_FORCE_INLINE matrix2x2::values adjugate(const matrix2x2::values& m) {
            return adjugate(MATRIX2X2_UNPACK(m));
        }

        ELOO_FORCE_INLINE matrix3x3::values adjugate(MATRIX3X3_DECLARE_PARAMS(v)) {
            return transpose(cofactor(MATRIX3X3_FORWARD_PARAMS(v)));
        }
        ELOO_FORCE_INLINE matrix3x3::values adjugate(const matrix3x3::values& m) {
            return adjugate(MATRIX3X3_UNPACK(m));
        }

        ELOO_FORCE_INLINE matrix4x4::values adjugate(MATRIX4X4_DECLARE_PARAMS(v)) {
            return transpose(cofactor(MATRIX4X4_FORWARD_PARAMS(v)));
        }
        ELOO_FORCE_INLINE matrix4x4::values adjugate(const matrix4x4::values& m) {
            return adjugate(MATRIX4X4_UNPACK(m));
        }


        /////////////////////////////////////////////////////////////////////
        // Inverse

        ELOO_FORCE_INLINE constexpr matrix2x2::values inverse(MATRIX2X2_DECLARE_PARAMS(v)) {
            const float det = determinant(MATRIX2X2_FORWARD_PARAMS(v));
            if (is_close_to_zero(det)) {
                return matrix2x2::ZERO;
            }
            return adjugate(MATRIX2X2_FORWARD_PARAMS(v)) * (1.0f / det);
        }
        ELOO_FORCE_INLINE matrix2x2::values inverse(const matrix2x2::values& m) {
            return inverse(MATRIX2X2_UNPACK(m));
        }

        ELOO_FORCE_INLINE constexpr matrix3x3::values inverse(MATRIX3X3_DECLARE_PARAMS(v)) {
            const float det = determinant(MATRIX3X3_FORWARD_PARAMS(v));
            if (is_close_to_zero(det)) {
                return matrix3x3::ZERO;
            }
            return adjugate(MATRIX3X3_FORWARD_PARAMS(v)) * (1.0f / det);
        }
        ELOO_FORCE_INLINE matrix3x3::values inverse(const matrix3x3::values& m) {
            return inverse(MATRIX3X3_UNPACK(m));
        }

        ELOO_FORCE_INLINE constexpr matrix4x4::values inverse(MATRIX4X4_DECLARE_PARAMS(v)) {
            const float det = determinant(MATRIX4X4_FORWARD_PARAMS(v));
            if (is_close_to_zero(det)) {
                return matrix4x4::ZERO;
            }
            return adjugate(MATRIX4X4_FORWARD_PARAMS(v)) * (1.0f / det);
        }
        ELOO_FORCE_INLINE matrix4x4::values inverse(const matrix4x4::values& m) {
            return inverse(MATRIX4X4_UNPACK(m));
        }


        /////////////////////////////////////////////////////////////////////
        // Translation

        ELOO_FORCE_INLINE constexpr matrix4x4::values create_translation(float x, float y, float z) {
            return {
                1.0f, 0.0f, 0.0f, x,
                0.0f, 1.0f, 0.0f, y,
                0.0f, 0.0f, 1.0f, z,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }
        ELOO_FORCE_INLINE matrix4x4::values create_translation(const float3::values& xyz) {
            return create_translation(xyz.x(), xyz.y(), xyz.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Rotation

        ELOO_FORCE_INLINE matrix4x4::values create_rotation_x(float radians) {
            const float cosTheta = cos(radians);
            const float sinTheta = sin(radians);
            return {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cosTheta, -sinTheta, 0.0f,
                0.0f, sinTheta, cosTheta, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }

        ELOO_FORCE_INLINE matrix4x4::values create_rotation_y(float radians) {
            const float cosTheta = cos(radians);
            const float sinTheta = sin(radians);
            return {
                cosTheta, 0.0f, sinTheta, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                -sinTheta, 0.0f, cosTheta, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }

        ELOO_FORCE_INLINE matrix4x4::values create_rotation_z(float radians) {
            const float cosTheta = cos(radians);
            const float sinTheta = sin(radians);
            return {
                cosTheta, -sinTheta, 0.0f, 0.0f,
                sinTheta, cosTheta, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }

        ELOO_FORCE_INLINE matrix4x4::values create_rotation(float x, float y, float z) {
            return create_rotation_z(z) * create_rotation_y(y) * create_rotation_x(x);
        }

        ELOO_FORCE_INLINE matrix4x4::values create_rotation(float radians, float axisX, float axisY, float axisZ) {
            const float cosTheta = cos(radians);
            const float sinTheta = sin(radians);
            const float oneMinusCos = 1.0f - cosTheta;
            if (!math::vector::is_normalized(axisX, axisY, axisZ)) {
                const float3::values normalizedAxis = math::vector::normalize(axisX, axisY, axisZ);
                axisX = normalizedAxis.x();
                axisY = normalizedAxis.y();
                axisZ = normalizedAxis.z();
            }
            return {
                // row 1
                cosTheta + axisX * axisX * oneMinusCos,         // column 1
                axisX * axisY * oneMinusCos - axisZ * sinTheta, // column 2
                axisX * axisZ * oneMinusCos + axisY * sinTheta, // column 3
                0.0f,                                           // column 4

                // row 2
                axisY * axisX * oneMinusCos + axisZ * sinTheta, // column 1
                cosTheta + axisY * axisY * oneMinusCos,         // column 2
                axisY * axisZ * oneMinusCos - axisX * sinTheta, // column 3
                0.0f,                                           // column 4

                // row 3
                axisZ * axisX * oneMinusCos - axisY * sinTheta, // column 1
                axisZ * axisY * oneMinusCos + axisX * sinTheta, // column 2
                cosTheta + axisZ * axisZ * oneMinusCos,         // column 3
                0.0f,                                           // column 4

                // row 4
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }
        ELOO_FORCE_INLINE matrix4x4::values create_rotation(float radians, const float3::values& axis = float3::UP) {
            return create_rotation(radians, axis.x(), axis.y(), axis.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Scale

        ELOO_FORCE_INLINE constexpr matrix4x4::values create_scale(float x, float y, float z) {
            return {
                x, 0.0f, 0.0f, 0.0f,
                0.0f, y, 0.0f, 0.0f,
                0.0f, 0.0f, z, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }
        ELOO_FORCE_INLINE matrix4x4::values create_scale(const float3::values& xyz) {
            return create_scale(xyz.x(), xyz.y(), xyz.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Shear

        ELOO_FORCE_INLINE constexpr matrix4x4::values create_shear(float x, float y, float z) {
            return {
                1.0f, y, z, 0.0f,
                x, 1.0f, z, 0.0f,
                x, y, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }
        ELOO_FORCE_INLINE matrix4x4::values create_shear(const float3::values& xyz) {
            return create_shear(xyz.x(), xyz.y(), xyz.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Transformations

        ELOO_FORCE_INLINE matrix4x4::values create_transform(const float3::values& translation, float rotation, const float3::values& scale) {
            return create_translation(translation) * create_rotation(rotation) * create_scale(scale);
        }

        ELOO_FORCE_INLINE matrix4x4::values create_transform(const float3::values& translation, const float3::values& rotation, const float3::values& scale) {
            return create_translation(translation) * create_rotation(rotation.x(), rotation.y(), rotation.z()) * create_scale(scale);
        }

        ELOO_FORCE_INLINE void decompose_transform(const matrix4x4::values& m, float3::values& translation, float3::values& rotation, float3::values& scale) {
            translation = { m[matrix4x4::R1C4], m[matrix4x4::R2C4], m[matrix4x4::R3C4] };
            // Extract scale
            scale = {
                sqrt(m[matrix4x4::R1C1] * m[matrix4x4::R1C1] + m[matrix4x4::R1C2] * m[matrix4x4::R1C2] + m[matrix4x4::R1C3] * m[matrix4x4::R1C3]),
                sqrt(m[matrix4x4::R2C1] * m[matrix4x4::R2C1] + m[matrix4x4::R2C2] * m[matrix4x4::R2C2] + m[matrix4x4::R2C3] * m[matrix4x4::R2C3]),
                sqrt(m[matrix4x4::R3C1] * m[matrix4x4::R3C1] + m[matrix4x4::R3C2] * m[matrix4x4::R3C2] + m[matrix4x4::R3C3] * m[matrix4x4::R3C3])
            };
            // Normalize the matrix
            const matrix4x4::values normalizedMatrix = {
                m[matrix4x4::R1C1] / scale.x(), m[matrix4x4::R1C2] / scale.x(), m[matrix4x4::R1C3] / scale.x(), 0.0f,
                m[matrix4x4::R2C1] / scale.y(), m[matrix4x4::R2C2] / scale.y(), m[matrix4x4::R2C3] / scale.y(), 0.0f,
                m[matrix4x4::R3C1] / scale.z(), m[matrix4x4::R3C2] / scale.z(), m[matrix4x4::R3C3] / scale.z(), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
            // Extract rotation
            rotation = {
                atan2(normalizedMatrix[matrix4x4::R3C2], normalizedMatrix[matrix4x4::R3C3]),
                atan2(-normalizedMatrix[matrix4x4::R3C1], sqrt(normalizedMatrix[matrix4x4::R3C2] * normalizedMatrix[matrix4x4::R3C2] + normalizedMatrix[matrix4x4::R3C3] * normalizedMatrix[matrix4x4::R3C3])),
                atan2(normalizedMatrix[matrix4x4::R2C1], normalizedMatrix[matrix4x4::R1C1])
            };
        }


        /////////////////////////////////////////////////////////////////////
        // Projection matrices

        ELOO_FORCE_INLINE matrix4x4::values create_look_at(const float3::values& eye, const float3::values& target, const float3::values& up) {
            const float3::values f = math::vector::normalize(target - eye);                 // Forward
            const float3::values r = math::vector::normalize(math::vector::cross(f, up));   // Right
            const float3::values u = math::vector::cross(r, f);                             // Up
            return {
                r.x(),                       u.x(),                       -f.x(),                     0.0f,
                r.y(),                       u.y(),                       -f.y(),                     0.0f,
                r.z(),                       u.z(),                       -f.z(),                     0.0f,
                -math::vector::dot(r, eye),  -math::vector::dot(u, eye),  math::vector::dot(f, eye),  1.0f
            };
        }

        ELOO_FORCE_INLINE matrix4x4::values create_perspective(float fovYRadians, float aspectRatio, float nearZ, float farZ) {
            const float f = tan(fovYRadians * 0.5f);
            const float v11 = 1.0f / (aspectRatio * f);
            const float v22 = 1.0f / f;
            const float v33 = (farZ + nearZ) / (farZ - nearZ);
            const float v34 = (2.0f * farZ * nearZ) / (farZ - nearZ);
            return {
                v11,  0.0f, 0.0f, 0.0f,
                0.0f, v22,  0.0f, 0.0f,
                0.0f, 0.0f, v33,  v34,
                0.0f, 0.0f, -1.0f, 0.0f
            };
        }

        ELOO_FORCE_INLINE matrix4x4::values create_orthographic(float left, float right, float bottom, float top, float nearZ, float farZ) {
            const float v11 = 2.0f / (right - left);
            const float v22 = 2.0f / (top - bottom);
            const float v33 = -2.0f / (farZ - nearZ);
            const float v14 = -(right + left) / (right - left);
            const float v24 = -(top + bottom) / (top - bottom);
            const float v34 = -(farZ + nearZ) / (farZ - nearZ);
            return {
                v11,  0.0f, 0.0f, v14,
                0.0f, v22,  0.0f, v24,
                0.0f, 0.0f, v33,  v34,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }
    }


    // ==================================================================
    // ==================================================================
    //
    // Quaternion math
    //
    // ==================================================================
    // ==================================================================

    namespace quaternion {

        /////////////////////////////////////////////////////////////////////
        // Create a quaternion from euler angles

        ELOO_FORCE_INLINE constexpr eloo::quaternion::values create_from_euler(float x, float y, float z) {
            const float halfX = x * 0.5f;
            const float halfY = y * 0.5f;
            const float halfZ = z * 0.5f;
            const float cosX = cos(halfX);
            const float sinX = sin(halfX);
            const float cosY = cos(halfY);
            const float sinY = sin(halfY);
            const float cosZ = cos(halfZ);
            const float sinZ = sin(halfZ);
            return {
                cosY * sinX * cosZ + sinY * cosX * sinZ,
                sinY * cosX * cosZ - cosY * sinX * sinZ,
                cosY * cosX * sinZ - sinY * sinX * cosZ,
                cosY * cosX * cosZ + sinY * sinX * sinZ
            };
        }
        ELOO_FORCE_INLINE eloo::quaternion::values create_from_euler(const float3::values& euler) {
            return create_from_euler(euler.x(), euler.y(), euler.z());
        }


        /////////////////////////////////////////////////////////////////////
        // Get euler angles from a quaternion

        ELOO_FORCE_INLINE constexpr float3::values as_euler(float x, float y, float z, float w) {
            return {
                atan2(2.0f * (y * z + x * w), w * w - x * x - y * y + z * z),
                asin(-2.0f * (x * z - y * w)),
                atan2(2.0f * (x * y + z * w), w * w + x * x - y * y - z * z)
            };
        }
        ELOO_FORCE_INLINE float3::values as_euler(const eloo::quaternion::values& q) {
            return as_euler(q.x(), q.y(), q.z(), q.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Vector helper alias

        ELOO_FORCE_INLINE constexpr bool is_normalized(float x, float y, float z, float w) {
            return vector::is_normalized(x, y, z, w);
        }
        ELOO_FORCE_INLINE bool is_normalized(const eloo::quaternion::values& q) {
            return vector::is_normalized(q.x(), q.y(), q.z(), q.w());
        }

        ELOO_FORCE_INLINE eloo::quaternion::values normalize(float x, float y, float z, float w) {
            return { vector::normalize(x, y, z, w) };
        }
        ELOO_FORCE_INLINE eloo::quaternion::values normalize(const eloo::quaternion::values& q) {
            return { vector::normalize(q.x(), q.y(), q.z(), q.w()) };
        }

        ELOO_FORCE_INLINE eloo::quaternion::values normalize_fast(float x, float y, float z, float w, uint32_t iterationCount = 2) {
            return { vector::normalize_fast(x, y, z, w, iterationCount) };
        }
        ELOO_FORCE_INLINE eloo::quaternion::values normalize_fast(const eloo::quaternion::values& q, uint32_t iterationCount = 2) {
            return { vector::normalize_fast(q.x(), q.y(), q.z(), q.w(), iterationCount) };
        }

        ELOO_FORCE_INLINE constexpr float magnitude_sqr(float x, float y, float z, float w) {
            return vector::magnitude_sqr(x, y, z, w);
        }
        ELOO_FORCE_INLINE float magnitude_sqr(const eloo::quaternion::values& q) {
            return vector::magnitude_sqr(q.x(), q.y(), q.z(), q.w());
        }

        ELOO_FORCE_INLINE float magnitude(float x, float y, float z, float w) {
            return vector::magnitude(x, y, z, w);
        }
        ELOO_FORCE_INLINE float magnitude(const eloo::quaternion::values& q) {
            return vector::magnitude(q.x(), q.y(), q.z(), q.w());
        }

        ELOO_FORCE_INLINE constexpr float magnitude_fast(float x, float y, float z, float w, uint32_t iterationCount = 2) {
            return vector::magnitude_fast(x, y, z, w, iterationCount);
        }
        ELOO_FORCE_INLINE float magnitude_fast(const eloo::quaternion::values& q, uint32_t iterationCount = 2) {
            return vector::magnitude_fast(q.x(), q.y(), q.z(), q.w(), iterationCount);
        }

        ELOO_FORCE_INLINE constexpr float dot(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) {
            return vector::dot(x1, y1, z1, w1, x2, y2, z2, w2);
        }
        ELOO_FORCE_INLINE float dot(const eloo::quaternion::values& q1, const eloo::quaternion::values& q2) {
            return vector::dot(q1.x(), q1.y(), q1.z(), q1.w(), q2.x(), q2.y(), q2.z(), q2.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Conjugate

        ELOO_FORCE_INLINE constexpr eloo::quaternion::values conjugate(float x, float y, float z, float w) {
            return { -x, -y, -z, w };
        }
        ELOO_FORCE_INLINE eloo::quaternion::values conjugate(const eloo::quaternion::values& q) {
            return conjugate(q.x(), q.y(), q.z(), q.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Inverse

        ELOO_FORCE_INLINE constexpr eloo::quaternion::values inverse(float x, float y, float z, float w) {
            const float magSqr = magnitude_sqr(x, y, z, w);
            if (is_close_to_zero(magSqr)) {
                return eloo::quaternion::ZERO;
            }
            return conjugate(x, y, z, w) * (1.0f / magSqr);
        }
        ELOO_FORCE_INLINE eloo::quaternion::values inverse(const eloo::quaternion::values& q) {
            return inverse(q.x(), q.y(), q.z(), q.w());
        }


        /////////////////////////////////////////////////////////////////////
        // Extract the roll, pitch, and yaw

        ELOO_FORCE_INLINE constexpr float roll(float x, float y, float z, float w) {
            return atan2(2.0f * (y * z + x * w), w * w - x * x - y * y + z * z);
        }
        ELOO_FORCE_INLINE float roll(const eloo::quaternion::values& q) {
            return roll(q.x(), q.y(), q.z(), q.w());
        }

        ELOO_FORCE_INLINE constexpr float pitch(float x, float y, float z, float w) {
            return asin(-2.0f * (x * z - y * w));
        }
        ELOO_FORCE_INLINE float pitch(const eloo::quaternion::values& q) {
            return pitch(q.x(), q.y(), q.z(), q.w());
        }

        ELOO_FORCE_INLINE constexpr float yaw(float x, float y, float z, float w) {
            return atan2(2.0f * (x * y + z * w), w * w + x * x - y * y - z * z);
        }
        ELOO_FORCE_INLINE float yaw(const eloo::quaternion::values& q) {
            return yaw(q.x(), q.y(), q.z(), q.w());
        }
    }

    /////////////////////////////////////////////////////////////////////
    // Slerp

    namespace interpolation {
        ELOO_FORCE_INLINE eloo::quaternion::values slerp(float xStart, float yStart, float zStart, float wStart, float xEnd, float yEnd, float zEnd, float wEnd, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            const float _t = interpolate(0.0f, 1.0f, t, type, options);
            const float cosTheta = quaternion::dot(xStart, yStart, zStart, wStart, xEnd, yEnd, zEnd, wEnd);
            if (is_close_to(cosTheta, 1.0f)) {
                return quaternion::normalize(
                    xStart + _t * (xEnd - xStart),
                    yStart + _t * (yEnd - yStart),
                    zStart + _t * (zEnd - zStart),
                    wStart + _t * (wEnd - wStart));
            }
            const float theta = acos(cosTheta);
            const float sinTheta = sin(theta);
            const float a = sin((1.0f - _t) * theta) / sinTheta;
            const float b = sin(_t * theta) / sinTheta;
            return {
                a * xStart + b * xEnd,
                a * yStart + b * yEnd,
                a * zStart + b * zEnd,
                a * wStart + b * wEnd
            };
        }
        ELOO_FORCE_INLINE eloo::quaternion::values slerp(const eloo::quaternion::values& start, const eloo::quaternion::values& end, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return slerp(start.x(), start.y(), start.z(), start.w(), end.x(), end.y(), end.z(), end.w(), t, type, options);
        }
    }
}