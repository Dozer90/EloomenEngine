#pragma once

#include <EASTL/type_traits.h>

namespace eloo {
template<typename T> concept Numeric = eastl::is_arithmetic_v<T>;
template<typename T> concept Integral = eastl::is_integral_v<T>;
template<typename T> concept FloatingPoint = eastl::is_floating_point_v<T>;
}

#define TEMPLATE_ALLOW_IF_T_MEETS_CONDITION(T, _cond) \
    template<typename T, typename = eastl::enable_if_t<eastl::_cond<T>::value>>
#define TEMPLATE_ALLOW_IF_T_MEETS_BOTH_CONDITIONS(T, _cond1, _cond2) \
    template<typename T, typename = eastl::enable_if_t<eastl::_cond1<T>::value && eastl::_cond2<T>::value>>
#define TEMPLATE_ALLOW_IF_T_IS_OF_TYPE_AND_SIZE(T, _type, _size) \
    template<typename T, typename = eastl::enable_if_t<eastl::is_same<T, _type##_size##_t>>>

#define TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut) \
    template<typename TOther, typename TOut = eastl::common_type_t<T, TOther>, typename = eastl::enable_if_t<eastl::is_arithmetic<TOther>::value>>

#define ARITHMETIC_TEMPLATE(T)                  TEMPLATE_ALLOW_IF_T_MEETS_CONDITION(T, is_arithmetic)
#define FLOATING_POINT_TEMPLATE(T)              TEMPLATE_ALLOW_IF_T_MEETS_CONDITION(T, is_floating_point)
#define INTEGRAL_TEMPLATE(T)                    TEMPLATE_ALLOW_IF_T_MEETS_CONDITION(T, is_integral)
#define SIGNED_INTEGRAL_TEMPLATE(T)             TEMPLATE_ALLOW_IF_T_MEETS_BOTH_CONDITIONS(T, is_integral, is_signed)
#define UNSIGNED_INTEGRAL_TEMPLATE(T)           TEMPLATE_ALLOW_IF_T_MEETS_BOTH_CONDITIONS(T, is_integral, is_unsigned)
#define INTEGRAL_SIZE_TEMPLATE(T, _size)        TEMPLATE_ALLOW_IF_T_IS_OF_TYPE_AND_SIZE(T, int, _size)
#define FLOATING_POINT_SIZE_TEMPLATE(T, _size)  TEMPLATE_ALLOW_IF_T_IS_OF_TYPE_AND_SIZE(T, float, _size)

#define ARITHMETIC_TEMPLATE_T                   ARITHMETIC_TEMPLATE(T)
#define FLOATING_POINT_TEMPLATE_T               FLOATING_POINT_TEMPLATE(T)
#define INTEGRAL_TEMPLATE_T                     INTEGRAL_TEMPLATE(T)
#define SIGNED_INTEGRAL_TEMPLATE_T              SIGNED_INTEGRAL_TEMPLATE(T)
#define UNSIGNED_INTEGRAL_TEMPLATE_T            UNSIGNED_INTEGRAL_TEMPLATE(T)
#define INTEGRAL_SIZE_TEMPLATE_T(_size)         INTEGRAL_SIZE_TEMPLATE(T, _size)
#define FLOATING_POINT_SIZE_TEMPLATE_T(_size)   FLOATING_POINT_SIZE_TEMPLATE(T, _size)


namespace eloo {
class half {
public:
    half() = default;
    constexpr inline half(float f) : mBits(floatToHalf(f)) {}

    inline operator float () const { return halfToFloat(mBits); }

    inline half& operator = (float f) {
        mBits = floatToHalf(f);
        return *this;
    }

    constexpr static uint16_t floatToHalf(float f) {
        uint32_t fui = *reinterpret_cast<uint32_t*>(&f);
        uint32_t sign = (fui >> 16) & 0x8000;
        uint32_t exponent = ((fui >> 23) & 0xFF) - 0x70;
        uint32_t mantissa = (fui & 0x007FFFFF) >> 13;

        if (exponent <= 0) {
            // Subnormal numbers (denormals)
            return sign | (mantissa >> (1 - exponent));
        }
        if (exponent >= 31) {
            // Infinity/NaN
            return sign | 0x7C00 | (mantissa ? 1 : 0);
        }
        return sign | (exponent << 10) | mantissa;
    }

    constexpr static float halfToFloat(uint16_t h) {
        uint32_t fui = 0;
        uint32_t sign = (h & 0x8000) << 16;
        uint32_t exponent = (h & 0x7C00) >> 10;
        uint32_t mantissa = h & 0x03FF;

        if (exponent == 31) {
            // Infinity/NaN
            fui = sign | 0x7F800000 | (mantissa << 13);
        } else if (exponent == 0) {
            // Subnormal numbers (denormals)
            if (mantissa == 0) {
                // Signed zero
                fui = sign;
            } else {
                exponent = 1;
                while ((mantissa & 0x0400) == 0) {
                    mantissa <<= 1;
                    exponent--;
                }
                fui = sign | ((exponent + 0x70) << 23) | ((mantissa & 0x03FF) << 13);
            }
        } else {
            fui = sign | ((exponent + 0x70) << 23) | (mantissa << 13);
        }
        return *reinterpret_cast<float*>(&fui);
    }

private:
    uint16_t mBits = 0;
};
}

typedef double float64_t;
typedef float float32_t;
typedef eloo::half float16_t;

// Include half (float16_t) in the floating point checks
namespace eastl {
template<> struct is_floating_point<float16_t> : eastl::true_type {};
}


namespace eloo {
// floating point containers
template<Numeric T> struct vector2 {
    T x, y;

    inline constexpr vector2() : x(static_cast<T>(0)), y(static_cast<T>(0)) {}
    inline constexpr vector2(T xy) : x(xy), y(xy) {}
    inline constexpr vector2(T x, T y) : x(x), y(y) {}

    inline T* elements()                                { return &x; }
    inline const T* elements() const                    { return &x; }

    inline static constexpr vector2 zero()              { return { static_cast<T>(0) }; }
    inline static constexpr vector2 one()               { return { static_cast<T>(1) }; }
    inline static constexpr vector2 up()                { return { static_cast<T>(0), static_cast<T>(1) }; }
    inline static constexpr vector2 right()             { return { static_cast<T>(1), static_cast<T>(0) }; }

    //vector2<T>& normalizeFast(unsigned int iterationCount = 1);
    //float magnitudeFast(unsigned int iterationCount = 1) const;

#pragma region Floating point functions
    FloatingPoint<T> inline bool isNormalized() const   { return magnitudeSqr() == static_cast<T>(1); }
    FloatingPoint<T> inline T magnitude() const         { return Math::sqrt(magnitudeSqr()); }
    FloatingPoint<T> inline T magnitudeSqr() const      { return x * x + y * y; }

    FloatingPoint<T> inline vector2<T>& normalize() {
        const T magSqr = magnitudeSqr();
        if (magSqr == static_cast<T>(1)) return *this;
        if (magSqr == static_cast<T>(0)) return zero();
        const T mag = Math::sqrt(magSqr);
        x /= mag;
        y /= mag;
        return *this;
    }
#pragma endregion

#pragma region Integral functions
    Integral<T> inline vector2<T> snapToGrid(T gridSize) const {
        return { (x / gridSize) * gridSize, (y / gridSize) * gridSize };
    }

    Integral<T> inline vector2<T> clampStep(T stepSize) const {
        return { (x + stepSize / 2) / stepSize * stepSize,
                 (y + stepSize / 2) / stepSize * stepSize };
    }

    Integral<T> inline T manhattanDistance(const vector2<T>& v) const {
        return Math::abs(x - v.x) + Math::abs(y - v.y);
    }

    Integral<T> inline bool isEven() const {
        return x % static_cast<T>(2) == static_cast<T>(0)
            && y % static_cast<T>(2) == static_cast<T>(0);
    }
    Integral<T> inline bool isOdd() const   { return !isEven(); }

    Integral<T> inline T gcd() const        { return eastl::gcd(x, y); }
    Integral<T> inline T lcm() const        { return eastl::lcm(x, y); }
#pragma endregion


    //////////////////////////////////////////////////////
    // Operator overloads

    inline T& operator [] (uint8_t index)               { return elements()[index]; }
    inline const T& operator [] (uint8_t index) const   { return elements()[index]; }

    inline friend bool operator != (const vector2<T>& v1, const vector2<T>& v2) {
        return v1.x != v2.x && v1.y != v2.y;
    }
    inline friend bool operator == (const vector2<T>& v1, const vector2<T>& v2) {
        return !(v1 != v2);
    }

    inline friend vector2<T> operator - (const vector2<T>& v)   { return { -v.x, -v.y }; }
    inline vector2<T>& operator - ()                            { return (*this = -*this); }


    //     DIVISION     //////////////////////////////////

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline friend vector2<TOut> operator / (const vector2<T>& v1, const vector2<TOther>& v2) {
        return { static_cast<TOut>(v1.x) / static_cast<TOut>(v2.x),
                 static_cast<TOut>(v1.y) / static_cast<TOut>(v2.y) };
    }

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline vector2<T>& operator /= (const vector2<TOther>& v) {
        if constexpr (!eastl::is_same<T, TOut>::value) {
            #pragma message("Warning: Possible precision loss in 'vector2' /= operation.")
        }
        x /= static_cast<T>(v.x);
        y /= static_cast<T>(v.y);
        return *this;
    }

    ARITHMETIC_TEMPLATE(TOther)
    inline friend vector2<T> operator / (const vector2<T>& v, TOther o) {
        return { v.x / static_cast<T>(o),
                 v.y / static_cast<T>(o) };
    }

    ARITHMETIC_TEMPLATE(TOther) inline vector2<T>& operator /= (TOther o) { return (*this = *this / o); }


    //  MULTIPLICATION  //////////////////////////////////

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline friend vector2<TOut> operator * (const vector2<T>& v1, const vector2<TOther>& v2) {
        return { static_cast<TOut>(v1.x) * static_cast<TOut>(v2.x),
                 static_cast<TOut>(v1.y) * static_cast<TOut>(v2.y) };
    }

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline vector2<T>& operator *= (const vector2<TOther>& v) {
        if constexpr (!eastl::is_same<T, TOut>::value) {
            #pragma message("Warning: Possible precision loss in 'vector2' *= operation.")
        }
        x *= static_cast<T>(v.x);
        y *= static_cast<T>(v.y);
        return *this;
    }

    ARITHMETIC_TEMPLATE(TOther)
    inline friend vector2<T> operator * (const vector2<T>& v, TOther o) {
        return { v.x * static_cast<T>(o),
                 v.y * static_cast<T>(o) };
    }

    ARITHMETIC_TEMPLATE(TOther) inline vector2<T>& operator *= (TOther o) { return (*this = *this * o); }


    //     ADDITION     /////////////////////////////

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline friend vector2<TOut> operator + (const vector2<T>& v1, const vector2<TOther>& v2) {
        return { static_cast<TOut>(v1.x) + static_cast<TOut>(v2.x),
                 static_cast<TOut>(v1.y) + static_cast<TOut>(v2.y) };
    }

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline vector2<T>& operator += (const vector2<TOther>& v) {
        if constexpr (!eastl::is_same<T, TOut>::value) {
            #pragma message("Warning: Possible precision loss in 'vector2' += operation.")
        }
        x += static_cast<T>(v.x);
        y += static_cast<T>(v.y);
        return *this;
    }

    ARITHMETIC_TEMPLATE(TOther)
    inline friend vector2<T> operator + (const vector2<T>& v, TOther o) {
        return { v.x + static_cast<T>(o),
                 v.y + static_cast<T>(o) };
    }

    ARITHMETIC_TEMPLATE(TOther) inline vector2<T>& operator += (TOther o) { return (*this = *this + o); }
    

    //   SUBTRACTION    ///////////////////////////////

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline friend vector2<TOut> operator - (const vector2<T>& v1, const vector2<TOther>& v2) {
        return { static_cast<TOut>(v1.x) - static_cast<TOut>(v2.x),
                 static_cast<TOut>(v1.y) - static_cast<TOut>(v2.y) };
    }

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline vector2<T>& operator -= (const vector2<TOther>& v) {
        if constexpr (!eastl::is_same<T, TOut>::value) {
            #pragma message("Warning: Possible precision loss in 'vector2' -= operation.")
        }
        x -= static_cast<T>(v.x);
        y -= static_cast<T>(v.y);
        return *this;
    }

    ARITHMETIC_TEMPLATE(TOther)
    inline friend vector2<T> operator - (const vector2<T>& v, TOther o) {
        return { v.x - static_cast<T>(o),
                 v.y - static_cast<T>(o) };
    }

    ARITHMETIC_TEMPLATE(TOther) inline vector2<T>& operator -= (TOther o) { return (*this = *this - o); }


    //     MODULO       ///////////////////////////////

    Integral<T> vector2<T> operator % (const vector2<T>& v) const { return { x % v.x, y % v.y }; }
    Integral<T> vector2<T> operator & (const vector2<T>& v) const { return { x & v.x, y & v.y }; }
    Integral<T> vector2<T> operator | (const vector2<T>& v) const { return { x | v.x, y | v.y }; }
    Integral<T> vector2<T> operator ^ (const vector2<T>& v) const { return { x ^ v.x, y ^ v.y }; }
    Integral<T> vector2<T> operator << (const int shift) const    { return { x << shift, y << shift }; }
    Integral<T> vector2<T> operator >> (const int shift) const    { return { x >> shift, y >> shift }; }
};

using int2 = vector2<int>;
using half2 = vector2<half>;
using float2 = vector2<float>;
}