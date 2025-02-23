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

#define NUMERIC_TEMPLATE template<typename T> inline eastl::enable_if<eastl::is_arithmetic<T>::value, T>::type
#define NUMERIC_TEMPLATE_RETURN_TYPE(_T) template<typename T> inline eastl::enable_if<eastl::is_arithmetic<T>::value, _T>::type

namespace eloo::Math {

/////////////////////////////////////////////////////////
// Fast square variants

inline double invSqrtFast(double v) {
    const int64_t i = 0x5FE6EB50C7B537A9 - (*(int64_t*)&v >> 1);
    const double y = *(double*)&i;
    return y * (1.5 - (v * 0.5 * y * y));
}

inline float invSqrtFast(float v) {
    const int32_t i = 0x5F3759DF - (*(int32_t*)&v >> 1);
    const float y = *(float*)&i;
    return y * (1.5f - (v * 0.5f * y * y));
}

// Newton's Method (Iterative Refinement)
inline double sqrtFast(double v, uint16_t iterations = 1) {
    if (v == 0.0 || iterations == 0) { return 0.0; }
    double guess = v * 0.5;
    do {
        guess = (guess + v / guess) * 0.5;
    } while (--iterations > 0);
    return guess;
}

inline float sqrtFast(float v, uint16_t iterations = 1) {
    if (v == 0.0f || iterations == 0) { return 0.0f; }
    float guess = v * 0.5f;
    do {
        guess = (guess + v / guess) * 0.5f;
    } while (--iterations > 0);
    return guess;
}

inline double rsqrtFast(double v, uint16_t iterations = 1) {
    if (v == 0.0 || iterations == 0) { return 0.0; }
    return 1.0 / sqrtFast(v, iterations);
}

inline float rsqrtFast(float v, uint16_t iterations = 1) {
    if (v == 0.0 || iterations == 0) { return 0.0; }
    return 1.0 / sqrtFast(v, iterations);
}


/////////////////////////////////////////////////////////
// Comparison

bool isEqual(ldouble lhs, ldouble rhs)              { return abs(lhs - rhs) <= Consts::ldbl::Epsilon; }
bool isEqual(double lhs, double rhs)                { return abs(lhs - rhs) <= Consts::dbl::Epsilon; }
bool isEqual(float lhs, float rhs)                  { return abs(lhs - rhs) <= Consts::flt::Epsilon; }

NUMERIC_TEMPLATE min(T value, T minValue)           { return value < minValue ? minValue : value; }
NUMERIC_TEMPLATE max(T value, T maxValue)           { return value > maxValue ? maxValue : value; }


/////////////////////////////////////////////////////////
// Signs

NUMERIC_TEMPLATE abs(T value)                       { return std::abs(value); }
NUMERIC_TEMPLATE_RETURN_TYPE(int) sign(T value)     { return static_cast<int>((val > static_cast<T>(0)) - (val < static_cast<T>(0))); }


/////////////////////////////////////////////////////////
// Logarithm

NUMERIC_TEMPLATE log(T value)                       { return std::abs(value); }
inline double log(double value)                     { return static_cast<double>(log(static_cast<ldouble>(value))); }

NUMERIC_TEMPLATE log10(T value)                     { return std::log10(value); }
inline double log10(double value)                   { return static_cast<double>(log10(static_cast<ldouble>(value))); }


/////////////////////////////////////////////////////////
// Exponentials

NUMERIC_TEMPLATE pow(T value, T power)              { return std::pow(value, power); }
inline double pow(double value, double power)       { return static_cast<double>(pow(static_cast<ldouble>(value), static_cast<ldouble>(power))); }

NUMERIC_TEMPLATE exp(T value)                       { return static_cast<T>(std::exp(value)); }

NUMERIC_TEMPLATE sqr(T value)                       { return value * value; }
NUMERIC_TEMPLATE cube(T value)                      { return value * value * value; }
NUMERIC_TEMPLATE quad(T value)                      { return value * value * value * value; }
NUMERIC_TEMPLATE quint(T value)                     { return value * value * value * value * value; }

NUMERIC_TEMPLATE sqrt(T value)                      { return std::sqrt(value); }

NUMERIC_TEMPLATE rsqrt(T value)                     { return static_cast<T>(1.0) / std::sqrt(value); }


/////////////////////////////////////////////////////////
// Clamping

NUMERIC_TEMPLATE clamp(T value, T minValue, T maxValue) {
    return (value < minValue) ? minValue
         : (value > maxValue) ? maxValue
         : value;
}
inline double saturate(double value)                { return clamp(value, 0.0, 1.0); }
inline float saturate(float value)                  { return clamp(value, 0.0f, 1.0f); }


/////////////////////////////////////////////////////////
// Rounding

NUMERIC_TEMPLATE floor(T value)                     { return std::floor(value); }
NUMERIC_TEMPLATE round(T value)                     { return std::round(value); }
NUMERIC_TEMPLATE ceil(T value)                      { return std::ceil(value); }

inline ldouble frac(ldouble value)                  { return value - floor(value); }
inline double frac(double value)                    { return value - floor(value); }
inline float frac(float value)                      { return value - floor(value); }


/////////////////////////////////////////////////////////
// Conversions

NUMERIC_TEMPLATE lerp(T from, T to, float t)    { return from + (to - from) * t; }
NUMERIC_TEMPLATE remap(T v, T old1, T old2, T new1, T new2) {
    return new1 + (v - old1) * (new2 - new1) / (old2 - old1);
}
NUMERIC_TEMPLATE remapClamped(T v, T old1, T old2, T new1, T new2) {
    return clamp(remap(v, old1, old2, new1, new2), min(new1, new2), max(new1, new2));
}

NUMERIC_TEMPLATE toRadians(T degrees)               { return static_cast<T>(static_cast<ldouble>(degrees) * Consts::ldbl::DegToRad); }
NUMERIC_TEMPLATE toDegrees(T radians)               { return static_cast<T>(static_cast<ldouble>(radians) * Consts::ldbl::RadToDeg); }


/////////////////////////////////////////////////////////
// Trigonometry

NUMERIC_TEMPLATE sin(T radians)                     { return std::sin(radians); }
inline double sin(double radians)                   { return static_cast<double>(sin(static_cast<ldouble>(radians))); }
NUMERIC_TEMPLATE cos(T radians)                     { return std::cos(radians); }
inline double cos(double radians)                   { return static_cast<double>(cos(static_cast<ldouble>(radians))); }
NUMERIC_TEMPLATE tan(T radians)                     { return std::tan(radians); }
inline double tan(double radians)                   { return static_cast<double>(tan(static_cast<ldouble>(radians))); }

NUMERIC_TEMPLATE asin(T value)                      { return std::asin(value); }
inline double asin(double value)                    { return static_cast<double>(asin(static_cast<ldouble>(value))); }
NUMERIC_TEMPLATE acos(T value)                      { return std::acos(value); }
inline double acos(double value)                    { return static_cast<double>(acos(static_cast<ldouble>(value))); }
NUMERIC_TEMPLATE atan(T value)                      { return std::atan(value); }
inline double atan(double value)                    { return static_cast<double>(atan(static_cast<ldouble>(value))); }

NUMERIC_TEMPLATE atan2(T y, T x)                    { return std::atan2(y, x); }

/////////////////////////////////////////////////////////
// Modulo

NUMERIC_TEMPLATE mod(T value, T divisor)            { return divisor != 0 ? value % divisor : 0; }
inline ldouble mod(ldouble value, ldouble divisor)  { return divisor != 0.0L ? std::fmod(value, divisor) : 0.0L; }
inline double mod(double value, double divisor)     { return divisor != 0.0 ? std::fmod(value, divisor) : 0.0; }
inline float mod(float value, float divisor)        { return divisor != 0.0f ? std::fmod(value, divisor) : 0.0f; }


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