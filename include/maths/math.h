#pragma once

#include <utility/defines.h>
#include <datatype/float2.h>
#include <datatype/float3.h>
#include <datatype/float4.h>
#include <maths/constants.h>
#include <maths/quaternion.h>
#include <maths/matrix2x2.h>
#include <maths/matrix3x3.h>
#include <maths/matrix4x4.h>
#include <maths/interpolation.h>

#include <EASTL/type_traits.h>

#include <cmath>
#include <bit>

namespace eloo::math {

    namespace {
        template <typename T> concept numeric_t = ::eastl::is_arithmetic_v<T>;
        template <typename T> concept float_t = ::eastl::is_floating_point_v<T>;
        template <typename T> concept int_t = ::eastl::is_integral_v<T>;
        template <typename T> concept sint_t = ::eastl::is_integral_v<T> && ::eastl::is_signed_v<T>;
        template <typename T> concept uint_t = ::eastl::is_integral_v<T> && !::eastl::is_signed_v<T>;
        template <numeric_t T1, numeric_t T2> using common_t = ::eastl::common_type_t<T1, T2>;

        template <float_t T>
        using float_to_int_bit_matching =
            ::eastl::conditional_t<::eastl::is_same_v<T, float64_t>, int64_t,
            ::eastl::conditional_t<::eastl::is_same_v<T, float32_t>, int32_t,
            ::eastl::conditional_t<::eastl::is_same_v<T, float16_t>, uint16_t,
            void>>>;

        template <int_t T>
        using int_to_float_bit_matching =
            ::eastl::conditional_t<::eastl::is_same_v<T, int64_t>, float64_t,
            ::eastl::conditional_t<::eastl::is_same_v<T, int32_t>, float32_t,
            ::eastl::conditional_t<::eastl::is_same_v<T, uint16_t>, float16_t,
            void>>>;


        template <float_t T>
        inline constexpr int32_t float_exponent_bias =
            ::eastl::is_same_v<T, float64_t> ? 0x03FF :
            ::eastl::is_same_v<T, float32_t> ? 0x007F :
            ::eastl::is_same_v<T, float16_t> ? 0x000F : 0x0000;
        template <float_t T>
        inline constexpr int32_t float_exponent_shift =
            ::eastl::is_same_v<T, float64_t> ? 0x0034 :
            ::eastl::is_same_v<T, float32_t> ? 0x0017 :
            ::eastl::is_same_v<T, float16_t> ? 0x000A : 0x0000;

#define MAKE_FUNC_FOR_FLOAT16(f) \
        template <> FORCE_INLINE float16_t f(float16_t v) { \
            return float16_t(f(float16_t::float16_to_32(v))); \
        }
#define MAKE_CONSTEXPR_FUNC_FOR_FLOAT16(f) \
        template <> FORCE_INLINE constexpr float16_t f(float16_t v) { \
            return float16_t(f(float16_t::float16_to_32(v))); \
        }

#define MAKE_FUNC_FOR_FLOAT16_2ARGS(f) \
        template <> FORCE_INLINE float16_t f(float16_t x, float16_t y) { \
            return float16_t(f(float16_t::float16_to_32(x), float16_t::float16_to_32(y))); \
        }
#define MAKE_CONSTEXPR_FUNC_FOR_FLOAT16_2ARGS(f) \
        template <> FORCE_INLINE constexpr float16_t f(float16_t x, float16_t y) { \
            return float16_t(f(float16_t::float16_to_32(x), float16_t::float16_to_32(y))); \
        }
    }



    /////////////////////////////////////////////////////////
    // Comparison

    template <float_t T1, numeric_t T2>
    FORCE_INLINE constexpr bool is_close_to(T1 val1, T2 val2) {
        if constexpr (sizeof(T1) == 8) {
            return is_close_to_with_tolerance(val1, val2, f64::CLOSE_REL_TOLERANCE, f64::CLOSE_ABS_TOLERANCE);
        } else if constexpr (sizeof(T1) == 4) {
            return is_close_to_with_tolerance(val1, val2, f32::CLOSE_REL_TOLERANCE, f32::CLOSE_ABS_TOLERANCE);
        } else {
            return is_close_to_with_tolerance(val1, val2, f16::CLOSE_REL_TOLERANCE, f16::CLOSE_ABS_TOLERANCE);
        }
    }

    template <float_t T1, numeric_t T2>
    FORCE_INLINE constexpr bool is_close_to_with_tolerance(T1 val1, T2 val2, T1 relTolerance, T1 absTolerance) {
        T1 diff = abs(val1 - static_cast<T1>(val2));
        return diff <= absTolerance || diff <= relTolerance * max(abs(val1), abs(static_cast<T1>(val2)));
    }

    template <float_t T>
    FORCE_INLINE constexpr bool is_close_to_zero(T val) {
        return is_close_to(val, 0);
    }

    template <float_t T>
    FORCE_INLINE constexpr bool is_close_to_zero_with_tolerance(T val, T relTolerance, T absTolerance) {
        return is_close_to_with_tolerance(val, 0, relTolerance, absTolerance);
    }

    template <numeric_t T1, numeric_t T2, typename TOut = common_t<T1, T2>>
    FORCE_INLINE constexpr TOut min(T1 v1, T2 v2) {
        return static_cast<TOut>(v1 < v2 ? v1 : v2);
    }

    template <numeric_t T1, numeric_t T2, typename TOut = common_t<T1, T2>>
    FORCE_INLINE constexpr TOut max(T1 v1, T2 v2) {
        return static_cast<TOut>(v1 > v2 ? v1 : v2);
    }

    /////////////////////////////////////////////////////////
    // Signs

    template <numeric_t T>
    requires eastl::is_signed_v<T> && !eastl::is_same_v<T, bool>
    FORCE_INLINE constexpr T abs(T v) {
        return ::std::abs(v);
    }

    template <numeric_t T>
    requires eastl::is_unsigned_v<T>
    FORCE_INLINE constexpr T abs(T v) {
        return v;
    }

    template <numeric_t T>
    FORCE_INLINE constexpr int sign(T v) {
        if constexpr (float_t<T>) {
            return std::signbit(v) ? (is_close_to_zero(v) ? 0 : -1) : (v > 0);
        } else {
            return static_cast<int>((v > 0) - (v < 0));
        }
    }


    /////////////////////////////////////////////////////////
    // Logarithm

    // Provides ~99.7% accuracy, significantly faster than log2()
    template <float_t T>
    FORCE_INLINE constexpr T fast_log2(T v) {
        using intX_t = float_to_int_bit_matching<T>;
        if constexpr (::eastl::is_same_v<T, float64_t>) {
            return ::std::bit_cast<intX_t>(v) * 1.4426950408889634e-16 - float_exponent_bias<T>;
        } else if constexpr (::eastl::is_same_v<T, float32_t>) {
            return ::std::bit_cast<intX_t>(v) * 1.1920928955078125e-7f - float_exponent_bias<T>;
        } else {
            return ::std::bit_cast<intX_t>(v) * 5.960464477539063e-8_h - float_exponent_bias<T>;
        }
    }

    // Provides ~99.7% accuracy, significantly faster than ln()
    FORCE_INLINE constexpr float64_t fast_ln(float64_t v) {
        return fast_log2(v) * f64::LN2;
    }
    FORCE_INLINE constexpr float32_t fast_ln(float32_t v) {
        return fast_log2(v) * f32::LN2;
    }
    FORCE_INLINE constexpr float16_t fast_ln(float16_t v) {
        return fast_log2(v) * f16::LN2;
    }

    // Provides ~99.99% accuracy, slightly slower than fast_ln
    template <float_t T>
    FORCE_INLINE constexpr T fast_ln_poly(T v) {
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

    template <float_t T1, float_t T2, typename TOut = common_t<T1, T2>>
    FORCE_INLINE constexpr TOut fast_log(T1 v, T2 base) {
        return static_cast<TOut>(fast_log2(v) / fast_log2(base));
    }

    // Provides complete accuracy, slow to calculate
    template <numeric_t T>
    FORCE_INLINE constexpr T log(T v) {
        return ::std::log(v);
    }


    /////////////////////////////////////////////////////////
    // Exponentials

    template <float_t T>
    FORCE_INLINE constexpr T fast_exp2_int(int v) {
        return static_cast<T>(1ull << v);
    }

    template <float_t T>
    FORCE_INLINE constexpr T fast_exp2(T v) {
        constexpr float64_t c1 = 0.69314718;
        constexpr float64_t c2 = 0.24022651;
        constexpr float64_t c3 = 0.05550411;
        const int i = static_cast<int>(v);
        const T d = v - i;
        return fast_exp2_int<T>(i) * static_cast<T>(1.0 + d * (c1 + d * (c2 + d * c3)));
    }

    FORCE_INLINE constexpr float64_t fast_exp(float64_t v) {
        return fast_exp2(v * f64::LOG2E);
    }
    FORCE_INLINE constexpr float32_t fast_exp(float32_t v) {
        return fast_exp2(v * f32::LOG2E);
    }
    FORCE_INLINE constexpr float16_t fast_exp(float16_t v) {
        return fast_exp2(v * f16::LOG2E);
    }

    template <float_t T1, float_t T2, typename TOut = common_t<T1, T2>>
    FORCE_INLINE constexpr TOut fast_exp(T1 v, T2 base) {
        return fast_exp2<TOut>(v * fast_log2(base));
    }

    FORCE_INLINE constexpr float64_t fast_exp10(float64_t v) {
        return fast_exp(v * f64::LOG10E);
    }
    FORCE_INLINE constexpr float32_t fast_exp10(float32_t v) {
        return fast_exp(v * f32::LOG10E);
    }
    FORCE_INLINE constexpr float16_t fast_exp10(float16_t v) {
        return fast_exp(v * f16::LOG10E);
    }

    template <numeric_t T>
    FORCE_INLINE constexpr T sqr(T v) {
        return v * v;
    }

    template <numeric_t T>
    FORCE_INLINE constexpr T cube(T v) {
        return v * v * v;
    }

    template <numeric_t T>
    FORCE_INLINE constexpr T quad(T v) {
        return v * v * v * v;
    }

    template <numeric_t T>
    FORCE_INLINE constexpr T quint(T v) {
        return v * v * v * v * v;
    }

    template <numeric_t T1, numeric_t T2, typename TOut = common_t<T1, T2>>
    FORCE_INLINE constexpr TOut pow(T1 v, T2 p) {
        return static_cast<TOut>(std::pow(static_cast<long double>(v),
                                          static_cast<long double>(p)));
    }


    /////////////////////////////////////////////////////////
    // Square roots

    template <float_t T>
    FORCE_INLINE constexpr T sqrt(T v) {
        if (is_close_to_zero(v)) {
            return static_cast<T>(0.0);
        }
        return static_cast<T>(::std::sqrt(static_cast<float64_t>(v)));
    }

    template <float_t T>
    FORCE_INLINE constexpr T inv_sqrt(T v) {
        if (is_close_to_zero(v)) {
            return static_cast<T>(0.0);
        }
        return static_cast<T>(1.0 / ::std::sqrt(static_cast<float64_t>(v)));
    }

    FORCE_INLINE constexpr float64_t fast_inv_sqrt(float64_t v) {
        const int64_t i = 0x5FE6EB50C7B537A9 - (::std::bit_cast<int64_t>(v) >> 1);
        const float64_t y = ::std::bit_cast<float64_t>(i);
        return y * (1.5 - (v * 0.5 * y * y));
    }

    FORCE_INLINE constexpr float32_t fast_inv_sqrt(float32_t v) {
        const int32_t i = 0x5F3759DF - (::std::bit_cast<int32_t>(v) >> 1);
        const float32_t y = ::std::bit_cast<float32_t>(i);
        return y * (1.5f - (v * 0.5f * y * y));
    }

    FORCE_INLINE constexpr float16_t fast_inv_sqrt(float16_t v) {
        const uint16_t i = 0x5F30 - (::std::bit_cast<uint16_t>(v) >> 1);
        const float32_t y = float16_t::float16_to_32(i);
        return y * (1.5_h - (v * 0.5_h * y * y));
    }

    template <float_t T>
    FORCE_INLINE constexpr T fast_sqrt(T v, uint16_t iterations = 1) {
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

    template <float_t T>
    FORCE_INLINE constexpr T fast_rsqrt(T v, uint16_t iterations = 1) {
        const float64_t v64 = static_cast<float64_t>(v);
        if (is_close_to_zero(v64) || iterations == 0) {
            return eastl::numeric_limits<T>::quiet_NaN();
        }
        return static_cast<T>(1.0 / fast_sqrt(v, iterations));
    }

    template <float_t T>
    FORCE_INLINE constexpr T rsqrt(T v) {
        const float64_t v64 = static_cast<float64_t>(v);
        if (is_close_to_zero(v64)) {
            return eastl::numeric_limits<T>::quiet_NaN();
        }
        return static_cast<T>(1.0 / ::std::sqrt(v64));
    }


    /////////////////////////////////////////////////////////
    // Clamping

    template <numeric_t T>
    FORCE_INLINE constexpr T clamp(T v, T m, T M) {
        return v < m ? m : v > M ? M : v;
    }

    template <numeric_t T>
    FORCE_INLINE constexpr T saturate(T v) {
        return clamp(v, 0, 1);
    }


    /////////////////////////////////////////////////////////
    // Rounding

    template <float_t T>
    FORCE_INLINE constexpr T round(T v) {
        return ::std::round(v);
    }
    MAKE_CONSTEXPR_FUNC_FOR_FLOAT16(round);

    template <float_t T>
    FORCE_INLINE constexpr T floor(T v) {
        return ::std::floor(v);
    }
    MAKE_CONSTEXPR_FUNC_FOR_FLOAT16(floor);

    template <float_t T>
    FORCE_INLINE constexpr T ceil(T v) {
        return ::std::ceil(v);
    }
    MAKE_CONSTEXPR_FUNC_FOR_FLOAT16(ceil);

    template <float_t T>
    FORCE_INLINE constexpr T frac(T v) {
        return v - floor(v);
    }


    /////////////////////////////////////////////////////////
    // Interpolation

    template <float_t T1, float_t T2, typename TOut = common_t<T1, T2>>
    FORCE_INLINE constexpr TOut lerp(T1 from, T2 to, T2 t) {
        return static_cast<TOut>(from + (to - from) * t);
    }

    template <float_t T>
    FORCE_INLINE constexpr T remap(T v, T om, T oM, T nm, T nM) {
        return nm + (v - om) * (nM - nm) / (oM - om);
    }


    /////////////////////////////////////////////////////////
    // Trigonometry

    template <numeric_t T>
    FORCE_INLINE constexpr T deg_to_rad(T v) {
        return static_cast<T>(static_cast<float64_t>(v) * ldbl::DegToRad);
    }

    template <numeric_t T>
    FORCE_INLINE constexpr T rad_to_deg(T v) {
        return static_cast<T>(static_cast<float64_t>(v) * ldbl::RadToDeg);
    }

    template <float_t T>
    FORCE_INLINE T sin(T rads) { return ::std::sin(rads); }
    MAKE_FUNC_FOR_FLOAT16(sin);

    template <float_t T>
    FORCE_INLINE T cos(T rads) { return ::std::cos(rads); }
    MAKE_FUNC_FOR_FLOAT16(cos);

    template <float_t T>
    FORCE_INLINE T tan(T rads) { return ::std::tan(rads); }
    MAKE_FUNC_FOR_FLOAT16(tan);

    template <float_t T>
    FORCE_INLINE T asin(T rads) { return ::std::asin(rads); }
    MAKE_FUNC_FOR_FLOAT16(asin);

    template <float_t T>
    FORCE_INLINE T acos(T rads) { return ::std::acos(rads); }
    MAKE_FUNC_FOR_FLOAT16(acos);

    template <float_t T>
    FORCE_INLINE T atan(T rads) { return ::std::atan(rads); }
    MAKE_FUNC_FOR_FLOAT16(atan);

    template <float_t T>
    FORCE_INLINE T atan2(T y, T x) { return ::std::atan2(y, x); }
    MAKE_FUNC_FOR_FLOAT16_2ARGS(atan2);


    /////////////////////////////////////////////////////////
    // Modulo

    template <numeric_t T1, numeric_t T2, typename TOut = common_t<T1, T2>>
    FORCE_INLINE constexpr TOut mod(T1 v, T2 d) {
        if constexpr (::eastl::is_integral<TOut>::value) {
            // Regular integral modulo
            return d == 0 ? 0 : static_cast<TOut>(v % d);
        } else {
            // Floating modulo
            using intX_t = float_to_int_bit_matching<TOut>;

            const intX_t bits = ::std::bit_cast<intX_t>(v);
            const intX_t expo_bits =
                ::eastl::is_same_v<TOut, float64_t> ? 0x7FF :
                ::eastl::is_same_v<TOut, float32_t> ? 0xFF :
                                                      0x1F;                       // float16
            const intX_t expo =
                ((bits >> float_exponent_shift<TOut>) & expo_bits) - float_exponent_bias<TOut>;

            if (expo <= 0) {
                return static_cast<TOut>(0);
            }
            TOut wholePart = ::std::bit_cast<TOut>(bits & ~((1LL << (float_exponent_shift<TOut> -expo)) - 1));
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

    /////////////////////////////////////////////////////////////////////
    // Magnitude Squared

    FORCE_INLINE constexpr float magnitude_sqr(float x, float y) {
        return x * x + y * y;
    }

    FORCE_INLINE constexpr float magnitude_sqr(const float2::values& xy) {
        return magnitude_sqr(xy.x(), xy.y());
    }

    FORCE_INLINE constexpr float magnitude_sqr(float x, float y, float z) {
        return x * x + y * y + z * z;
    }
    
    FORCE_INLINE constexpr float magnitude_sqr(const float3::values& xyz) {
        return magnitude_sqr(xyz.x(), xyz.y(), xyz.z());
    }

    FORCE_INLINE constexpr float magnitude_sqr(float x, float y, float z, float w) {
        return x * x + y * y + z * z + w * w;
    }
    
    FORCE_INLINE constexpr float magnitude_sqr(const float4::values& xyzw) {
        return magnitude_sqr(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w());
    }



    /////////////////////////////////////////////////////////////////////
    // Magnitude

    FORCE_INLINE constexpr float magnitude(float x, float y) {
        return sqrtf(magnitude_sqr(x, y));
    }
    
    FORCE_INLINE constexpr float magnitude(const float2::values& xy) {
        return sqrtf(magnitude_sqr(xy.x(), xy.y()));
    }

    FORCE_INLINE constexpr float magnitude(float x, float y, float z) {
        return sqrtf(magnitude_sqr(x, y, z));
    }
    
    FORCE_INLINE constexpr float magnitude(const float3::values& xyz) {
        return sqrtf(magnitude_sqr(xyz.x(), xyz.y(), xyz.z()));
    }

    FORCE_INLINE constexpr float magnitude(float x, float y, float z, float w) {
        return sqrtf(magnitude_sqr(x, y, z, w));
    }
    
    FORCE_INLINE constexpr float magnitude(const float4::values& xyzw) {
        return sqrtf(magnitude_sqr(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w()));
    }


    /////////////////////////////////////////////////////////////////////
    // Magnitude (fast)

    FORCE_INLINE float magnitude_fast(float x, float y, unsigned int iterationCount = 2) {
        if (is_normalized(x, y)) {
            return 1.0f;
        }
        const float magSqr = magnitude_sqr(x, y);
        if (is_close_to_zero(magSqr)) {
            return 0.0f;
        }
        return 1.0f / fast_rsqrt(magSqr, iterationCount);
    }
    
    FORCE_INLINE constexpr float magnitude_fast(const float2::values& xy, unsigned int iterationCount = 2) {
        return magnitude_fast(xy.x(), xy.y(), iterationCount);
    }

    FORCE_INLINE float magnitude_fast(float x, float y, float z, unsigned int iterationCount = 2) {
        if (is_normalized(x, y, z)) {
            return 1.0f;
        }
        const float magSqr = magnitude_sqr(x, y, z);
        if (is_close_to_zero(magSqr)) {
            return 0.0f;
        }
        return 1.0f / fast_rsqrt(magSqr, iterationCount);
    }
    
    FORCE_INLINE constexpr float magnitude_fast(const float3::values& xyz, unsigned int iterationCount = 2) {
        return magnitude_fast(xyz.x(), xyz.y(), xyz.z(), iterationCount);
    }

    FORCE_INLINE float magnitude_fast(float x, float y, float z, float w, unsigned int iterationCount = 2) {
        if (is_normalized(x, y, z, w)) {
            return 1.0f;
        }
        const float magSqr = magnitude_sqr(x, y, z, w);
        if (is_close_to_zero(magSqr)) {
            return 0.0f;
        }
        return 1.0f / fast_rsqrt(magSqr, iterationCount);
    }
    
    FORCE_INLINE constexpr float magnitude_fast(const float4::values& xyzw, unsigned int iterationCount = 2) {
        return magnitude_fast(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w(), iterationCount);
    }


    /////////////////////////////////////////////////////////////////////
    // Normalization check

    FORCE_INLINE constexpr bool is_normalized(float x, float y) {
        return is_close_to(magnitude_sqr(x, y), 1.0f);
    }
    
    FORCE_INLINE constexpr bool is_normalized(const float2::values& xy) {
        return is_normalized(xy.x(), xy.y());
    }

    FORCE_INLINE constexpr bool is_normalized(float x, float y, float z) {
        return is_close_to(magnitude_sqr(x, y, z), 1.0f);
    }
    
    FORCE_INLINE constexpr bool is_normalized(const float3::values& xyz) {
        return is_normalized(xyz.x(), xyz.y(), xyz.z());
    }

    FORCE_INLINE constexpr bool is_normalized(float x, float y, float z, float w) {
        return is_close_to(magnitude_sqr(x, y, z, w), 1.0f);
    }
    
    FORCE_INLINE constexpr bool is_normalized(const float4::values& xyzw) {
        return is_normalized(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w());
    }


    /////////////////////////////////////////////////////////////////////
    // Normalization

    FORCE_INLINE float2::values normalize(float x, float y) {
        const float mag = magnitude(x, y);
        if (is_close_to_zero(mag)) {
            return float2::NaN;
        }
        return { x / mag, y / mag };
    }
    
    FORCE_INLINE constexpr float2::values normalize(const float2::values& xy) {
        return normalize(xy.x(), xy.y());
    }

    FORCE_INLINE float3::values normalize(float x, float y, float z) {
        const float mag = magnitude(x, y, z);
        if (is_close_to_zero(mag)) {
            return float3::NaN;
        }
        return { x / mag, y / mag, z / mag };
    }
    
    FORCE_INLINE constexpr float3::values normalize(const float3::values& xyz) {
        return normalize(xyz.x(), xyz.y(), xyz.z());
    }

    FORCE_INLINE float4::values normalize(float x, float y, float z, float w) {
        const float mag = magnitude(x, y, z, w);
        if (is_close_to_zero(mag)) {
            return float4::NaN;
        }
        return { x / mag, y / mag, z / mag, w / mag };
    }
    
    FORCE_INLINE constexpr float4::values normalize(const float4::values& xyzw) {
        return normalize(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w());
    }


    /////////////////////////////////////////////////////////////////////
    // Normalization (fast)

    FORCE_INLINE constexpr float2::values normalize_fast(float x, float y, unsigned int iterationCount = 2) {
        if (is_normalized(x, y)) {
            return { x, y };
        }
        const float magSqr = magnitude_sqr(x, y);
        if (is_close_to_zero(magSqr)) {
            return float2::NaN;
        }
        if (is_close_to(x, y)) {
            return { x * f32::RSQRT2, y * f32::RSQRT2 };
        }
        const float mag = fast_rsqrt(magSqr, iterationCount);
        return { x * mag, y * mag };
    }
    
    FORCE_INLINE constexpr float2::values normalize_fast(const float2::values& xy, unsigned int iterationCount = 2) {
        return normalize_fast(xy.x(), xy.y(), iterationCount);
    }

    FORCE_INLINE constexpr float3::values normalize_fast(float x, float y, float z, unsigned int iterationCount = 2) {
        if (is_normalized(x, y, z)) {
            return { x, y, z };
        }
        const float magSqr = magnitude_sqr(x, y, z);
        if (is_close_to_zero(magSqr)) {
            return float3::NaN;
        }
        const float mag = fast_rsqrt(magSqr, iterationCount);
        return { x * mag, y * mag, z * mag };
    }
    
    FORCE_INLINE constexpr float3::values normalize_fast(const float3::values& xyz, unsigned int iterationCount = 2) {
        return normalize_fast(xyz.x(), xyz.y(), xyz.z(), iterationCount);
    }

    FORCE_INLINE constexpr float4::values normalize_fast(float x, float y, float z, float w, unsigned int iterationCount = 2) {
        if (is_normalized(x, y, z, w)) {
            return { x, y, z, w };
        }
        const float magSqr = magnitude_sqr(x, y, z, w);
        if (is_close_to_zero(magSqr)) {
            return float4::NaN;
        }
        const float mag = fast_rsqrt(magSqr, iterationCount);
        return { x * mag, y * mag, z * mag, w * mag };
    }
    
    FORCE_INLINE constexpr float4::values normalize_fast(const float4::values& xyzw, unsigned int iterationCount = 2) {
        return normalize_fast(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w(), iterationCount);
    }


    /////////////////////////////////////////////////////////////////////
    // Dot product

    FORCE_INLINE constexpr float dot(float x1, float y1, float x2, float y2) {
        return x1 * x2 + y1 * y2;
    }
    
    FORCE_INLINE constexpr float dot(const float2::values& xy1, const float2::values& xy2) {
        return dot(xy1.x(), xy1.y(), xy2.x(), xy2.y());
    }

    FORCE_INLINE constexpr float dot(float x1, float y1, float z1, float x2, float y2, float z2) {
        return x1 * x2 + y1 * y2 + z1 * z2;
    }
    
    FORCE_INLINE constexpr float dot(const float3::values& xyz1, const float3::values& xyz2) {
        return dot(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
    }

    FORCE_INLINE constexpr float dot(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) {
        return x1 * x2 + y1 * y2 + z1 * z2 + w1 * w2;
    }
    
    FORCE_INLINE constexpr float dot(const float4::values& xyzw1, const float4::values& xyzw2) {
        return dot(xyzw1.x(), xyzw1.y(), xyzw1.z(), xyzw1.w(), xyzw2.x(), xyzw2.y(), xyzw2.z(), xyzw2.w());
    }


    /////////////////////////////////////////////////////////////////////
    // Distance between two points (squared)

    FORCE_INLINE constexpr float distance_sqr(float x1, float y1, float x2, float y2) {
        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    }
    
    FORCE_INLINE constexpr float distance_sqr(const float2::values& xy1, const float2::values& xy2) {
        return distance_sqr(xy1.x(), xy1.y(), xy2.x(), xy2.y());
    }

    FORCE_INLINE constexpr float distance_sqr(float x1, float y1, float z1, float x2, float y2, float z2) {
        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
    }
    
    FORCE_INLINE constexpr float distance_sqr(const float3::values& xyz1, const float3::values& xyz2) {
        return distance_sqr(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
    }

    FORCE_INLINE constexpr float distance_sqr(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) {
        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) + (w1 - w2) * (w1 - w2);
    }
    
    FORCE_INLINE constexpr float distance_sqr(const float4::values& xyzw1, const float4::values& xyzw2) {
        return distance_sqr(xyzw1.x(), xyzw1.y(), xyzw1.z(), xyzw1.w(), xyzw2.x(), xyzw2.y(), xyzw2.z(), xyzw2.w());
    }


    /////////////////////////////////////////////////////////////////////
    // Distance between two points

    FORCE_INLINE constexpr float distance(float x1, float y1, float x2, float y2) {
        return sqrtf(distance_sqr(x1, y1, x2, y2));
    }
    
    FORCE_INLINE constexpr float distance(const float2::values& xy1, const float2::values& xy2) {
        return distance(xy1.x(), xy1.y(), xy2.x(), xy2.y());
    }

    FORCE_INLINE constexpr float distance(float x1, float y1, float z1, float x2, float y2, float z2) {
        return sqrtf(distance_sqr(x1, y1, z1, x2, y2, z2));
    }
    
    FORCE_INLINE constexpr float distance(const float3::values& xyz1, const float3::values& xyz2) {
        return distance(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
    }

    FORCE_INLINE constexpr float distance(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2) {
        return sqrtf(distance_sqr(x1, y1, z1, w1, x2, y2, z2, w2));
    }
    
    FORCE_INLINE constexpr float distance(const float4::values& xyzw1, const float4::values& xyzw2) {
        return distance(xyzw1.x(), xyzw1.y(), xyzw1.z(), xyzw1.w(), xyzw2.x(), xyzw2.y(), xyzw2.z(), xyzw2.w());
    }


    /////////////////////////////////////////////////////////////////////
    // Perpendicular (2D only)

    FORCE_INLINE constexpr float2::values perpendicular(float x, float y, bool clockwise = true) {
        return clockwise
            ? float2::values{ y, -x }
            : float2::values{ -y, x };
    }

    FORCE_INLINE constexpr float2::values perpendicular(const float2::values& xy, bool clockwise = true) {
        return perpendicular(xy.x(), xy.y(), clockwise);
    }


    /////////////////////////////////////////////////////////////////////
    // Cross product (2D)

    FORCE_INLINE constexpr float cross(float x1, float y1, float x2, float y2) {
        return x1 * y2 - y1 * x2;
    }
    
    FORCE_INLINE constexpr float cross(const float2::values& xy1, const float2::values& xy2) {
        return cross(xy1.x(), xy1.y(), xy2.x(), xy2.y());
    }


    /////////////////////////////////////////////////////////////////////
    // Cross product (3D)

    FORCE_INLINE constexpr float3::values cross(float x1, float y1, float z1, float x2, float y2, float z2) {
        return {
            y1 * z2 - z1 * y2,
            z1 * x2 - x1 * z2,
            x1 * y2 - y1 * x2
        };
    }
    
    FORCE_INLINE constexpr float3::values cross(const float3::values& xyz1, const float3::values& xyz2) {
        return cross(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
    }


    /////////////////////////////////////////////////////////////////////
    // Angle between two vectors (2D)

    FORCE_INLINE constexpr float angle(float x1, float y1, float x2, float y2) {
        const float combinedMagSqr = magnitude_sqr(x1, y1) * magnitude_sqr(x2, y2);
        if (is_close_to_zero(combinedMagSqr)) {
            return 0.0f;
        }
        const float dotProduct = dot(x1, y1, x2, y2);
        return acosf(dotProduct / sqrt(combinedMagSqr));
    }
    
    FORCE_INLINE constexpr float angle(const float2::values& xy1, const float2::values& xy2) {
        return angle(xy1.x(), xy1.y(), xy2.x(), xy2.y());
    }


    /////////////////////////////////////////////////////////////////////
    // Angle between two vectors (3D)

    FORCE_INLINE constexpr float angle(float x1, float y1, float z1, float x2, float y2, float z2) {
        const float combinedMagSqr = magnitude_sqr(x1, y1, z1) * magnitude_sqr(x2, y2, z2);
        if (is_close_to_zero(combinedMagSqr)) {
            return 0.0f;
        }
        const float dotProduct = dot(x1, y1, z1, x2, y2, z2);
        return acosf(dotProduct / sqrt(combinedMagSqr));
    }
    
    FORCE_INLINE constexpr float angle(const float3::values& xyz1, const float3::values& xyz2) {
        return angle(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
    }


    /////////////////////////////////////////////////////////////////////
    // Signed angle between two vectors (2D)

    FORCE_INLINE constexpr float angle_signed(float x1, float y1, float x2, float y2) {
        const float combinedMagSqr = magnitude_sqr(x1, y1) * magnitude_sqr(x2, y2);
        if (is_close_to_zero(combinedMagSqr)) {
            return 0.0f;
        }
        const float dotProduct = dot(x1, y1, x2, y2);
        const float angle = acosf(dotProduct / sqrt(combinedMagSqr));
        return cross(x1, y1, x2, y2) < 0.0f ? -angle : angle;
    }
    
    FORCE_INLINE constexpr float angle_signed(const float2::values& xy1, const float2::values& xy2) {
        return angle_signed(xy1.x(), xy1.y(), xy2.x(), xy2.y());
    }


    /////////////////////////////////////////////////////////////////////
    // Signed angle between two vectors (3D)

    FORCE_INLINE constexpr float angle_signed(float x1, float y1, float z1, float x2, float y2, float z2, float normalX, float normalY, float normalZ) {
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

    FORCE_INLINE constexpr float angle_signed(const float3::values& xyz1, const float3::values& xyz2, const float3::values& normal) {
        return angle_signed(
            xyz1.x(), xyz1.y(), xyz1.z(),
            xyz2.x(), xyz2.y(), xyz2.z(),
            normal.x(), normal.y(), normal.z());
    }


    /////////////////////////////////////////////////////////////////////
    // Reflect a vector off a normal (2D)

    FORCE_INLINE constexpr float2::values reflect(float x, float y, float normalX, float normalY) {
        const float dotProductBy2 = 2.0f * dot(x, y, normalX, normalY);
        return {
            x - dotProductBy2 * normalX,
            y - dotProductBy2 * normalY
        };
    }
    
    FORCE_INLINE constexpr float2::values reflect(const float2::values& xy1, const float2::values& xy2) {
        return reflect(xy1.x(), xy1.y(), xy2.x(), xy2.y());
    }


    /////////////////////////////////////////////////////////////////////
    // Reflect a vector off a normal (3D)

    FORCE_INLINE constexpr float3::values reflect(float x, float y, float z, float normalX, float normalY, float normalZ) {
        const float dotProductBy2 = 2.0f * dot(x, y, z, normalX, normalY, normalZ);
        return {
            x - dotProductBy2 * normalX,
            y - dotProductBy2 * normalY,
            z - dotProductBy2 * normalZ };
    }
    
    FORCE_INLINE constexpr float3::values reflect(const float3::values& xyz1, const float3::values& xyz2) {
        return reflect(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
    }


    /////////////////////////////////////////////////////////////////////
    // Project a vector onto another vector (2D)

    FORCE_INLINE constexpr float2::values project(float x1, float y1, float x2, float y2) {
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
    
    FORCE_INLINE constexpr float2::values project(const float2::values& xy1, const float2::values& xy2) {
        return project(xy1.x(), xy1.y(), xy2.x(), xy2.y());
    }


    /////////////////////////////////////////////////////////////////////
    // Project a vector onto another vector (3D)

    FORCE_INLINE constexpr float3::values project(float x1, float y1, float z1, float x2, float y2, float z2) {
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
    
    FORCE_INLINE constexpr float3::values project(const float3::values& xyz1, const float3::values& xyz2) {
        return project(xyz1.x(), xyz1.y(), xyz1.z(), xyz2.x(), xyz2.y(), xyz2.z());
    }


    /////////////////////////////////////////////////////////////////////
    // Rotate a vector (2D)

    FORCE_INLINE constexpr float2::values rotate(float x, float y, float radians) {
        const float cosTheta = cosf(radians);
        const float sinTheta = sinf(radians);
        return {
            x * cosTheta - y * sinTheta,
            x * sinTheta + y * cosTheta
        };
    }

    FORCE_INLINE constexpr float2::values rotate(const float2::values& xy, float radians) {
        return rotate(xy.x(), xy.y(), radians);
    }

    FORCE_INLINE constexpr float2::values rotate_offset(float x, float y, float xOffset, float yOffset, float radians) {
        const float cosTheta = cosf(radians);
        const float sinTheta = sinf(radians);
        return {
            (x - xOffset) * cosTheta - (y - yOffset) * sinTheta + xOffset,
            (x - xOffset) * sinTheta + (y - yOffset) * cosTheta + yOffset
        };
    }

    FORCE_INLINE constexpr float2::values rotate_offset(const float2::values& xy, const float2::values& offset, float radians) {
        return rotate_offset(xy.x(), xy.y(), offset.x(), offset.y(), radians);
    }


    /////////////////////////////////////////////////////////////////////
    // Rotate a vector (3D)

    FORCE_INLINE constexpr float3::values rotate(float x, float y, float z, float radians, float axisX, float axisY, float axisZ) {
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

    FORCE_INLINE constexpr float3::values rotate(const float3::values& xyz, float radians, const float2::values& axis) {
        return rotate(
            xyz.x(), xyz.y(), xyz.z(),
            radians,
            axis.x(), axis.y(), 0.0f);
    }


    /////////////////////////////////////////////////////////////////////
    // Project a 4D vector to 3D (perspective divide)

    FORCE_INLINE constexpr float3::values project_to_3d(float x, float y, float z, float w) {
        if (is_close_to_zero(w)) {
            return float3::NaN;
        }
        return {
            x / w,
            y / w,
            z / w
        };
    }

    FORCE_INLINE constexpr float3::values project_to_3d(const float4::values& xyzw) {
        return project_to_3d(xyzw.x(), xyzw.y(), xyzw.z(), xyzw.w());
    }


    /////////////////////////////////////////////////////////////////////
    // Project a 3D point to a plane defined by a 4D vector (plane equation: Ax + By + Cz + D = 0)

    FORCE_INLINE constexpr float3::values project_onto_plane(float x, float y, float z, float planeA, float planeB, float planeC, float planeD) {
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

    FORCE_INLINE constexpr float3::values project_onto_plane(const float3::values& xyz, float planeA, float planeB, float planeC, float planeD) {
        return project_onto_plane(
            xyz.x(), xyz.y(), xyz.z(),
            planeA, planeB, planeC, planeD);
    }

    FORCE_INLINE constexpr float3::values project_onto_plane(const float3::values& xyz, const float4::values& plane) {
        return project_onto_plane(
            xyz.x(), xyz.y(), xyz.z(),
            plane.x(), plane.y(), plane.z(), plane.w());
    }


    /////////////////////////////////////////////////////////////////////
    // Interpolation between two points (2D)

    namespace interpolation {
        FORCE_INLINE constexpr float2::values interpolate(float xStart, float yStart, float xEnd, float yEnd, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return {
                interpolate(xStart, xEnd, t, type, options),
                interpolate(yStart, yEnd, t, type, options)
            };
        }

        FORCE_INLINE constexpr float2::values interpolate(const float2::values& start, const float2::values& end, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return interpolate(start.x(), start.y(), end.x(), end.y(), t, type, options);
        }
    }


    /////////////////////////////////////////////////////////////////////
    // Interpolation between two points (3D)

    namespace interpolation {
        FORCE_INLINE constexpr float3::values interpolate(float xStart, float yStart, float zStart, float xEnd, float yEnd, float zEnd, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return {
                interpolate(xStart, xEnd, t, type, options),
                interpolate(yStart, yEnd, t, type, options),
                interpolate(zStart, zEnd, t, type, options)
            };
        }

        FORCE_INLINE constexpr float3::values interpolate(const float3::values& start, const float3::values& end, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return interpolate(start.x(), start.y(), start.z(), end.x(), end.y(), end.z(), t, type, options);
        }
    }


    /////////////////////////////////////////////////////////////////////
    // Interpolation between two points (4D)

    namespace interpolation {
        FORCE_INLINE constexpr float4::values interpolate(float xStart, float yStart, float zStart, float wStart, float xEnd, float yEnd, float zEnd, float wEnd, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return {
                interpolate(xStart, xEnd, t, type, options),
                interpolate(yStart, yEnd, t, type, options),
                interpolate(zStart, zEnd, t, type, options),
                interpolate(wStart, wEnd, t, type, options)
            };
        }

        FORCE_INLINE constexpr float4::values interpolate(const float4::values& start, const float4::values& end, float t, blend_func type, blend_opt options = ::eastl::monostate{}) {
            return interpolate(start.x(), start.y(), start.z(), start.w(), end.x(), end.y(), end.z(), end.w(), t, type, options);
        }
    }
}