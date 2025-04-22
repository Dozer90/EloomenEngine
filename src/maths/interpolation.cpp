#include <Maths/CoreMath.h>

#include <Generic/defines.h>

#include <EASTL/array.h>



#define DEFINE_INTERPOLATION_TYPE(interp_t) FORCE_INLINE constexpr float interp_t(float t, eloo::Math::Interpolation::Ease ease) { \
    switch (ease) { \
        case eloo::Math::Interpolation::Ease::in:      return interp_t##_in(t); \
        case eloo::Math::Interpolation::Ease::out:     return interp_t##_out(t); \
        case eloo::Math::Interpolation::Ease::inout:   return interp_t##_in_out(t); \
        default:            ASSERT_FALSE("Attempting to use " #interp_t " interpolation with unknown ease type %i", \
                                         static_cast<int>(ease)); \
        } \
    return 0.0f; \
}

#define DEFINE_INTERPOLATION_TYPE_PARAM(interp_t, param_t, param_name, param_default_value) \
FORCE_INLINE constexpr float ##interp_t##(float t, eloo::Math::Interpolation::Ease ease, param_t param_name = param_default_value) { \
    switch (ease) { \
        case eloo::Math::Interpolation::Ease::in:      return interp_t##_in(t, ##param_name##); \
        case eloo::Math::Interpolation::Ease::out:     return interp_t##_out(t, ##param_name##); \
        case eloo::Math::Interpolation::Ease::inout:   return interp_t##_in_out(t, ##param_name##); \
        default:            ASSERT_FALSE("Attempting to use " #interp_t " interpolation with unknown ease type %i", \
                                         static_cast<int>(ease)); \
        } \
    return 0.0f; \
}

namespace {
    // Sine         //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE(sine)

    FORCE_INLINE constexpr float sine_in(float t) {
        return 1.0f - eloo::Math::cos(t * eloo::Math::Consts::f32::PI * 0.5f);
    }

    FORCE_INLINE constexpr float sine_out(float t) {
        return eloo::Math::sin(t * eloo::Math::Consts::f32::PI * 0.5f);
    }

    FORCE_INLINE constexpr float sine_in_out(float t) {
        return -(eloo::Math::cos(t * eloo::Math::Consts::f32::PI) - 1.0f) * 0.5f;
    }

    // Quad         //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE(quad)

    FORCE_INLINE constexpr float quad_in(float t) {
        return t * t;
    }

    FORCE_INLINE constexpr float quad_out(float t) {
        const float omt = 1.0f - t;
        return 1.0f - omt * omt;
    }

    FORCE_INLINE constexpr float quad_in_out(float t) {
        const float omt = 1.0f - t;
        return t < 0.5f ? 2.0f * t * t : 1.0f - 2.0f * omt * omt;
    }

    // Cubic        //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE(cubic)

    FORCE_INLINE constexpr float cubic_in(float t) {
        return t * t * t;
    }

    FORCE_INLINE constexpr float cubic_out(float t) {
        const float omt = 1.0f - t;
        return 1.0f - omt * omt * omt;
    }

    FORCE_INLINE constexpr float cubic_in_out(float t) {
        const float omt = 1.0f - t;
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - 4.0f * omt * omt * omt;
    }

    // Quart        //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE(quart)

    FORCE_INLINE constexpr float quart_in(float t) {
        return t * t * t * t;
    }

    FORCE_INLINE constexpr float quart_out(float t) {
        const float omt = 1.0f - t;
        return 1.0f - omt * omt * omt * omt;
    }

    FORCE_INLINE constexpr float quart_in_out(float t) {
        const float omt = 1.0f - t;
        return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - 8.0f * omt * omt * omt * omt;
    }

    // Quint        //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE(quint)

    FORCE_INLINE constexpr float quint_in(float t) {
        return t * t * t * t * t;
    }

    FORCE_INLINE constexpr float quint_out(float t) {
        const float omt = 1.0f - t;
        return 1.0f - omt * omt * omt * omt * omt;
    }

    FORCE_INLINE constexpr float quint_in_out(float t) {
        const float omt = 1.0f - t;
        return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - 16.0f * omt * omt * omt * omt * omt;
    }

    // Circ         //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE(circ)

    FORCE_INLINE constexpr float circ_in(float t) {
        return 1.0f - sqrt(1.0f - pow(t, 2.0f));
    }

    FORCE_INLINE constexpr float circ_out(float t) {
        return sqrt(1.0f - pow(t - 1.0f, 2.0f));
    }

    FORCE_INLINE constexpr float circ_in_out(float t) {
        t *= 2.0f;
        return (t < 1.0f ? circ_in(t) : 1.0f + circ_out(t - 1.0f)) * 0.5f;
    }

    // Expo         //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE_PARAM(expo, float, power, 10)

    FORCE_INLINE constexpr float expo_in(float t, float power = 10.0f) {
        return t == 0.0f ? 0.0f : pow(2.0f, power * t - power);
    }

    FORCE_INLINE constexpr float expo_out(float t, float power = 10.0f) {
        return t == 1.0f ? 1.0f : 1.0f - pow(2.0f, -power * t);
    }

    FORCE_INLINE constexpr float expo_in_out(float t, float power = 10.0f) {
        return t < 0.5f ? pow(2.0f * t, power) * 0.5f : 1.0f - pow(2.0f * (1.0f - t), power) * 0.5f;
    }

    // Back         //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE_PARAM(back, float, overshoot, 1.70158f)

    FORCE_INLINE constexpr float back_in(float t, float overshoot = 1.70158f) {
        const float s3 = overshoot + 1.0f;
        return s3 * t * t * t - overshoot * t * t;
    }

    FORCE_INLINE constexpr float back_out(float t, float overshoot = 1.70158f) {
        const float s3 = overshoot + 1.0f;
        const float tmo = t - 1.0f;
        return 1.0f + s3 * tmo * tmo * tmo + overshoot * tmo * tmo;
    }

    FORCE_INLINE constexpr float back_in_out(float t, float overshoot = 1.70158f) {
        t *= 2.0f;
        const float s = overshoot * 1.525f;
        return t < 1.0f
            ? 0.5f * (t * t * ((s + 1.0f) * t - s))
            : 0.5f * ((t - 2.0f) * (t - 2.0f) * ((s + 1.0f) * (t - 2.0f) + s) + 2.0f);
    }

    // Elastic      //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE(elastic)

    FORCE_INLINE constexpr float elastic_in(float t) {
        return -eloo::Math::pow(2.0f, 10.0f * (t - 1.0f)) * eloo::Math::sin(((t - 0.075f) * eloo::Math::Consts::f32::Tau) * 3.3333333f);
    }

    FORCE_INLINE constexpr float elastic_out(float t) {
        return 1.0f + eloo::Math::pow(2.0f, -10.0f * t) * eloo::Math::sin(((t - 0.075f) * eloo::Math::Consts::f32::Tau) * 3.3333333f);
    }

    FORCE_INLINE constexpr float elastic_in_out(float t) {
        if (t == 0.0f || t == 1.0f) return t;
        t *= 2.0f;
        if (t < 1.0f) return -0.5f * eloo::Math::pow(2.0f, 10.0f * (t - 1.0f)) * eloo::Math::sin(((t - 1.075f) * eloo::Math::Consts::f32::Tau) * 3.3333333f);
        return 0.5f + 0.5f * eloo::Math::pow(2.0f, -10.0f * (t - 1.0f)) * eloo::Math::sin(((t - 1.075f) * eloo::Math::Consts::f32::Tau) * 3.3333333f);
    }

    // Bounce       //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE(bounce)

    FORCE_INLINE constexpr float bounce_in(float t) {
        return 1.0f - bounce_out(1.0f - t);
    }

    FORCE_INLINE constexpr float bounce_out(float t) {
        constexpr float n1 = 7.5625f;
        constexpr float d1 = 0.363636f;
        if (t < d1) { return n1 * t * t; }
        if (t < d1 * 2.0f) { return n1 * (t - d1 * 1.5f) * (t - d1 * 1.5f) + 0.75f; }
        if (t < d1 * 2.5f) { return n1 * (t - d1 * 2.25f) * t + 0.9375f; }
        return n1 * (t - d1 * 2.625f) * t + 0.984375f;
    }

    FORCE_INLINE constexpr float bounce_in_out(float t) {
        t *= 2.0f;
        return (t < 1 ? bounce_in(t) : 1.0f + bounce_out(t - 1.0f)) * 0.5f;
    }

    // Log          //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE(logarithmic)

    FORCE_INLINE constexpr float logarithmic_in(float t) {
        return eloo::Math::log(1.0f + t * (Consts::f32::e - 1.0f));
    }

    FORCE_INLINE constexpr float logarithmic_out(float t) {
        return 1.0f - eloo::Math::log(1.0f + (1.0f - t) * (eloo::Math::Consts::f32::e - 1.0f));
    }

    FORCE_INLINE constexpr float logarithmic_in_out(float t) {
        t *= 2.0f;
        return (t < 1 ? logarithmic_in(t) : 1.0f + logarithmic_out(t - 1.0f)) * 0.5f;
    }

    // Poly         //////////////////////////////////////
    DEFINE_INTERPOLATION_TYPE_PARAM(poly, float, n, 5)

    FORCE_INLINE constexpr float poly_in(float t, float n) {
        return pow(t, n);
    }

    FORCE_INLINE constexpr float poly_out(float t, float  n) {
        return 1.0f - pow(1.0f - t, n);
    }

    FORCE_INLINE constexpr float poly_in_out(float t, float n) {
        t *= 2.0f;
        return (t < 1 ? poly_in(t, n) : 1.0f + poly_out(t - 1.0f, n)) * 0.5f;
    }

    // Bezier       //////////////////////////////////////
    FORCE_INLINE constexpr float bezier(float t, float ctrl1 = 0.1f, float ctrl2 = 0.1f) {
        const float omt = 1.0f - t;
        return 3.0f * omt * omt * t * ctrl1 + 3.0f * omt * t * t * ctrl2 + t * t * t;
    }

    // Step         //////////////////////////////////////
    FORCE_INLINE constexpr float step(float t, unsigned int steps) {
        return round(t * steps) / static_cast<float>(steps);
    }

    // Spring       //////////////////////////////////////
    //https://www.desmos.com/calculator/jr8dd7xjr6
    FORCE_INLINE constexpr float spring(float t, float frequency = 0.64f, float oscillation = 5.6f, float decay = 1.4f) {
        return (sin(t * eloo::Math::Consts::f32::PI * (frequency + oscillation * t * t * t)) * pow(1.0f - t, decay) + t) * (1.0f + (0.8f * pow(1.0f - t, t)));
    }
}

float eloo::Math::Interpolation::interpolate(float from, float to, float t, Type type, Options options) {
    t = saturate(t);
    const float diff = to - from;
    Ease ease = Ease::in;
    if (eastl::holds_alternative<EaseOption>(options)) {
        ease = eastl::get<EaseOption>(options).ease;
    }

    switch (type) {
    case Type::lerp:        return from + diff * t;
    case Type::sine:        return from + diff * sine(t, ease);
    case Type::quad:        return from + diff * quad(t, ease);
    case Type::cubic:       return from + diff * cubic(t, ease);
    case Type::quart:       return from + diff * quart(t, ease);
    case Type::quint:       return from + diff * quint(t, ease);
    case Type::circ:        return from + diff * circ(t, ease);
    case Type::elastic:     return from + diff * elastic(t, ease);
    case Type::bounce:      return from + diff * bounce(t, ease);
    case Type::logarithmic: return from + diff * logarithmic(t, ease);

    //////////////////////////////////////////////
    // Types with optional data

    case Type::expo:
    {
        if (eastl::holds_alternative<ExpoOptions>(options)) {
            const ExpoOptions& data = eastl::get<ExpoOptions>(options);
            return from + diff * expo(t, data.ease, data.power);
        }
        return from + diff * expo(t, ease);
    }

    case Type::back:
    {
        if (eastl::holds_alternative<BackOptions>(options)) {
            const BackOptions& data = eastl::get<BackOptions>(options);
            return from + diff * back(t, data.ease, data.overshoot);
        }
        return from + diff * back(t, ease);
    }

    case Type::bezier: {
        if (eastl::holds_alternative<BezierOptions>(options)) {
            const BezierOptions& data = eastl::get<BezierOptions>(options);
            return from + diff * bezier(t, data.ctrl1, data.ctrl2);
        }
        return from + diff * bezier(t);
    }

    case Type::spring:
    {
        if (eastl::holds_alternative<SpringOptions>(options)) {
            const SpringOptions& data = eastl::get<SpringOptions>(options);
            return from + diff * spring(t, data.frequency, data.oscillation, data.decay);
        }
        return from + diff * spring(t);
    }

    //////////////////////////////////////////////
    // Types with REQUIRED data

    case Type::poly: {
        if (eastl::holds_alternative<PolyOptions>(options)) {
            const PolyOptions& data = eastl::get<PolyOptions>(options);
            return from + diff * poly(t, data.ease, data.n);
        }
        ASSERT_FALSE("Attempting to call Interpolation::interpolate() of Type::poly without passing in a PolyOptions struct.");
        break;
    }

    case Type::step: {
        if (eastl::holds_alternative<StepOptions>(options)) {
            const StepOptions& data = eastl::get<StepOptions>(options);
            return from + diff * step(t, data.steps);
        }
        ASSERT_FALSE("Attempting to call Interpolation::interpolate() of Type::steps without passing in a StepOptions struct.");
        break;
    }

    // Unknown type
    default:
        ASSERT_FALSE("Attempting to call Interpolation::interpolate() with unknown type %i",
            static_cast<int>(type));
        return to;
    }
}