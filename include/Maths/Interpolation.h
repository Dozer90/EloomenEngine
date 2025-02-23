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
    double power = 10.0;
};

struct BackOptions {
    Ease ease = Ease::in;
    double overshoot = 1.70158;
};

struct BezierOptions {
    double ctrl1 = 0.1f;
    double ctrl2 = 0.1f;
};

struct SpringOptions {
    double frequency = 0.64;
    double oscillation = 5.6;
    double decay = 1.4;
};

struct PolyOptions {
    Ease ease = Ease::in;
    double n = 0.0;
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

double interpolate(double from, double to, double t, Type type, Options options = eastl::monostate{});
}