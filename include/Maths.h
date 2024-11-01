#pragma once

#include <EASTL/type_traits.h>

#include <cmath>

namespace TankWarz
{

/// <summary>
/// Simple class designed to make dealing with square root values easier. When accessed,
/// the squared value of whatever was assigned is returned. To get the root value, use
/// "root_value".
/// </summary>
/// <typeparam name="T">Numerical type that supports sqrt</typeparam>
template <typename T>
class sqr_t
{
	static_assert(
		eastl::is_integral<T>::value ||
		(eastl::is_floating_point<T>::value && eastl::type_not_equal<double, T>::value),
		"Data type T is not supported by sqrt");

public:
	inline sqr_t() = default;
	inline sqr_t(const sqr_t<T>& other) { *this = other; }
	inline sqr_t(sqr_t<T>&& other) { *this = other; }
	inline sqr_t(const T& other) { *this = other; }
	inline sqr_t(T&& other) { *this = other; }

	// This allows the value to be read as the square value
	inline operator T() const { return root_value * root_value; }

	inline sqr_t& operator=(const T& value) { this->root_value = value; return *this; }
	inline sqr_t& operator=(const sqr_t<T>& rhs) { this->root_value = rhs.root_value; return *this; }
	inline sqr_t& operator=(T&& value) noexcept { this->root_value = value; return *this; }
	inline sqr_t& operator=(sqr_t<T>&& rhs) noexcept { *this = eastl::move(rhs); return *this; }

	inline bool operator==(const sqr_t<T>& rhs) const { return this->root_value == rhs.root_value; }
	inline bool operator!=(const sqr_t<T>& rhs) const { return this->root_value != rhs.root_value; }

public:
	T root_value;
};

using ldouble = long double;

#define NUMERIC_TEMPLATE template<typename T> eastl::enable_if<eastl::is_arithmetic<T>::value, T>::type

namespace Math
{
#pragma region Values

// The golden ratio
constexpr ldouble PHI_ld = 1.61803398874989484820458683436563811772030917980576L;
constexpr double PHI = static_cast<double>(PHI_ld);
constexpr float PHI_f = static_cast<float>(PHI_ld);

// Square root of 2
constexpr ldouble Sqrt2_ld = 1.41421356237309504880168872420969807856967187537695L;
constexpr double Sqrt2 = static_cast<double>(Sqrt2_ld);
constexpr float Sqrt2_f = static_cast<float>(Sqrt2_ld);

// Pi
constexpr ldouble PI_ld = 3.14159265358979323846264338327950288419716939937511L;
constexpr double PI = static_cast<double>(PI_ld);
constexpr float PI_f = static_cast<float>(PI_ld);

// Tau (2x PI)
constexpr ldouble Tau_ld = PI_ld + PI_ld;
constexpr double Tau = static_cast<double>(Tau_ld);
constexpr float Tau_f = static_cast<float>(Tau_ld);
constexpr ldouble TwoPI_ld = Tau_ld;
constexpr double TwoPI = Tau;
constexpr float TwoPI_f = Tau_f;

// Half PI
constexpr ldouble HalfPI_ld = PI_ld / 2.0L;
constexpr double HalfPI = static_cast<double>(HalfPI_ld);
constexpr float HalfPI_f = static_cast<float>(HalfPI_ld);

// Quarter PI
constexpr ldouble QuarterPI_ld = PI_ld / 4.0L;
constexpr double QuarterPI = static_cast<double>(QuarterPI_ld);
constexpr float QuarterPI_f = static_cast<float>(QuarterPI_ld);

// Degrees to radians conversion
constexpr ldouble DegToRad_ld = PI_ld / 180.0L;
constexpr double DegToRad = static_cast<double>(DegToRad_ld);
constexpr float DegToRad_f = static_cast<float>(DegToRad_ld);

// Radians to degrees conversion
constexpr ldouble RadToDeg_ld = 180.0L / PI_ld;
constexpr double RadToDeg = static_cast<double>(RadToDeg_ld);
constexpr float RadToDeg_f = static_cast<float>(RadToDeg_ld);

#pragma endregion
///>=====================================================================

#pragma region Functions

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
NUMERIC_TEMPLATE clamp(T value, T minValue, T maxValue)
{
    return (value < minValue) ? minValue :
        (value > maxValue) ? maxValue :
        value;
}

NUMERIC_TEMPLATE toRadians(T degrees) { return degrees * static_cast<T>(DegToRad_ld); }
NUMERIC_TEMPLATE toDegrees(T radians) { return radians * static_cast<T>(RadToDeg_ld); }

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