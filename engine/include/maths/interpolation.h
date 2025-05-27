#pragma once

#include <EASTL/variant.h>

namespace eloo::math::interpolation {
    enum class blend_func {
        // No params
        lerp,
        sine,
        quad,
        cubic,
        quart,
        quint,
        circ,
        elastic,
        bounce,
        log,

        // Optional params
        expo,
        back,
        bezier,
        spring,

        // Require params
        poly,
        step,

        _count
    };

    enum class ease_type {
        in,
        out,
        in_out
    };

    struct ease_opt {
        ease_type ease = ease_type::in;
    };

    struct expo_opt {
        ease_type ease = ease_type::in;
        float power = 10.0;
    };

    struct back_opt {
        ease_type ease = ease_type::in;
        float overshoot = 1.70158;
    };

    struct bezier_opt {
        float ctrl1 = 0.1f;
        float ctrl2 = 0.1f;
    };

    struct spring_opt {
        float frequency = 0.64;
        float oscillation = 5.6;
        float decay = 1.4;
    };

    struct poly_opt {
        ease_type ease = ease_type::in;
        float n = 0.0;
    };

    struct step_opt {
        int steps = 5;
    };

    using blend_opt = eastl::variant<
        eastl::monostate,
        ease_opt,
        expo_opt,
        back_opt,
        bezier_opt,
        spring_opt,
        poly_opt,
        step_opt>;

    float interpolate(float from, float to, float t, blend_func type, blend_opt options = eastl::monostate{});
}