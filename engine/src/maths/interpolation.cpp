#include "maths/math.h"

#include "utility/defines.h"

#include <EASTL/array.h>


#define DEFINE_INTERPOLATION_TYPE(interp_t) SGE_FORCE_INLINE float blend_##interp_t(float t, sge::math::interpolation::ease_type ease) { \
    switch (ease) { \
        case sge::math::interpolation::ease_type::in:      return interp_t##_in(t); \
        case sge::math::interpolation::ease_type::out:     return interp_t##_out(t); \
        case sge::math::interpolation::ease_type::in_out:  return interp_t##_in_out(t); \
        default: SGE_ASSERT_FALSE("Attempting to use " #interp_t " interpolation with unknown ease type %i", \
                                   static_cast<int>(ease)); \
        } \
    return 0.0f; \
}

#define DEFINE_INTERPOLATION_TYPE_PARAM(interp_t, param_t, param_name, param_default_value) \
SGE_FORCE_INLINE float blend_##interp_t(float t, sge::math::interpolation::ease_type ease, param_t param_name = param_default_value) { \
    switch (ease) { \
        case sge::math::interpolation::ease_type::in:      return interp_t##_in(t, param_name); \
        case sge::math::interpolation::ease_type::out:     return interp_t##_out(t, param_name); \
        case sge::math::interpolation::ease_type::in_out:  return interp_t##_in_out(t, param_name); \
        default: SGE_ASSERT_FALSE("Attempting to use " #interp_t " interpolation with unknown ease type %i", \
                                   static_cast<int>(ease)); \
        } \
    return 0.0f; \
}

namespace {
    // Sine         //////////////////////////////////////
    SGE_FORCE_INLINE float sine_in(float t) {
        return 1.0f - sge::math::cos(t * sge::math::f32::PI * 0.5f);
    }

    SGE_FORCE_INLINE float sine_out(float t) {
        return sge::math::sin(t * sge::math::f32::PI * 0.5f);
    }

    SGE_FORCE_INLINE float sine_in_out(float t) {
        return -(sge::math::cos(t * sge::math::f32::PI) - 1.0f) * 0.5f;
    }

    DEFINE_INTERPOLATION_TYPE(sine)

    // Quad         //////////////////////////////////////
    SGE_FORCE_INLINE float quad_in(float t) {
        return t * t;
    }

    SGE_FORCE_INLINE float quad_out(float t) {
        const float omt = 1.0f - t;
        return 1.0f - omt * omt;
    }

    SGE_FORCE_INLINE float quad_in_out(float t) {
        const float omt = 1.0f - t;
        return t < 0.5f ? 2.0f * t * t : 1.0f - 2.0f * omt * omt;
    }

    DEFINE_INTERPOLATION_TYPE(quad)

    // Cubic        //////////////////////////////////////
    SGE_FORCE_INLINE float cubic_in(float t) {
        return t * t * t;
    }

    SGE_FORCE_INLINE float cubic_out(float t) {
        const float omt = 1.0f - t;
        return 1.0f - omt * omt * omt;
    }

    SGE_FORCE_INLINE float cubic_in_out(float t) {
        const float omt = 1.0f - t;
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - 4.0f * omt * omt * omt;
    }

    DEFINE_INTERPOLATION_TYPE(cubic)

    // Quart        //////////////////////////////////////
    SGE_FORCE_INLINE float quart_in(float t) {
        return t * t * t * t;
    }

    SGE_FORCE_INLINE float quart_out(float t) {
        const float omt = 1.0f - t;
        return 1.0f - omt * omt * omt * omt;
    }

    SGE_FORCE_INLINE float quart_in_out(float t) {
        const float omt = 1.0f - t;
        return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - 8.0f * omt * omt * omt * omt;
    }

    DEFINE_INTERPOLATION_TYPE(quart)

    // Quint        //////////////////////////////////////
    SGE_FORCE_INLINE float quint_in(float t) {
        return t * t * t * t * t;
    }

    SGE_FORCE_INLINE float quint_out(float t) {
        const float omt = 1.0f - t;
        return 1.0f - omt * omt * omt * omt * omt;
    }

    SGE_FORCE_INLINE float quint_in_out(float t) {
        const float omt = 1.0f - t;
        return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - 16.0f * omt * omt * omt * omt * omt;
    }

    DEFINE_INTERPOLATION_TYPE(quint)

    // Circ         //////////////////////////////////////
    SGE_FORCE_INLINE float circ_in(float t) {
        return 1.0f - sqrt(1.0f - pow(t, 2.0f));
    }

    SGE_FORCE_INLINE float circ_out(float t) {
        return sqrt(1.0f - pow(t - 1.0f, 2.0f));
    }

    SGE_FORCE_INLINE float circ_in_out(float t) {
        t *= 2.0f;
        return (t < 1.0f ? circ_in(t) : 1.0f + circ_out(t - 1.0f)) * 0.5f;
    }

    DEFINE_INTERPOLATION_TYPE(circ)

    // Expo         //////////////////////////////////////
    SGE_FORCE_INLINE float expo_in(float t, float power = 10.0f) {
        return t == 0.0f ? 0.0f : pow(2.0f, power * t - power);
    }

    SGE_FORCE_INLINE float expo_out(float t, float power = 10.0f) {
        return t == 1.0f ? 1.0f : 1.0f - pow(2.0f, -power * t);
    }

    SGE_FORCE_INLINE float expo_in_out(float t, float power = 10.0f) {
        return t < 0.5f ? pow(2.0f * t, power) * 0.5f : 1.0f - pow(2.0f * (1.0f - t), power) * 0.5f;
    }

    DEFINE_INTERPOLATION_TYPE_PARAM(expo, float, power, 10)

    // Back         //////////////////////////////////////
    SGE_FORCE_INLINE float back_in(float t, float overshoot = 1.70158f) {
        const float s3 = overshoot + 1.0f;
        return s3 * t * t * t - overshoot * t * t;
    }

    SGE_FORCE_INLINE float back_out(float t, float overshoot = 1.70158f) {
        const float s3 = overshoot + 1.0f;
        const float tmo = t - 1.0f;
        return 1.0f + s3 * tmo * tmo * tmo + overshoot * tmo * tmo;
    }

    SGE_FORCE_INLINE float back_in_out(float t, float overshoot = 1.70158f) {
        t *= 2.0f;
        const float s = overshoot * 1.525f;
        return t < 1.0f
            ? 0.5f * (t * t * ((s + 1.0f) * t - s))
            : 0.5f * ((t - 2.0f) * (t - 2.0f) * ((s + 1.0f) * (t - 2.0f) + s) + 2.0f);
    }

    DEFINE_INTERPOLATION_TYPE_PARAM(back, float, overshoot, 1.70158f)

    // Elastic      //////////////////////////////////////
    SGE_FORCE_INLINE float elastic_in(float t) {
        return -sge::math::pow(2.0f, 10.0f * (t - 1.0f)) * sge::math::sin(((t - 0.075f) * sge::math::f32::TAU) * 3.3333333f);
    }

    SGE_FORCE_INLINE float elastic_out(float t) {
        return 1.0f + sge::math::pow(2.0f, -10.0f * t) * sge::math::sin(((t - 0.075f) * sge::math::f32::TAU) * 3.3333333f);
    }

    SGE_FORCE_INLINE float elastic_in_out(float t) {
        if (t == 0.0f || t == 1.0f) return t;
        t *= 2.0f;
        if (t < 1.0f) return -0.5f * sge::math::pow(2.0f, 10.0f * (t - 1.0f)) * sge::math::sin(((t - 1.075f) * sge::math::f32::TAU) * 3.3333333f);
        return 0.5f + 0.5f * sge::math::pow(2.0f, -10.0f * (t - 1.0f)) * sge::math::sin(((t - 1.075f) * sge::math::f32::TAU) * 3.3333333f);
    }

    DEFINE_INTERPOLATION_TYPE(elastic)

    // Bounce       //////////////////////////////////////
    SGE_FORCE_INLINE float bounce_out(float t) {
        float n1 = 7.5625f;
        float d1 = 0.363636f;
        if (t < d1) { return n1 * t * t; }
        if (t < d1 * 2.0f) { return n1 * (t - d1 * 1.5f) * (t - d1 * 1.5f) + 0.75f; }
        if (t < d1 * 2.5f) { return n1 * (t - d1 * 2.25f) * t + 0.9375f; }
        return n1 * (t - d1 * 2.625f) * t + 0.984375f;
    }

    SGE_FORCE_INLINE float bounce_in(float t) {
        return 1.0f - bounce_out(1.0f - t);
    }

    SGE_FORCE_INLINE float bounce_in_out(float t) {
        t *= 2.0f;
        return (t < 1 ? bounce_in(t) : 1.0f + bounce_out(t - 1.0f)) * 0.5f;
    }

    DEFINE_INTERPOLATION_TYPE(bounce)

    // Log          //////////////////////////////////////
    SGE_FORCE_INLINE float log_in(float t) {
        return sge::math::log(1.0f + t * (sge::math::f32::E - 1.0f));
    }

    SGE_FORCE_INLINE float log_out(float t) {
        return 1.0f - sge::math::log(1.0f + (1.0f - t) * (sge::math::f32::E - 1.0f));
    }

    SGE_FORCE_INLINE float log_in_out(float t) {
        t *= 2.0f;
        return (t < 1 ? log_in(t) : 1.0f + log_out(t - 1.0f)) * 0.5f;
    }

    DEFINE_INTERPOLATION_TYPE(log)

    // Poly         //////////////////////////////////////
    SGE_FORCE_INLINE float poly_in(float t, float n) {
        return pow(t, n);
    }

    SGE_FORCE_INLINE float poly_out(float t, float  n) {
        return 1.0f - pow(1.0f - t, n);
    }

    SGE_FORCE_INLINE float poly_in_out(float t, float n) {
        t *= 2.0f;
        return (t < 1 ? poly_in(t, n) : 1.0f + poly_out(t - 1.0f, n)) * 0.5f;
    }

    DEFINE_INTERPOLATION_TYPE_PARAM(poly, float, n, 5)

    // Bezier       //////////////////////////////////////
    SGE_FORCE_INLINE float blend_bezier(float t, float ctrl1 = 0.1f, float ctrl2 = 0.1f) {
        const float omt = 1.0f - t;
        return 3.0f * omt * omt * t * ctrl1 + 3.0f * omt * t * t * ctrl2 + t * t * t;
    }

    // Step         //////////////////////////////////////
    SGE_FORCE_INLINE float blend_step(float t, unsigned int steps) {
        return round(t * steps) / static_cast<float>(steps);
    }

    // Spring       //////////////////////////////////////
    //https://www.desmos.com/calculator/jr8dd7xjr6
    SGE_FORCE_INLINE float blend_spring(float t, float frequency = 0.64f, float oscillation = 5.6f, float decay = 1.4f) {
        return (sin(t * sge::math::f32::PI * (frequency + oscillation * t * t * t)) * pow(1.0f - t, decay) + t) * (1.0f + (0.8f * pow(1.0f - t, t)));
    }
}

float sge::math::interpolation::interpolate(float from, float to, float t, blend_func type, blend_opt options) {
    t = saturate(t);
    const float diff = to - from;
    ease_type ease = ease_type::in;
    if (eastl::holds_alternative<ease_opt>(options)) {
        ease = eastl::get<ease_opt>(options).ease;
    }

    switch (type) {
        case blend_func::lerp:      return from + diff * t;
        case blend_func::sine:      return from + diff * blend_sine(t, ease);
        case blend_func::quad:      return from + diff * blend_quad(t, ease);
        case blend_func::cubic:     return from + diff * blend_cubic(t, ease);
        case blend_func::quart:     return from + diff * blend_quart(t, ease);
        case blend_func::quint:     return from + diff * blend_quint(t, ease);
        case blend_func::circ:      return from + diff * blend_circ(t, ease);
        case blend_func::elastic:   return from + diff * blend_elastic(t, ease);
        case blend_func::bounce:    return from + diff * blend_bounce(t, ease);
        case blend_func::log:       return from + diff * blend_log(t, ease);

        //////////////////////////////////////////////
        // Types with optional data

        case blend_func::expo:
        {
            if (eastl::holds_alternative<expo_opt>(options)) {
                const expo_opt& data = eastl::get<expo_opt>(options);
                return from + diff * blend_expo(t, data.ease, data.power);
            }
            return from + diff * blend_expo(t, ease);
        }

        case blend_func::back:
        {
            if (eastl::holds_alternative<back_opt>(options)) {
                const back_opt& data = eastl::get<back_opt>(options);
                return from + diff * blend_back(t, data.ease, data.overshoot);
            }
            return from + diff * blend_back(t, ease);
        }

        case blend_func::bezier: {
            if (eastl::holds_alternative<bezier_opt>(options)) {
                const bezier_opt& data = eastl::get<bezier_opt>(options);
                return from + diff * blend_bezier(t, data.ctrl1, data.ctrl2);
            }
            return from + diff * blend_bezier(t);
        }

        case blend_func::spring:
        {
            if (eastl::holds_alternative<spring_opt>(options)) {
                const spring_opt& data = eastl::get<spring_opt>(options);
                return from + diff * blend_spring(t, data.frequency, data.oscillation, data.decay);
            }
            return from + diff * blend_spring(t);
        }

        //////////////////////////////////////////////
        // Types with REQUIRED data

        case blend_func::poly: {
            if (eastl::holds_alternative<poly_opt>(options)) {
                const poly_opt& data = eastl::get<poly_opt>(options);
                return from + diff * blend_poly(t, data.ease, data.n);
            }
            SGE_ASSERT_FALSE("Attempting to call interpolation::interpolate() of blend_func::blend_poly without passing in a poly_opt struct.");
            break;
        }

        case blend_func::step: {
            if (eastl::holds_alternative<step_opt>(options)) {
                const step_opt& data = eastl::get<step_opt>(options);
                return from + diff * blend_step(t, data.steps);
            }
            SGE_ASSERT_FALSE("Attempting to call interpolation::interpolate() of blend_func::blend_steps without passing in a step_opt struct.");
            break;
        }

        // Unknown type
        default: {
            SGE_ASSERT_FALSE("Attempting to call interpolation::interpolate() with unknown type %i",
                static_cast<int>(type));
            break;
        }
    }

    return to;
}