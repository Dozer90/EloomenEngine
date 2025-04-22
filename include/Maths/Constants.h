#pragma once

#include <datatype/half.h>

namespace eloo::Math {
    namespace f64 {
        constexpr float64_t E                      = 2.71828182845904523536028747135266249775724709369995;
        constexpr float64_t LOG2E                  = 1.44269504088896340735992468100189213742664595415299;
        constexpr float64_t LOG10E                 = 0.43429448190325182765112891891660508229439700580366;
        constexpr float64_t LN2                    = 0.69314718055994530941723212145817656807550013436025;
        constexpr float64_t LN10                   = 2.30258509299404568401799145468436420760110148862870;
        constexpr float64_t LN10_2                 = 0.30102999566398119521373889472449302676818988146210;
        constexpr float64_t SQRT2                  = 1.41421356237309504880168872420969807856967187537695;
        constexpr float64_t SQRT3                  = 1.73205080756887729352744634150587236694280525381038;
        constexpr float64_t SQRT5                  = 2.23606797749978969640917366873127623544061835961153;
        constexpr float64_t RSQRT2                 = 1.0 / SQRT2;
        constexpr float64_t RSQRT3                 = 1.0 / SQRT3;
        constexpr float64_t RSQRT5                 = 1.0 / SQRT5;
        constexpr float64_t PHI                    = 1.61803398874989484820458683436563811772030917980576;
        constexpr float64_t GOLDEN_RATIO           = PHI;
        constexpr float64_t GOLDEN_RATIO_CONJUGATE = 0.61803398874989484820458683436563811772030917980576;
        constexpr float64_t EULER_MASCHERONI       = 0.57721566490153286060651209008240243104215933593992;
        constexpr float64_t CATALAN                = 0.915965594177219015054603514932384110774;
        constexpr float64_t APERY                  = 1.20205690315959428539973816151144999076498629234049;
        constexpr float64_t PI                     = 3.14159265358979323846264338327950288419716939937511;
        constexpr float64_t TWO_PI                 = PI + PI;
        constexpr float64_t TAU                    = TWO_PI;
        constexpr float64_t TWO_THIRD_TAU          = TAU / 3.0;
        constexpr float64_t HALF_PI                = PI * 0.5;
        constexpr float64_t QUARTER_PI             = PI * 0.25;
        constexpr float64_t DEG_TO_RAD             = PI / 180.0;
        constexpr float64_t RAD_TO_DEG             = 180.0 / PI;
        constexpr float64_t EPSILON                = 2.2204460492503131e-016;
        constexpr float64_t CLOSE_REL_TOLERANCE    = 1e-12;
        constexpr float64_t CLOSE_ABS_TOLERANCE    = 1e-15;
    }

    namespace f32 {
        constexpr float32_t E                      = static_cast<float>(f64::E);
        constexpr float32_t LOG2E                  = static_cast<float>(f64::LOG2E);
        constexpr float32_t LOG10E                 = static_cast<float>(f64::LOG10E);
        constexpr float32_t LN2                    = static_cast<float>(f64::LN2);
        constexpr float32_t LN10                   = static_cast<float>(f64::LN10);
        constexpr float32_t LN10_2                 = static_cast<float>(f64::LN10_2);
        constexpr float32_t SQRT2                  = static_cast<float>(f64::SQRT2);
        constexpr float32_t SQRT3                  = static_cast<float>(f64::SQRT3);
        constexpr float32_t SQRT5                  = static_cast<float>(f64::SQRT5);
        constexpr float32_t RSQRT2                 = static_cast<float>(f64::RSQRT2);
        constexpr float32_t RSQRT3                 = static_cast<float>(f64::RSQRT3);
        constexpr float32_t RSQRT5                 = static_cast<float>(f64::RSQRT5);
        constexpr float32_t PHI                    = static_cast<float>(f64::PHI);
        constexpr float32_t GOLDEN_RATIO           = static_cast<float>(f64::GOLDEN_RATIO);
        constexpr float32_t GOLDEN_RATIO_CONJUGATE = static_cast<float>(f64::GOLDEN_RATIO_CONJUGATE);
        constexpr float32_t EULER_MASCHERONI       = static_cast<float>(f64::EULER_MASCHERONI);
        constexpr float32_t CATALAN                = static_cast<float>(f64::CATALAN);
        constexpr float32_t APERY                  = static_cast<float>(f64::APERY);
        constexpr float32_t PI                     = static_cast<float>(f64::PI);
        constexpr float32_t TWO_PI                 = static_cast<float>(f64::TWO_PI);
        constexpr float32_t TAU                    = static_cast<float>(f64::TAU);
        constexpr float32_t TWO_THIRD_TAU          = static_cast<float>(f64::TWO_THIRD_TAU);
        constexpr float32_t HALF_PI                = static_cast<float>(f64::HALF_PI);
        constexpr float32_t QUARTER_PI             = static_cast<float>(f64::QUARTER_PI);
        constexpr float32_t DEG_TO_RAD             = static_cast<float>(f64::DEG_TO_RAD);
        constexpr float32_t RAD_TO_DEG             = static_cast<float>(f64::RAD_TO_DEG);
        constexpr float32_t EPSILON                = 1.192092896e-07f;
        constexpr float32_t CLOSE_REL_TOLERANCE    = 1e-6;
        constexpr float32_t CLOSE_ABS_TOLERANCE    = 1e-8;
    }

    namespace f16 {
        constexpr float16_t E                      = float16_t(f32::E);
        constexpr float16_t LOG2E                  = float16_t(f32::LOG2E);
        constexpr float16_t LOG10E                 = float16_t(f32::LOG10E);
        constexpr float16_t LN2                    = float16_t(f32::LN2);
        constexpr float16_t LN10                   = float16_t(f32::LN10);
        constexpr float16_t LN10_2                 = float16_t(f32::LN10_2);
        constexpr float16_t SQRT2                  = float16_t(f32::SQRT2);
        constexpr float16_t SQRT3                  = float16_t(f32::SQRT3);
        constexpr float16_t SQRT5                  = float16_t(f32::SQRT5);
        constexpr float16_t RSQRT2                 = float16_t(f32::RSQRT2);
        constexpr float16_t RSQRT3                 = float16_t(f32::RSQRT3);
        constexpr float16_t RSQRT5                 = float16_t(f32::RSQRT5);
        constexpr float16_t PHI                    = float16_t(f32::PHI);
        constexpr float16_t GOLDEN_RATIO           = float16_t(f32::GOLDEN_RATIO);
        constexpr float16_t GOLDEN_RATIO_CONJUGATE = float16_t(f32::GOLDEN_RATIO_CONJUGATE);
        constexpr float16_t EULER_MASCHERONI       = float16_t(f32::EULER_MASCHERONI);
        constexpr float16_t CATALAN                = float16_t(f32::CATALAN);
        constexpr float16_t APERY                  = float16_t(f32::APERY);
        constexpr float16_t PI                     = float16_t(f32::PI);
        constexpr float16_t TWO_PI                 = float16_t(f32::TWO_PI);
        constexpr float16_t TAU                    = float16_t(f32::TAU);
        constexpr float16_t TWO_THIRD_TAU          = float16_t(f32::TWO_THIRD_TAU);
        constexpr float16_t HALF_PI                = float16_t(f32::HALF_PI);
        constexpr float16_t QUARTER_PI             = float16_t(f32::QUARTER_PI);
        constexpr float16_t DEG_TO_RAD             = float16_t(f32::DEG_TO_RAD);
        constexpr float16_t RAD_TO_DEG             = float16_t(f32::RAD_TO_DEG);
        constexpr float16_t EPSILON                = 0.00097656_h;
        constexpr float16_t CLOSE_REL_TOLERANCE    = float16_t(1e-3);
        constexpr float16_t CLOSE_ABS_TOLERANCE    = float16_t(1e-5);
    }
} // namespace eloo::Math::Consts