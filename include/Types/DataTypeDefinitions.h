#pragma once

#include <EASTL/type_traits.h>

namespace eloo {
template<typename T> concept IsArithmetic    = eastl::is_arithmetic_v<T>;
template<typename T> concept IsIntegral      = eastl::is_integral_v<T>;
template<typename T> concept IsFloatingPoint = eastl::is_floating_point_v<T>;
}

namespace eloo {
// floating point containers
template<typename T> requires IsArithmetic<T> struct vector2_t {
    T x, y;

    inline constexpr vector2_t() : x(static_cast<T>(0)), y(static_cast<T>(0)) {}
    inline constexpr vector2_t(T xy) : x(xy), y(xy) {}
    inline constexpr vector2_t(T x, T y) : x(x), y(y) {}

    inline T* elements()                                { return &x; }
    inline const T* elements() const                    { return &x; }

    inline static constexpr vector2_t<T> zero()         { return { static_cast<T>(0) }; }
    inline static constexpr vector2_t<T> one()          { return { static_cast<T>(1) }; }
    inline static constexpr vector2_t<T> up()           { return { static_cast<T>(0), static_cast<T>(1) }; }
    inline static constexpr vector2_t<T> right()        { return { static_cast<T>(1), static_cast<T>(0) }; }

    //vector2_t<T>& normalizeFast(unsigned int iterationCount = 1);
    //float magnitudeFast(unsigned int iterationCount = 1) const;

#pragma region Floating point functions
    FloatingPoint<T> inline bool isNormalized() const   { return magnitudeSqr() == static_cast<T>(1); }
    FloatingPoint<T> inline T magnitude() const         { return Math::sqrt(magnitudeSqr()); }
    FloatingPoint<T> inline T magnitudeSqr() const      { return x * x + y * y; }

    FloatingPoint<T> inline vector2_t<T>& normalize() {
        const T magSqr = magnitudeSqr();
        if (Math::is_close(magSqr, 1))) return *this;
        if (Math::is_close(magSqr, 0))) return zero();
        const T mag = Math::sqrt(magSqr);
        x /= mag;
        y /= mag;
        return *this;
    }
#pragma endregion

#pragma region Integral functions
    Integral<T> inline vector2_t<T> snapToGrid(T gridSize) const {
        return { (x / gridSize) * gridSize, (y / gridSize) * gridSize };
    }

    Integral<T> inline vector2_t<T> clampStep(T stepSize) const {
        return { (x + stepSize / 2) / stepSize * stepSize,
                 (y + stepSize / 2) / stepSize * stepSize };
    }

    Integral<T> inline T manhattanDistance(const vector2_t<T>& v) const {
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

    inline friend bool operator != (const vector2_t<T>& v1, const vector2_t<T>& v2) {
        return v1.x != v2.x && v1.y != v2.y;
    }
    inline friend bool operator == (const vector2_t<T>& v1, const vector2_t<T>& v2) {
        return !(v1 != v2);
    }

    inline friend vector2_t<T> operator - (const vector2_t<T>& v)   { return { -v.x, -v.y }; }
    inline vector2_t<T>& operator - ()                            { return (*this = -*this); }


    //     DIVISION     //////////////////////////////////

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline friend vector2_t<TOut> operator / (const vector2_t<T>& v1, const vector2_t<TOther>& v2) {
        return { static_cast<TOut>(v1.x) / static_cast<TOut>(v2.x),
                 static_cast<TOut>(v1.y) / static_cast<TOut>(v2.y) };
    }

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline vector2_t<T>& operator /= (const vector2_t<TOther>& v) {
        if constexpr (!eastl::is_same<T, TOut>::value) {
            #pragma message("Warning: Possible precision loss in 'vector2_t' /= operation.")
        }
        x /= static_cast<T>(v.x);
        y /= static_cast<T>(v.y);
        return *this;
    }

    ARITHMETIC_TEMPLATE(TOther)
    inline friend vector2_t<T> operator / (const vector2_t<T>& v, TOther o) {
        return { v.x / static_cast<T>(o),
                 v.y / static_cast<T>(o) };
    }

    ARITHMETIC_TEMPLATE(TOther) inline vector2_t<T>& operator /= (TOther o) { return (*this = *this / o); }


    //  MULTIPLICATION  //////////////////////////////////

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline friend vector2_t<TOut> operator * (const vector2_t<T>& v1, const vector2_t<TOther>& v2) {
        return { static_cast<TOut>(v1.x) * static_cast<TOut>(v2.x),
                 static_cast<TOut>(v1.y) * static_cast<TOut>(v2.y) };
    }

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline vector2_t<T>& operator *= (const vector2_t<TOther>& v) {
        if constexpr (!eastl::is_same<T, TOut>::value) {
            #pragma message("Warning: Possible precision loss in 'vector2_t' *= operation.")
        }
        x *= static_cast<T>(v.x);
        y *= static_cast<T>(v.y);
        return *this;
    }

    ARITHMETIC_TEMPLATE(TOther)
    inline friend vector2_t<T> operator * (const vector2_t<T>& v, TOther o) {
        return { v.x * static_cast<T>(o),
                 v.y * static_cast<T>(o) };
    }

    ARITHMETIC_TEMPLATE(TOther) inline vector2_t<T>& operator *= (TOther o) { return (*this = *this * o); }


    //     ADDITION     /////////////////////////////

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline friend vector2_t<TOut> operator + (const vector2_t<T>& v1, const vector2_t<TOther>& v2) {
        return { static_cast<TOut>(v1.x) + static_cast<TOut>(v2.x),
                 static_cast<TOut>(v1.y) + static_cast<TOut>(v2.y) };
    }

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline vector2_t<T>& operator += (const vector2_t<TOther>& v) {
        if constexpr (!eastl::is_same<T, TOut>::value) {
            #pragma message("Warning: Possible precision loss in 'vector2_t' += operation.")
        }
        x += static_cast<T>(v.x);
        y += static_cast<T>(v.y);
        return *this;
    }

    ARITHMETIC_TEMPLATE(TOther)
    inline friend vector2_t<T> operator + (const vector2_t<T>& v, TOther o) {
        return { v.x + static_cast<T>(o),
                 v.y + static_cast<T>(o) };
    }

    ARITHMETIC_TEMPLATE(TOther) inline vector2_t<T>& operator += (TOther o) { return (*this = *this + o); }
    

    //   SUBTRACTION    ///////////////////////////////

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline friend vector2_t<TOut> operator - (const vector2_t<T>& v1, const vector2_t<TOther>& v2) {
        return { static_cast<TOut>(v1.x) - static_cast<TOut>(v2.x),
                 static_cast<TOut>(v1.y) - static_cast<TOut>(v2.y) };
    }

    TEMPLATE_ALLOW_CROSS_TYPE_ARITHMETIC(T, TOther, TOut)
    inline vector2_t<T>& operator -= (const vector2_t<TOther>& v) {
        if constexpr (!eastl::is_same<T, TOut>::value) {
            #pragma message("Warning: Possible precision loss in 'vector2_t' -= operation.")
        }
        x -= static_cast<T>(v.x);
        y -= static_cast<T>(v.y);
        return *this;
    }

    ARITHMETIC_TEMPLATE(TOther)
    inline friend vector2_t<T> operator - (const vector2_t<T>& v, TOther o) {
        return { v.x - static_cast<T>(o),
                 v.y - static_cast<T>(o) };
    }

    ARITHMETIC_TEMPLATE(TOther) inline vector2_t<T>& operator -= (TOther o) { return (*this = *this - o); }


    //     MODULO       ///////////////////////////////

    Integral<T> vector2_t<T> operator % (const vector2_t<T>& v) const { return { x % v.x, y % v.y }; }
    Integral<T> vector2_t<T> operator & (const vector2_t<T>& v) const { return { x & v.x, y & v.y }; }
    Integral<T> vector2_t<T> operator | (const vector2_t<T>& v) const { return { x | v.x, y | v.y }; }
    Integral<T> vector2_t<T> operator ^ (const vector2_t<T>& v) const { return { x ^ v.x, y ^ v.y }; }
    Integral<T> vector2_t<T> operator << (const int shift) const    { return { x << shift, y << shift }; }
    Integral<T> vector2_t<T> operator >> (const int shift) const    { return { x >> shift, y >> shift }; }
};

using int2 = vector2_t<int>;
using half2 = vector2_t<half>;
using float2 = vector2_t<float>;
}