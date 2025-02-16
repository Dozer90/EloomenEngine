#pragma once

using ldouble = long double;

namespace eloo::Math::Consts {
namespace ldbl {
constexpr ldouble e                     = 2.71828182845904523536028747135266249775724709369995L;
constexpr ldouble log2e                 = 1.44269504088896340735992468100189213742664595415299L;
constexpr ldouble log10e                = 0.43429448190325182765112891891660508229439700580366L;
constexpr ldouble ln2                   = 0.69314718055994530941723212145817656807550013436025L;
constexpr ldouble ln10                  = 2.30258509299404568401799145468436420760110148862870L;
constexpr ldouble Sqrt2                 = 1.41421356237309504880168872420969807856967187537695L;
constexpr ldouble Sqrt3                 = 1.73205080756887729352744634150587236694280525381038L;
constexpr ldouble Sqrt5                 = 2.23606797749978969640917366873127623544061835961153L;
constexpr ldouble RSqrt2                = 1.0L / Sqrt2;
constexpr ldouble RSqrt3                = 1.0L / Sqrt3;
constexpr ldouble RSqrt5                = 1.0L / Sqrt5;
constexpr ldouble PHI                   = 1.61803398874989484820458683436563811772030917980576L;
constexpr ldouble GoldenRatio           = PHI;
constexpr ldouble GoldenRatioConjugate  = 0.61803398874989484820458683436563811772030917980576L;
constexpr ldouble EulerMascheroni       = 0.57721566490153286060651209008240243104215933593992L;
constexpr ldouble Catalan               = 0.915965594177219015054603514932384110774L;
constexpr ldouble Apery                 = 1.20205690315959428539973816151144999076498629234049L;
constexpr ldouble PI                    = 3.14159265358979323846264338327950288419716939937511L;
constexpr ldouble TwoPI                 = PI + PI;
constexpr ldouble Tau                   = TwoPI;
constexpr ldouble TwoThirdTau           = Tau / 3.0L;
constexpr ldouble HalfPI                = PI * 0.5L;
constexpr ldouble QuarterPI             = PI * 0.25L;
constexpr ldouble DegToRad              = PI / 180.0L;
constexpr ldouble RadToDeg              = 180.0L / PI;
constexpr ldouble Epsilon               = 2.2204460492503131e-016L;
} // namespace ldbl

namespace dbl {
constexpr double e                      = static_cast<double>(Consts::ldbl::e);
constexpr double log2e                  = static_cast<double>(Consts::ldbl::log2e);
constexpr double log10e                 = static_cast<double>(Consts::ldbl::log10e);
constexpr double ln2                    = static_cast<double>(Consts::ldbl::ln2);
constexpr double ln10                   = static_cast<double>(Consts::ldbl::ln10);
constexpr double Sqrt2                  = static_cast<double>(Consts::ldbl::Sqrt2);
constexpr double Sqrt3                  = static_cast<double>(Consts::ldbl::Sqrt3);
constexpr double Sqrt5                  = static_cast<double>(Consts::ldbl::Sqrt5);
constexpr double RRSqrt2                = static_cast<double>(Consts::ldbl::RSqrt2);
constexpr double RRSqrt3                = static_cast<double>(Consts::ldbl::RSqrt3);
constexpr double RRSqrt5                = static_cast<double>(Consts::ldbl::RSqrt5);
constexpr double PHI                    = static_cast<double>(Consts::ldbl::PHI);
constexpr double GoldenRatio            = static_cast<double>(Consts::ldbl::GoldenRatio);
constexpr double GoldenRatioConjugate   = static_cast<double>(Consts::ldbl::GoldenRatioConjugate);
constexpr double EulerMascheroni        = static_cast<double>(Consts::ldbl::EulerMascheroni);
constexpr double Catalan                = static_cast<double>(Consts::ldbl::Catalan);
constexpr double Apery                  = static_cast<double>(Consts::ldbl::Apery);
constexpr double PI                     = static_cast<double>(Consts::ldbl::PI);
constexpr double TwoPI                  = static_cast<double>(Consts::ldbl::TwoPI);
constexpr double Tau                    = static_cast<double>(Consts::ldbl::Tau);
constexpr double TwoThirdTau            = static_cast<double>(Consts::ldbl::TwoThirdTau);
constexpr double HalfPI                 = static_cast<double>(Consts::ldbl::HalfPI);
constexpr double QuarterPI              = static_cast<double>(Consts::ldbl::QuarterPI);
constexpr double DegToRad               = static_cast<double>(Consts::ldbl::DegToRad);
constexpr double RadToDeg               = static_cast<double>(Consts::ldbl::RadToDeg);
constexpr double Epsilon                = 2.2204460492503131e-016;
} // namespace dbl

namespace flt {
constexpr float e                       = static_cast<float>(Consts::ldbl::e);
constexpr float log2e                   = static_cast<float>(Consts::ldbl::log2e);
constexpr float log10e                  = static_cast<float>(Consts::ldbl::log10e);
constexpr float ln2                     = static_cast<float>(Consts::ldbl::ln2);
constexpr float ln10                    = static_cast<float>(Consts::ldbl::ln10);
constexpr float Sqrt2                   = static_cast<float>(Consts::ldbl::Sqrt2);
constexpr float Sqrt3                   = static_cast<float>(Consts::ldbl::Sqrt3);
constexpr float Sqrt5                   = static_cast<float>(Consts::ldbl::Sqrt5);
constexpr float RSqrt2                  = static_cast<float>(Consts::ldbl::RSqrt2);
constexpr float RSqrt3                  = static_cast<float>(Consts::ldbl::RSqrt3);
constexpr float RSqrt5                  = static_cast<float>(Consts::ldbl::RSqrt5);
constexpr float PHI                     = static_cast<float>(Consts::ldbl::PHI);
constexpr float GoldenRatio             = static_cast<float>(Consts::ldbl::GoldenRatio);
constexpr float GoldenRatioConjugate    = static_cast<float>(Consts::ldbl::GoldenRatioConjugate);
constexpr float EulerMascheroni         = static_cast<float>(Consts::ldbl::EulerMascheroni);
constexpr float Catalan                 = static_cast<float>(Consts::ldbl::Catalan);
constexpr float Apery                   = static_cast<float>(Consts::ldbl::Apery);
constexpr float PI                      = static_cast<float>(Consts::ldbl::PI);
constexpr float TwoPI                   = static_cast<float>(Consts::ldbl::TwoPI);
constexpr float Tau                     = static_cast<float>(Consts::ldbl::Tau);
constexpr float TwoThirdTau             = static_cast<float>(Consts::ldbl::TwoThirdTau);
constexpr float HalfPI                  = static_cast<float>(Consts::ldbl::HalfPI);
constexpr float QuarterPI               = static_cast<float>(Consts::ldbl::QuarterPI);
constexpr float DegToRad                = static_cast<float>(Consts::ldbl::DegToRad);
constexpr float RadToDeg                = static_cast<float>(Consts::ldbl::RadToDeg);
constexpr float Epsilon                 = 1.192092896e-07f;
} // namespace flt
} // namespace eloo::Math::Consts