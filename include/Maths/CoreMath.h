#include <float2.h>
#include <float3.h>
#include <float4.h>
#include <Maths/Constants.h>
#include <Maths/Quaternion.h>
#include <Maths/Matrix2x2.h>
#include <Maths/Matrix3x3.h>
#include <Maths/Matrix4x4.h>
#include <Maths/Interpolation.h>

#include <EASTL/type_traits.h>

#include <cmath>

namespace eloo::Math {

namespace {
template <typename T> concept number_t = eastl::is_arithmetic_v<T>;
template <typename T> concept float_t = eastl::is_floating_point_v<T>;
template <typename T> concept int_t = eastl::is_integral_v<T>;
template <number_t T1, number_t T2>
using common_t = eastl::common_type_t<T1, T2>;
template <float_t T>
using ftoi_t = eastl::conditional_t<eastl::is_same_v<T, float64_t>, int64_t,
               eastl::conditional_t<eastl::is_same_v<T, float32_t>, int32_t,
               eastl::conditional_t<eastl::is_same_v<T, float16_t>, uint16_t, void>>>;
template <int_t T>
using ttof_t = eastl::conditional_t<eastl::is_same_v<T, int64_t>, float64_t,
               eastl::conditional_t<eastl::is_same_v<T, int32_t>, float32_t,
               eastl::conditional_t<eastl::is_same_v<T, uint16_t>, float16_t, void>>>;

#define make_func_for_float16(f) \
template <> float16_t f(float16_t v) { \
    return float16_t::float32_to_16(f(float16_t::float16_to_32(v))); \
}
#define make_constexpr_func_for_float16(f) \
template <> constexpr float16_t f(float16_t v) { \
    return float16_t::float32_to_16(f(float16_t::float16_to_32(v))); \
}

#define make_func_for_float16_2args(f) \
template <> float16_t f(float16_t x, float16_t y) { \
    return float16_t::float32_to_16(f(float16_t::float16_to_32(x), float16_t::float16_to_32(y))); \
}
#define make_constexpr_func_for_float16_2args(f) \
template <> constexpr float16_t f(float16_t x, float16_t y) { \
    return float16_t::float32_to_16(f(float16_t::float16_to_32(x), float16_t::float16_to_32(y))); \
}

template <float_t T>
inline constexpr int32_t float_exponent_bias = eastl::is_same_v<T, float64_t> ? 0x03FF :
                                               eastl::is_same_v<T, float32_t> ? 0x007F :
                                               eastl::is_same_v<T, float16_t> ? 0x000F : 0x0000;
template <float_t T>
inline constexpr int32_t float_mantissa_mask = eastl::is_same_v<T, float64_t> ? 0x0034 :
                                               eastl::is_same_v<T, float32_t> ? 0x0017 :
                                               eastl::is_same_v<T, float16_t> ? 0x000A : 0x0000;
}


/////////////////////////////////////////////////////////
// Comparison

template <float_t T1, number_t T2>
constexpr bool is_close(T1 val1, T2 val2) {
    const T1 v = abs(val1 - static_cast<T1>(val2));
    if constexpr (sizeof(T1) >= 64) {
        return v <= static_cast<T2>(Consts::f64::Epsilon);
    } else if constexpr (sizeof(T1) == 32) {
        return v <= Consts::f32::Epsilon;
    } else {
        return v <= Consts::f16::Epsilon;
    }
}

template <number_t T1, number_t T2, typename TOut = common_t<T1, T2>>
constexpr TOut min(T1 v1, T2 v2) {
    return static_cast<TOut>(v1 <  v2 ? v1 : v2);
}

template <number_t T1, number_t T2, typename TOut = common_t<T1, T2>>
constexpr TOut max(T1 v1, T2 v2) {
    return static_cast<TOut>(v1 > v2 ? v1 : v2);
}

/////////////////////////////////////////////////////////
// Signs

template <number_t T>
constexpr T abs(T v)                { return std::abs(value); }
template <number_t T>
constexpr int sign(T v)             { return static_cast<int>((val > 0) - (val < 0)); }


/////////////////////////////////////////////////////////
// Logarithm

// Provides ~99.7% accuracy, significantly faster than log2()
template <float_t T> constexpr T fast_log2(T v) {
    using intX_t = ftoi_t<T>;
    if constexpr (eastl::is_same_v<T, float64_t>) {
        return std::bit_cast<intX_t>(v) * 1.4426950408889634e-16 - float_exponent_bias<T>;
    } else if constexpr (eastl::is_same_v<T, float32_t>) {
        return std::bit_cast<intX_t>(v) * 1.1920928955078125e-7f - float_exponent_bias<T>;
    } else {
        return std::bit_cast<intX_t>(v) * 5.960464477539063e-8_h - float_exponent_bias<T>;
    }
}

// Provides ~99.7% accuracy, significantly faster than ln()
constexpr float64_t fast_ln(float64_t v)        { return fast_log2(v) * Consts::f64::ln2; }
constexpr float32_t fast_ln(float32_t v)        { return fast_log2(v) * Consts::f32::ln2; }
constexpr float16_t fast_ln(float16_t v)        { return fast_log2(v) * Consts::f16::ln2; }

// Provides ~99.99% accuracy, slightly slower than fast_ln
template <float_t T>
constexpr T fast_ln_poly(T v) {
    constexpr float64_t _1_3rd = 0.33333333333333333;
    constexpr float64_t _1_5th = 0.2;
    constexpr float64_t _1_7th = 0.14285714285714285;
    const float64_t y = (static_cast<float64_t>(v) - 1.0) / (static_cast<float64_t>(v) + 1.0);
    const float64_t y2 = y * y;
    const float64_t r = 2.0 * y * (1.0 + y2 * (_1_3rd + y2 * (_1_5th + y2 * _1_7th)));
    if constexpr (eastl::is_same<T, float64_t>::value) {
        return r;
    } else {
        return static_cast<T>(r);
    }
}

template <float_t T1, float_t T2, typename TOut = common_t<T1, T2>>
constexpr TOut fast_log(T1 v, T2 base)          { return static_cast<TOut>(fast_log2(v) / fast_log2(base)); }

// Provides complete accuracy, slow to calculate
template <float_t T> constexpr T log(T v)       { return std::log(v); }


/////////////////////////////////////////////////////////
// Exponentials

template <float_t T>
constexpr T fast_exp2_int(int v)                { return static_cast<T>(1ull << v); }

template <float_t T>
constexpr T fast_exp2(T v) {
    constexpr float64_t c1 = 0.6960656421;
    constexpr float64_t c2 = 0.2244667935;
    constexpr float64_t c3 = 0.0790243555;
    const int i = static_cast<int>(v);
    const T d = v - i;
    return fast_exp2_int<T>(i) * static_cast<T>(1.0 + d * (c1 + d * (c2 + d * c3)));
}

constexpr float64_t fast_exp(float64_t v)       { return fast_exp2(v * Consts::f64::log2e); }
constexpr float32_t fast_exp(float32_t v)       { return fast_exp2(v * Consts::f32::log2e); }
constexpr float16_t fast_exp(float16_t v)       { return fast_exp2(v * Consts::f16::log2e); }
template <float_t T1, float_t T2, typename TOut = common_t<T1, T2>>
constexpr TOut fast_exp(T1 v, T2 base)          { return fast_exp<TOut>(v * fast_ln(base)); }

constexpr float64_t fast_exp10(float64_t v)     { return fast_exp(v * Consts::f64::log10e); }
constexpr float32_t fast_exp10(float32_t v)     { return fast_exp(v * Consts::f32::log10e); }
constexpr float16_t fast_exp10(float16_t v)     { return fast_exp(v * Consts::f16::log10e); }

template <number_t T> constexpr T sqr(T v)      { return v * v; }
template <number_t T> constexpr T cube(T v)     { return v * v * v; }
template <number_t T> constexpr T quad(T v)     { return v * v * v * v; }
template <number_t T> constexpr T quint(T v)    { return v * v * v * v * v; }


/////////////////////////////////////////////////////////
// Square roots

constexpr float64_t fast_inv_sqrt(float64_t v) {
    const int64_t i = 0x5FE6EB50C7B537A9 - (std::bit_cast<int64_t>(v) >> 1);
    const float64_t y = std::bit_cast<float64_t>(i);
    return y * (1.5 - (v * 0.5 * y * y));
}

constexpr float32_t fast_inv_sqrt(float32_t v) {
    const int32_t i = 0x5F3759DF - (std::bit_cast<int32_t>(v) >> 1);
    const float32_t y = std::bit_cast<float32_t>(i);
    return y * (1.5f - (v * 0.5f * y * y));
}

constexpr float16_t fast_inv_sqrt(float16_t v) {
    const uint16_t i = 0x5F30 - (std::bit_cast<uint16_t>(v) >> 1);
    const float16_t y = std::bit_cast<float16_t>(i);
    return y * (1.5_h - (v * 0.5_h * y * y));
}

template <float_t T> constexpr T fast_sqrt(T v, uint16_t iterations = 1) {
    const float64_t v64 = static_cast<float64_t>(v);
    if (is_close(v64, 0.0) || iterations == 0) {
        return static_cast<T>(0.0);
    }

    float64_t guess = v64 * 0.5;
    do {
        guess = (guess + v64 / guess) * 0.5;
    } while (--iterations > 0);
    return guess;
}

template <float_t T> constexpr T fast_rsqrt(T v, uint16_t iterations = 1) {
    const float64_t v64 = static_cast<float64_t>(v);
    if (is_close(v64, 0.0) || iterations == 0) {
        return static_cast<T>(0.0);
    }
    return static_cast<T>(1.0 / fast_sqrt(v, iterations));
}


/////////////////////////////////////////////////////////
// Clamping

template <number_t T> constexpr T clamp(T v, T m, T M) {
    return v < m ? m : v > M ? M : v;
}

template <number_t T> constexpr T saturate(T v)     { return clamp(v, 0, 1); }


/////////////////////////////////////////////////////////
// Rounding

template <float_t T> constexpr T floor(T v)         { return std::floor(v); }
template <float_t T> constexpr T ceil(T v)          { return std::ceil(v); }
template <float_t T> constexpr T frac(T v)          { return v - floor(v); }

make_constexpr_func_for_float16(floor)
make_constexpr_func_for_float16(ceil)


/////////////////////////////////////////////////////////
// Conversions

template <float_t T1, float_t T2, typename TOut = common_t<T1, T2>>
constexpr TOut lerp(T1 from, T2 to, T2 t)           { return static_cast<TOut>(from + (to - from) * t); }

template <float_t T>
constexpr T remap(T v, T om, T oM, T nm, T nM)      { return nm + (v - om) * (nM - nm) / (oM - om); }


/////////////////////////////////////////////////////////
// Trigonometry

template <number_t T> constexpr T deg_to_rad(T v)   { return static_cast<T>(static_cast<float64_t>(v) * Consts::ldbl::DegToRad); }
template <number_t T> constexpr T rad_to_deg(T v)   { return static_cast<T>(static_cast<float64_t>(v) * Consts::ldbl::RadToDeg); }

template <float_t T> T sin(T rads)                  { return std::sin(rads); }
template <float_t T> T cos(T rads)                  { return std::cos(rads); }
template <float_t T> T tan(T rads)                  { return std::tan(rads); }
make_func_for_float16(sin)
make_func_for_float16(cos)
make_func_for_float16(tan)

template <float_t T> T asin(T rads)                 { return std::asin(rads); }
template <float_t T> T acos(T rads)                 { return std::acos(rads); }
template <float_t T> T atan(T rads)                 { return std::atan(rads); }
make_func_for_float16(asin)
make_func_for_float16(acos)
make_func_for_float16(atan)

template <float_t T> T atan2(T y, T x)              { return std::atan2(y, x); }
make_func_for_float16_2args(atan2)


/////////////////////////////////////////////////////////
// Modulo

template <number_t T1, number_t T2, typename TOut = common_t<T1, T2>>
constexpr TOut mod(T1 v, T2 d) {
    if constexpr (eastl::is_integral<TOut>::value) {
        // Regular integral modulo
        return d == 0 ? 0 : return static_cast<TOut>(v % d);
    } else {
        // Floating modulo
        using intX_t = ftoi_t<TOut>;

        const intX_t bits = std::bit_cast<intX_t>(v);
        const intX_t expo = ((bits >> float_mantissa_mask<TOut>) & 0xFF) - float_exponent_bias<TOut>;
        if (expo <= 0) {
            return static_cast<TOut>(0);
        }
        TOut wholePart = std::bit_cast<TOut>(bits & ~((1LL << (float_mantissa_mask<TOut> - expo)) - 1));
        return static_cast<TOut>(v) - wholePart * d;
    }
}


/////////////////////////////////////////////////////////
//
//                       float2
//
/////////////////////////////////////////////////////////

bool isNormalized(const float2& v);

float2 normalize(const float2& v);

float2 normalizeFast(const float2& v, unsigned int iterationCount = 1);

float magnitudeFast(const float2& v, unsigned int iterationCount = 1);

float dot(const float2& v1, const float2& v2);

float distance(const float2& v1, const float2& v2);

float distanceSqr(const float2& v1, const float2& v2);

float2 perpendicular(const float2& v);

float angle(const float2& v1, const float2& v2);

float angleSigned(const float2& v1, const float2& v2);

float2 reflect(const float2& v, const float2& normal);

float2 project(const float2& v, const float2& normal);

float2 rotate(const float2& v, float radians);

namespace Interpolation {
float2 interpolate(const float2& start, const float2& end, double t, Type type, Options options = eastl::monostate{});
}


/////////////////////////////////////////////////////////
//
//                       float3
//
/////////////////////////////////////////////////////////

bool isNormalized(const float3& v);

float3 normalize(const float3& v);

float3 normalizeFast(const float3& v, unsigned int iterationCount = 1);

float magnitudeFast(const float3& v, unsigned int iterationCount = 1);

float dot(const float3& v1, const float3& v2);

float3 cross(const float3& v1, const float3& v2);

float distance(const float3& v1, const float3& v2);

float distanceSqr(const float3& v1, const float3& v2);

float angle(const float3& v1, const float3& v2);

float angleSigned(const float3& v1, const float3& v2, const float3& normal);

float3 reflect(const float3& v, const float3& normal);

float3 project(const float3& v, const float3& normal);

float3 rotate(const float3& toRotate, float radians, const float3& axis);

namespace Interpolation {
float3 interpolate(const float3& start, const float3& end, double t, Type type, Options options = eastl::monostate{});
}


/////////////////////////////////////////////////////////
//
//                       float4
//
/////////////////////////////////////////////////////////

bool isNormalized(const float4& v);

float4 normalize(const float4& v);

float4 normalizeFast(const float4& v, unsigned int iterationCount = 1);

float magnitudeFast(const float4& v, unsigned int iterationCount = 1);

float dot(const float4& v1, const float4& v2);

float distance(const float4& v1, const float4& v2);

float distanceSqr(const float4& v1, const float4& v2);

float3 projectTo3D(const float4& v);

float distanceToPlane(const float4& plane, const float3& point);

namespace Interpolation {
float4 interpolate(const float4& start, const float4& end, double t, Type type, Options options = eastl::monostate{});
}




float3 operator * (const float3& v, const Matrix3x3& m) {

    m.row(0)
    return { m.r1 * v.x + m.r2 * v.y + m.r3 * v.z }; }


};