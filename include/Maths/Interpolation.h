#pragma once

#include <EASTL/variant.h>

namespace eloo::Math::Interpolation {
    enum class Type {
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
        logarithmic,

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

    enum class Ease {
        in,
        out,
        inout
    };

    struct EaseOption {
        Ease ease = Ease::in;
    };

    struct ExpoOptions {
        Ease ease = Ease::in;
        float power = 10.0;
    };

    struct BackOptions {
        Ease ease = Ease::in;
        float overshoot = 1.70158;
    };

    struct BezierOptions {
        float ctrl1 = 0.1f;
        float ctrl2 = 0.1f;
    };

    struct SpringOptions {
        float frequency = 0.64;
        float oscillation = 5.6;
        float decay = 1.4;
    };

    struct PolyOptions {
        Ease ease = Ease::in;
        float n = 0.0;
    };

    struct StepOptions {
        int steps = 5;
    };

    using Options = eastl::variant<
        eastl::monostate,
        EaseOption,
        ExpoOptions,
        BackOptions,
        BezierOptions,
        SpringOptions,
        PolyOptions,
        StepOptions>;

    float interpolate(float from, float to, float t, Type type, Options options = eastl::monostate{});
}