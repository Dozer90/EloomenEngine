#pragma once

#include <Maths/Constants.h>


#if 0

#pragma once

#include "Platform_BaseTypes.h"
#include "Platform_Math.h"
#include "Platform_Builtins.h"

#include "Types_BaseMath.h"

#define FLT16_MAX 0x7BFF
#define FLT16_LOWEST 0xFBFF
#define SMALL_NUMBER 1.e-4f

#include "Math_Platform.h"

#if _MSC_VER && !__INTEL_COMPILER
#define MSVC_FALLBACK_RINT
#endif
#ifdef MSVC_FALLBACK_RINT
#include <smmintrin.h>
#endif

namespace Math {

constexpr float min_fast(float a, float b) {
    // the exact ieee definition of min
    // which infers to a min cpu instruction
    return a < b ? a : b;
}
constexpr float max_fast(float a, float b) {
    // the exact ieee definition of max
    // which infers to a max cpu instruction
    return a > b ? a : b;
}
constexpr float clamp(float value, float minValue, float maxValue) {
    // this is arranged to produce the minimum assembly possible
    // maxss xmm0, xmm1
    // minss xmm0, xmm1
    // ret
    return min_fast(max_fast(value, minValue), maxValue);
}
inline int round_to_int(float f) {
    return static_cast<int>(Platform::ceil(f - 0.5f));
}

inline int float_to_int(float f) {
    return static_cast<int>(f);
};

inline uint float_to_uint(float f) {
    return (uint)(int)f;
};

FORCEINLINE float saturate(float f) {
    return clamp(f, 0.0f, 1.0f);
}
// NOTE: Casting a negative floating point to unsigned int on ARM processors will yield 0. Use [float|double]_to_int
// in order to cast to from floating point to signed int to unsigned int.
// NOTE: Casting a positive floating point greater than max signed int to signed int on ARM processors will yield max signed int.
// Reference: http://www.embeddeduse.com/2013/08/25/casting-a-negative-float-to-an-unsigned-int/
FORCEINLINE uint double_to_uint(double f) {
#if defined(__arm__) || defined(__arm64__)
    return (f > 0.0f) ? (static_cast<uint>(f)) : (static_cast<uint>(static_cast<int>(f)));
#else
    return static_cast<uint>(f);
#endif // defined(__arm__) || defined(__arm64__)
};

template<typename T, typename = typename std::enable_if<!std::is_integral<T>::value>::type>
FORCEINLINE T RadiansToDegrees(T radians) {
    return radians * (180.0f / M_PIf);
}

template<typename T, typename = typename std::enable_if<!std::is_integral<T>::value>::type>
FORCEINLINE T DegreesToRadians(T degrees) {
    return degrees * (M_PIf / 180.0f);
}

// Is the value a power of two?
FORCEINLINE uint IsPow2(uint v) // (blame Nat D. for bugs)
{
    return v && !(v & (v - 1));
}

FORCEINLINE uint log2u(uint v) {
    unsigned int r;
    unsigned int shift;

    r = (v > 0xFFFF) << 4; v >>= r;
    shift = (v > 0xFF) << 3; v >>= shift; r |= shift;
    shift = (v > 0xF) << 2; v >>= shift; r |= shift;
    shift = (v > 0x3) << 1; v >>= shift; r |= shift;
    r |= (v >> 1);

    return r;
}

FORCEINLINE constexpr uint powu(uint base, uint exp) {
    uint result = 1;
    while (exp) {
        if (exp & 1) result *= base;

        exp >>= 1;
        base *= base;
    }
    return result;
}

// Log base-2        log2(x) = ln(x)/ln(2) and 1/ln(2) ==log2(e)
FORCEINLINE double log2(double x) { return Platform::log(x) * M_LOG2E; }

//Integer square root.  Slower than hardware FP square root but works with large numbers.
FORCEINLINE unsigned int sqrtu(unsigned long n) {
    unsigned int c = 0x8000;
    unsigned int g = 0x8000;

    for (;;) {
        if (g * g > n)
            g ^= c;
        c >>= 1;
        if (c == 0)
            return g;
        g |= c;
    }
}


//Helper functions to Math::round
FORCEINLINE float round(float x) {
    // MSVC 2019 refuses to inline rintf, which would inline to /one/ instruction
    // So instead we manually do rintf. This isn't optimal because mvsc struggles with
    // intrinsics, it interjects some unnecessary movs and xors, but it is better than a function call.
#ifdef MSVC_FALLBACK_RINT
    __m128 xm = _mm_set_ss(x);
    return _mm_cvtss_f32(_mm_round_ss(xm, xm, _MM_FROUND_CUR_DIRECTION));
#else
    return Platform::rint(x);
#endif
}

//double round( double x );
FORCEINLINE float floor(float x) { return Platform::floor(x); }
FORCEINLINE float ceil(float x) { return Platform::ceil(x); }

inline int floor_to_int(float x) {
    return Math::float_to_int(Math::floor(x));
}
inline int ceil_to_int(float x) {
    return Math::float_to_int(Math::ceil(x));
}
FORCEINLINE float int_to_float(int pInt) {
    return (float)pInt;
};


//Function to linearly interpolate between a and b by 1.f - val.
inline float lerp(float a, float b, float val) {
    return b * val + (1.f - val) * a;
};

constexpr int clamp_nobranch(int a, int lo, int hi) {
    lo -= a, hi -= a;
    return a + (lo & (lo < 0) - 1) + (hi & (hi > 0) - 1);
};

//returns fractional portion of a float
inline float frac(float a) {
    return a - floor(a);
}


// Find the highest order bit set in a number (e.g. 0=>0, 1=>1, 2,3=>2, etc.)
// NOTE: this one is weird, uses 1-based bit indices since it returns 0 if no bits are set
constexpr uint highest_bit(uint uiInt) {
    uint i = 0;
    while (uiInt > 0) {
        uiInt = uiInt >> 1;
        i++;
    }
    return i;
}

// Find the lowest order bit set in a number. Given 32 bit uints, returns 32 if no bits are set.
// Ex.	00000000000000000000000000000000b -> 32
//		00000000000000000000000111111111b -> 0
//		10000000000111000000110000100000b -> 5
//		00000001000000000000000000000000b -> 24
constexpr uint lowest_bit(uint uiInt) {
    uint i = 0;
    while (uiInt) {
        uiInt = uiInt << 1;
        ++i;
    }
    // Would be more correct to use CHAR_BIT instead of 8, but I didn't want to include climits.
    return 8 * sizeof(uint) - i;
}

/**
 * @brief Sets a bit to a given value
 * @param input The input value to modify
 * @param position The position of the bit, lowest to highest
 * @param value The bit value to set. 0-1
 * @return A new value with a bit set to what you gave it. Magical, huh?
*/
template<typename I>
[[nodiscard]] constexpr I set_bit(I input, const uint8 position, const bool value) {
    static_assert(std::is_integral_v<I>, "The input must be an integral type");
    return (input & ~(1 << position)) | (value << position);
}


// Round 'size' up to the next multiple of N.  N must be a power of 2
constexpr uint RoundUpPow2(uint size, uint N) {
    return (((size)+((N)-1)) & (~((N)-1)));
}
constexpr uint64 RoundUpPow2(uint64 size, uint64 N) {
    return (((size)+((N)-1)) & (~((N)-1)));
}
#if defined(_MACOS)
constexpr uint64 RoundUpPow2(size_t size, size_t N) {
    return RoundUpPow2((uint64)size, (uint64)N);
}
#endif

constexpr uint32 RoundUpNonPow2(uint32 uSize, uint32 N) {
    return (uSize + N - 1) - (uSize + N - 1) % N;
}

// Round 'size' down to next lowest power of 2
constexpr uint RoundDownPow2(uint size) {
    // NOTE; highest_bit does not do what you might have thought it does.   It returns one-based indices, with 0 indicating "no bits"
    uint mask = (1 << (highest_bit(size)));
    return size & (mask >> 1);
}


// Round 'size' up to the next multiple of N. Use RoundUpPow2 if N is a power of 2 as it should be faster.
constexpr uint RoundUpNextMultiple(uint size, uint N) {
    return ((size + N - 1) / N) * N;
}

//-------------------------------------------------------------------------------
// FP16 functions.  Our FP16 conversion follows IEEE 754-2008, which is what DX10+ uses for FP16.
//
// FP32To16 functions DO NOT support FP16 denorm modes - denorm flushes to zero.  This is for 
// simplicity/speed.
//
// FP16To32 functions DO support denorm FP16 input data (to ensure we can correctly convert 
// FP16 data coming from external sources).
//-------------------------------------------------------------------------------
word FP32To16(float f);

float FP16To32(word w);

void FP32To16Array(word* pOut, const float* pIn, uint nFloats);

void FP16To32Array(float* pOut, const word* pIn, uint nFloats);


//Returns the max of two fp16 values
word FP16Max(word a, word b);

//Returns the min of two fp16 values
word FP16Min(word a, word b);

//Return true if a > b
bool FP16IsGreater(word a, word b);

bool FP16IsEqual(word a, word b);

bool FP16IsEqualArray(const word* pa, const word* pb, uint nSize);


/// Convert a normalized float (0:1) to N-bit fixed point (0:(2^N)-1)
template< int BITS >
FORCEINLINE uint ToFixed(float v) {
    return static_cast<uint>(Math::round_to_int(v * ((1 << BITS) - 1)));
}

/// Converts an N-bit integer (0:(2^n)-1) to a normalized float (0:1)
template< int BITS >
FORCEINLINE float FromFixed(uint n) {
    return (n & ((1 << BITS) - 1)) * (1.0f / ((1 << BITS) - 1));
}


//http://en.wikipedia.org/wiki/Gompertz_curve
FORCEINLINE float Gompertz(float fA, float fB, float fC, float fX) {
    return fA * Platform::exp(fB * Platform::exp(fC * fX));
}


FORCEINLINE float Gaussian(float fA, float fB, float fC, float fX) {
    return fA * Platform::exp(-(fX - fB) * (fX - fB) / (2.f * fC * fC));
}
FORCEINLINE float GaussianDerivative(float fA, float fB, float fC, float fX) {
    return (fB - fX) * Gaussian(fA, fB, fC, fX) / (fC * fC);
}

FORCEINLINE float Gaussian(float x, float sigma) {
    float a = Platform::abs(x) / sigma;
    a *= a;
    return (1.f / (sigma * sqrt(2.f * M_PIf))) * exp(-0.5f * a);
}

//circularly symmetric gaussian function.  Smaller fSigma will produce narrower gaussian shape.
inline float Gaussian2D(float fSigma, float2 pos) {
    float fInvSigma2 = 1.f / (fSigma * fSigma);
    float fExp = -(pos.x * pos.x + pos.y * pos.y) * fInvSigma2 * 0.5f;
    return (1.f / M_2PIf) * Platform::exp(fExp) * fInvSigma2;
}

//Directional derivative along x-axis.
inline float Gaussian2DDerivativeX(float fSigma, float2 pos) {
    float fInvSigma2 = 1.f / (fSigma * fSigma);
    float fExp = -(pos.x * pos.x + pos.y * pos.y) * fInvSigma2 * 0.5f;
    return (pos.x / M_2PIf) * Platform::exp(fExp) * fInvSigma2 * fInvSigma2;
}

//Normalized gaussian with fE = expected value and fV = sqrt( variance )
FORCEINLINE float Gaussian(float fE, float fV, float fX) {
    return Gaussian(1.f / (fV * Platform::sqrt(2.f * M_PIf)), fE, fV, fX);
}

// Test for value that is near specified float (due to floating point imprecision)
inline bool approx(float val, float about, float range) {
    return ((Platform::abs(val - about) < range));
}

/// Gives the percentage of how far along Value is from $min to $max.
FORCEINLINE float GetRangePercentage(const float value, const float min, const float max) {
    float divisor = max - min;
    if (approx(divisor, 0.f, SMALL_NUMBER)) {
        return (value >= max) ? 1.f : 0.f;
    }

    return (value - min) / divisor;
}

FORCEINLINE float GetMappedToRange(const float value, const float2 inRange, const float2 outRange) {
    return lerp(outRange.x, outRange.y, GetRangePercentage(value, inRange.x, inRange.y));
}

FORCEINLINE float GetMappedToRangeClamped(const float value, const float2 inRange, const float2 outRange) {
    return lerp(outRange.x, outRange.y, Platform::Clamp(GetRangePercentage(value, inRange.x, inRange.y), 0.f, 1.f));
}

FORCEINLINE float ExpDecay(float a, float b, float decay, float deltaTime) {
    return b + (a - b) * Platform::exp(-decay * deltaTime);
}

// Produces a decay rate from seconds that the ExpDecay method can use.
FORCEINLINE float GetExpDecayRate(float durationInSeconds) {
    return Platform::log(2.0f) / durationInSeconds;
}

// Interpolate while easing in and out at the limits.
// graph: http://wiki.unity3d.com/index.php?title=Mathfx
inline float hermite(float start, float end, float value) {
    return lerp(start, end, value * value * (3.0f - 2.0f * value));
}

//Classic smoothstep
inline float smoothstep(float a, float b, float x) {
    // Scale, bias and saturate x to 0..1 range
    x = clamp((x - a) / (b - a), 0.0, 1.0);
    // Evaluate polynomial
    return x * x * (3 - 2 * x);
}
inline float smoothstep_norm(float x) {
    // Evaluate polynomial
    return x * x * (3 - 2 * x);
}

//Smooth step with C2 continuous.
inline float smootherstep(float a, float b, float x) {
    // Scale, and clamp x to 0..1 range
    x = clamp((x - a) / (b - a), 0.0, 1.0);
    // Evaluate polynomial
    return x * x * x * (x * (x * 6 - 15) + 10);
}
inline float smootherstep_norm(float x) {
    // Evaluate polynomial
    return x * x * x * (x * (x * 6 - 15) + 10);
}

inline float bias(float x, float fBias) {
    return x / ((1.f / fBias - 2.f) * (1.f - x) + 1.f);
}

inline float gain(float x, float fGain) {
    if (x < 0.5f) {
        return bias(x * 2.f, fGain) * 0.5f;
    } else {
        return bias(x * 2.f - 1.f, 1.f - fGain) / 2.f + 0.5f;
    }
}


// Boing-like interpolation
// Will first overshoot, then waver back and forth around the end value before
// coming to a rest.
// graph: http://wiki.unity3d.com/index.php?title=Mathfx
inline float berp(float start, float end, float value) {
    value = clamp(value, 0, 1);

    value = (Platform::sin(value * M_PIf * (0.2f + 2.5f * value * value * value)) * Platform::pow(1.0f - value, 2.2f) + value) *
        (1.0f + (1.2f * (1.0f - value)));

    return start + (end - start) * value;
}

// Easing functions
// These are meant to be fast functions that given an input from 0 to 1, will return a value from 0 to 1 but with different characteristics

//SmoothStop
//This function eases off towards the end by a power function
inline float smoothstop(float x) {
    x = clamp(x, 0, 1);

    return 1 - ((1 - x) * (1 - x));
}

inline float smoothstop(float start, float end, float x) {
    x = clamp(x, 0, 1);
    x = 1 - ((1 - x) * (1 - x));

    return start + (end - start) * x;
}

inline float smoothstop3(float x) {
    x = clamp(x, 0, 1);

    return 1 - ((1 - x) * (1 - x) * (1 - x));
}

inline float smoothstop3(float start, float end, float x) {
    x = clamp(x, 0, 1);
    x = 1 - ((1 - x) * (1 - x) * (1 - x));

    return x;
}

inline float smoothstop4(float x) {
    x = clamp(x, 0, 1);

    return 1 - ((1 - x) * (1 - x) * (1 - x) * (1 - x));
}

inline float smoothstop4(float start, float end, float x) {
    x = clamp(x, 0, 1);
    x = 1 - ((1 - x) * (1 - x) * (1 - x) * (1 - x));

    return x;
}


// Returns sign of... thing.
// NOTE: +/-0 return 0
template<typename T>
inline int sign(T val) {
    return (val > T(0)) - (val < T(0));
}

// Modulus that always returns a positive remainder
// 0 <= remainder < divisor
// https://en.wikipedia.org/wiki/Modulo
// TODO try 'floored division' where the remainder has the same sign as the divisor, promoted by Donald Knuth
inline int euclidian_modulus(int val, uint divisor) {
    // operator maintains the invariant '(a/b)*b + a%b shall equal a'
    const int result = val % int(divisor);
    return (result >= 0) ? result : result + int(divisor);
}
inline int euclidian_modulus(int val, int divisor) // signed divisor has an extra abs
{
    const int result = val % divisor;
    return (result >= 0) ? result : result + Platform::abs(divisor);
}

//Evaluate a hermite spline consisting of nCP control points.  Hermite is defined by 3 scalar arrays.  They are:
// pT = knot vector, length nCP. Must be strictly increasing, so N > M implies that pT[N] > pT[M].  Spline is evaluated 
//      between pT[0] and pT[nCP-1].  If fT < pT[0] the value returned is pY[0], and if fT > pT[nCP-1] the 
//      value returned is pY[nCP-1].
// pY = Spline value at each knot point, length nCP.  These are interpolated.
// pD = Tangent values for each knot point, length nCP.  These are also interpolated.
[[deprecated("Use Math::Spline")]] float eval_hermite(float fT, const float* pT, const float* pY, const float* pD, uint nCP);


// Homogeneous cubic equation solver from Jim Blinn. Returns the number of roots ( either 1 or 3), 
// with the actual root values stored in aX_out and aW_out.  Rare double-root case is handled by the
// 3-root path for simplicity, in which case two of the roots returned will be the same. 
//
// Cubic equation is expected to be in this homogeneous form (bi-variate in x,w):
//   Ax^3 + 3Bx^2w + 3Cxw^2 + Dw^3 = 0
//
// This is supposed to be the most numerically stable root finder around, although it is not the 
// cheapest.  This should be considered a reference solution since it can often be simplified/optimized for
// specific situations.
//https://courses.cs.washington.edu/courses/cse590b/13au/lecture_notes/

uint solve_cubic_equation_homogeneous(float aX_out[3], float aW_out[3], float A, float B, float C, float D);


//Same as above, but non-homogeneous splines of form (single-variate in x):
//   Ax^3 + Bx^2 + Cx + D = 0

uint solve_cubic_equation(float aX[3], float A, float B, float C, float D);


}


#endif