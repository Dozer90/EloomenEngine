#pragma once

#include <Types/half.h>

namespace eloo::Math::Consts {
namespace f64 {
constexpr float64_t e                      = 2.71828182845904523536028747135266249775724709369995;
constexpr float64_t log2e                  = 1.44269504088896340735992468100189213742664595415299;
constexpr float64_t log10e                 = 0.43429448190325182765112891891660508229439700580366;
constexpr float64_t ln2                    = 0.69314718055994530941723212145817656807550013436025;
constexpr float64_t ln10                   = 2.30258509299404568401799145468436420760110148862870;
constexpr float64_t ln10_2                 = 0.30102999566398119521373889472449302676818988146210;
constexpr float64_t Sqrt2                  = 1.41421356237309504880168872420969807856967187537695;
constexpr float64_t Sqrt3                  = 1.73205080756887729352744634150587236694280525381038;
constexpr float64_t Sqrt5                  = 2.23606797749978969640917366873127623544061835961153;
constexpr float64_t RSqrt2                 = 1.0 / Sqrt2;
constexpr float64_t RSqrt3                 = 1.0 / Sqrt3;
constexpr float64_t RSqrt5                 = 1.0 / Sqrt5;
constexpr float64_t PHI                    = 1.61803398874989484820458683436563811772030917980576;
constexpr float64_t GoldenRatio            = PHI;
constexpr float64_t GoldenRatioConjugate   = 0.61803398874989484820458683436563811772030917980576;
constexpr float64_t EulerMascheroni        = 0.57721566490153286060651209008240243104215933593992;
constexpr float64_t Catalan                = 0.915965594177219015054603514932384110774;
constexpr float64_t Apery                  = 1.20205690315959428539973816151144999076498629234049;
constexpr float64_t PI                     = 3.14159265358979323846264338327950288419716939937511;
constexpr float64_t TwoPI                  = PI + PI;
constexpr float64_t Tau                    = TwoPI;
constexpr float64_t TwoThirdTau            = Tau / 3.0;
constexpr float64_t HalfPI                 = PI * 0.5;
constexpr float64_t QuarterPI              = PI * 0.25;
constexpr float64_t DegToRad               = PI / 180.0;
constexpr float64_t RadToDeg               = 180.0 / PI;
constexpr float64_t Epsilon                = 2.2204460492503131e-016;
}

namespace f32 {
constexpr float32_t e                       = static_cast<float>(Consts::f64::e);
constexpr float32_t log2e                   = static_cast<float>(Consts::f64::log2e);
constexpr float32_t log10e                  = static_cast<float>(Consts::f64::log10e);
constexpr float32_t ln2                     = static_cast<float>(Consts::f64::ln2);
constexpr float32_t ln10                    = static_cast<float>(Consts::f64::ln10);
constexpr float32_t Sqrt2                   = static_cast<float>(Consts::f64::Sqrt2);
constexpr float32_t Sqrt3                   = static_cast<float>(Consts::f64::Sqrt3);
constexpr float32_t Sqrt5                   = static_cast<float>(Consts::f64::Sqrt5);
constexpr float32_t RSqrt2                  = static_cast<float>(Consts::f64::RSqrt2);
constexpr float32_t RSqrt3                  = static_cast<float>(Consts::f64::RSqrt3);
constexpr float32_t RSqrt5                  = static_cast<float>(Consts::f64::RSqrt5);
constexpr float32_t PHI                     = static_cast<float>(Consts::f64::PHI);
constexpr float32_t GoldenRatio             = static_cast<float>(Consts::f64::GoldenRatio);
constexpr float32_t GoldenRatioConjugate    = static_cast<float>(Consts::f64::GoldenRatioConjugate);
constexpr float32_t EulerMascheroni         = static_cast<float>(Consts::f64::EulerMascheroni);
constexpr float32_t Catalan                 = static_cast<float>(Consts::f64::Catalan);
constexpr float32_t Apery                   = static_cast<float>(Consts::f64::Apery);
constexpr float32_t PI                      = static_cast<float>(Consts::f64::PI);
constexpr float32_t TwoPI                   = static_cast<float>(Consts::f64::TwoPI);
constexpr float32_t Tau                     = static_cast<float>(Consts::f64::Tau);
constexpr float32_t TwoThirdTau             = static_cast<float>(Consts::f64::TwoThirdTau);
constexpr float32_t HalfPI                  = static_cast<float>(Consts::f64::HalfPI);
constexpr float32_t QuarterPI               = static_cast<float>(Consts::f64::QuarterPI);
constexpr float32_t DegToRad                = static_cast<float>(Consts::f64::DegToRad);
constexpr float32_t RadToDeg                = static_cast<float>(Consts::f64::RadToDeg);
constexpr float32_t Epsilon                 = 1.192092896e-07f;
}

namespace f16 {
constexpr float16_t e                       = float16_t::float_to_half(Consts::f32::e);
constexpr float16_t log2e                   = float16_t::float_to_half(Consts::f32::log2e);
constexpr float16_t log10e                  = float16_t::float_to_half(Consts::f32::log10e);
constexpr float16_t ln2                     = float16_t::float_to_half(Consts::f32::ln2);
constexpr float16_t ln10                    = float16_t::float_to_half(Consts::f32::ln10);
constexpr float16_t Sqrt2                   = float16_t::float_to_half(Consts::f32::Sqrt2);
constexpr float16_t Sqrt3                   = float16_t::float_to_half(Consts::f32::Sqrt3);
constexpr float16_t Sqrt5                   = float16_t::float_to_half(Consts::f32::Sqrt5);
constexpr float16_t RSqrt2                  = float16_t::float_to_half(Consts::f32::RSqrt2);
constexpr float16_t RSqrt3                  = float16_t::float_to_half(Consts::f32::RSqrt3);
constexpr float16_t RSqrt5                  = float16_t::float_to_half(Consts::f32::RSqrt5);
constexpr float16_t PHI                     = float16_t::float_to_half(Consts::f32::PHI);
constexpr float16_t GoldenRatio             = float16_t::float_to_half(Consts::f32::GoldenRatio);
constexpr float16_t GoldenRatioConjugate    = float16_t::float_to_half(Consts::f32::GoldenRatioConjugate);
constexpr float16_t EulerMascheroni         = float16_t::float_to_half(Consts::f32::EulerMascheroni);
constexpr float16_t Catalan                 = float16_t::float_to_half(Consts::f32::Catalan);
constexpr float16_t Apery                   = float16_t::float_to_half(Consts::f32::Apery);
constexpr float16_t PI                      = float16_t::float_to_half(Consts::f32::PI);
constexpr float16_t TwoPI                   = float16_t::float_to_half(Consts::f32::TwoPI);
constexpr float16_t Tau                     = float16_t::float_to_half(Consts::f32::Tau);
constexpr float16_t TwoThirdTau             = float16_t::float_to_half(Consts::f32::TwoThirdTau);
constexpr float16_t HalfPI                  = float16_t::float_to_half(Consts::f32::HalfPI);
constexpr float16_t QuarterPI               = float16_t::float_to_half(Consts::f32::QuarterPI);
constexpr float16_t DegToRad                = float16_t::float_to_half(Consts::f32::DegToRad);
constexpr float16_t RadToDeg                = float16_t::float_to_half(Consts::f32::RadToDeg);
constexpr float16_t Epsilon                 = 0.00097656_h;
}
} // namespace eloo::Math::Consts