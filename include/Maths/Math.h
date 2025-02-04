#pragma once

#include <EASTL/type_traits.h>

#include <cmath>
#include <cstdint>

#define NUMERIC_TEMPLATE template<typename T> eastl::enable_if<eastl::is_arithmetic<T>::value, T>::type

namespace eloo {
namespace Math {
using ldouble = long double;

namespace Consts {
namespace ld {
constexpr ldouble PHI           = 1.61803398874989484820458683436563811772030917980576L;
constexpr ldouble GoldenRatio   = PHI;
constexpr ldouble Sqrt2         = 1.41421356237309504880168872420969807856967187537695L;
constexpr ldouble PI            = 3.14159265358979323846264338327950288419716939937511L;
constexpr ldouble TwoPI         = PI + PI;
constexpr ldouble Tau           = TwoPI;
constexpr ldouble HalfPI        = PI * 0.5L;
constexpr ldouble QuarterPI     = PI * 0.25L;
constexpr ldouble DegToRad      = PI / 180.0L;
constexpr ldouble RadToDeg      = 180.0L / PI;
constexpr ldouble Epsilon       = 2.2204460492503131e-016L;
} // namespace ld

namespace d {
constexpr double PHI            = static_cast<double>(Consts::ld::PHI);
constexpr double GoldenRatio    = PHI;
constexpr double Sqrt2          = static_cast<double>(Consts::ld::Sqrt2);
constexpr double PI             = static_cast<double>(Consts::ld::PI);
constexpr double TwoPI          = static_cast<double>(Consts::ld::TwoPI);
constexpr double Tau            = TwoPI;
constexpr double HalfPI         = static_cast<double>(Consts::ld::HalfPI);
constexpr double QuarterPI      = static_cast<double>(Consts::ld::QuarterPI);
constexpr double DegToRad       = static_cast<double>(Consts::ld::DegToRad);
constexpr double RadToDeg       = static_cast<double>(Consts::ld::RadToDeg);
constexpr double Epsilon        = 2.2204460492503131e-016;
} // namespace d

namespace f {
constexpr float PHI             = static_cast<float>(Consts::ld::PHI);
constexpr float GoldenRatio     = PHI;
constexpr float Sqrt2           = static_cast<float>(Consts::ld::Sqrt2);
constexpr float PI              = static_cast<float>(Consts::ld::PI);
constexpr float TwoPI           = static_cast<double>(Consts::ld::TwoPI);
constexpr float Tau             = TwoPI;
constexpr float HalfPI          = static_cast<float>(Consts::ld::HalfPI);
constexpr float QuarterPI       = static_cast<float>(Consts::ld::QuarterPI);
constexpr float DegToRad        = static_cast<float>(Consts::ld::DegToRad);
constexpr float RadToDeg        = static_cast<float>(Consts::ld::RadToDeg);
constexpr float Epsilon         = 1.192092896e-07f;
} // namespace f
} // namespace Consts

#pragma region Values

// Fast Inverse Square Root
double fastInvSqrt(double v) {
    const int64_t i = 0x5FE6EB50C7B537A9 - (*(int64_t*)&v >> 1);
    const double y = *(double*)&i;
    return y * (1.5 - (v * 0.5 * y * y));
}

float fastInvSqrt(float v) {
    const int32_t i = 0x5F3759DF - (*(int32_t*)&v >> 1);
    const float y = *(float*)&i;
    return y * (1.5f - (v * 0.5f * y * y));
}

// Newton's Method (Iterative Refinement)
double fastSqrt(double v, uint16_t itterations = 1) {
    if (v == 0.0 || itterations == 0) { return 0.0; }
    double guess = v * 0.5;
    do {
        guess = (guess + v / guess) * 0.5;
    } while (--itterations > 0);
    return guess;
}

float fastSqrt(float v, uint16_t itterations = 1) {
    if (v == 0.0f || itterations == 0) { return 0.0f; }
    float guess = v * 0.5f;
    do {
        guess = (guess + v / guess) * 0.5f;
    } while (--itterations > 0);
    return guess;
}

#pragma region Functions

bool isEqual(ldouble lhs, ldouble rhs) { return abs(lhs - rhs) <= Consts::ld::Epsilon; }
bool isEqual(double lhs, double rhs) { return abs(lhs - rhs) <= Consts::d::Epsilon; }
bool isEqual(float lhs, float rhs) { return abs(lhs - rhs) <= Consts::f::Epsilon; }
bool isZero(ldouble value) { return isEqual(value, 0.0L); }
bool isZero(double value) { return isEqual(value, 0.0); }
bool isZero(float value) { return isEqual(value, 0.0f); }

NUMERIC_TEMPLATE abs(T value) { return std::abs(value); }

NUMERIC_TEMPLATE log(T value) { return std::abs(value); }
inline double log(double value) { return static_cast<double>(log(static_cast<ldouble>(value))); }
NUMERIC_TEMPLATE log10(T value) { return std::log10(value); }
inline double log10(double value) { return static_cast<double>(log10(static_cast<ldouble>(value))); }
NUMERIC_TEMPLATE pow(T value, T power) { return std::pow(value, power); }
inline double pow(double value, double power) { return static_cast<double>(pow(static_cast<ldouble>(value), static_cast<ldouble>(power))); }

NUMERIC_TEMPLATE sqr(T value) { return value * value; }
NUMERIC_TEMPLATE cube(T value) { return value * value * value; }
NUMERIC_TEMPLATE quad(T value) { return value * value * value * value; }
NUMERIC_TEMPLATE quint(T value) { return value * value * value * value * value; }

NUMERIC_TEMPLATE min(T value, T minValue) { return value < minValue ? minValue : value; }
NUMERIC_TEMPLATE max(T value, T maxValue) { return value > maxValue ? maxValue : value; }
NUMERIC_TEMPLATE clamp(T value, T minValue, T maxValue) {
    return (value < minValue) ? minValue :
        (value > maxValue) ? maxValue :
        value;
}

NUMERIC_TEMPLATE toRadians(T degrees) { return static_cast<T>(static_cast<ldouble>(degrees) * Consts::ld::DegToRad); }
NUMERIC_TEMPLATE toDegrees(T radians) { return static_cast<T>(static_cast<ldouble>(radians) * Consts::ld::RadToDeg); }

NUMERIC_TEMPLATE sin(T radians) { return std::sin(radians); }
inline double sin(double radians) { return static_cast<double>(sin(static_cast<ldouble>(radians))); }
NUMERIC_TEMPLATE cos(T radians) { return std::cos(radians); }
inline double cos(double radians) { return static_cast<double>(cos(static_cast<ldouble>(radians))); }
NUMERIC_TEMPLATE tan(T radians) { return std::tan(radians); }
inline double tan(double radians) { return static_cast<double>(tan(static_cast<ldouble>(radians))); }

NUMERIC_TEMPLATE asin(T value) { return std::asin(value); }
inline double asin(double value) { return static_cast<double>(asin(static_cast<ldouble>(value))); }
NUMERIC_TEMPLATE acos(T value) { return std::acos(value); }
inline double acos(double value) { return static_cast<double>(acos(static_cast<ldouble>(value))); }
NUMERIC_TEMPLATE atan(T value) { return std::atan(value); }
inline double atan(double value) { return static_cast<double>(atan(static_cast<ldouble>(value))); }

NUMERIC_TEMPLATE atan2(T y, T x) { return std::atan2(y, x); }

NUMERIC_TEMPLATE mod(T value, T divisor) { return divisor != 0 ? value % divisor : 0; }
inline ldouble mod(ldouble value, ldouble divisor) { return divisor != 0.0L ? std::fmod(value, divisor) : 0.0L; }
inline double mod(double value, double divisor) { return divisor != 0.0 ? std::fmod(value, divisor) : 0.0; }
inline float mod(float value, float divisor) { return divisor != 0.0f ? std::fmod(value, divisor) : 0.0f; }
#pragma endregion
}
}