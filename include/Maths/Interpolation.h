#pragma once

#include <EASTL/variant.h>

namespace eloo::Math::Interpolation {

enum class Type {
    lerp,
    sine,
    quad,
    cubic,
    quart,
    quint,
    expo,
    circ,
    back,
    elastic,
    bounce,
    log,
    poly,
    bezier,
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

struct PolyOptions {
    Ease ease = Ease::in;
    double n = 0.0;
};

struct BezierOptions {
    double ctrl1 = 0.1f;
    double ctrl2 = 0.1f;
};

struct StepOptions {
    int steps = 5;
};

using Options = eastl::variant<
    eastl::monostate,
    EaseOption,
    ExpoOptions,
    BackOptions,
    PolyOptions,
    BezierOptions,
    StepOptions>;


double interpolate(double from, double to, double t, Type type, Options options = eastl::monostate{});

double sine(double t, Ease ease);
double sineIn(double t);
double sineOut(double t);
double sineInOut(double t);

double quad(double t, Ease ease);
double quadIn(double t);
double quadOut(double t);
double quadInOut(double t);

double cubic(double t, Ease ease);
double cubicIn(double t);
double cubicOut(double t);
double cubicInOut(double t);

double quart(double t, Ease ease);
double quartIn(double t);
double quartOut(double t);
double quartInOut(double t);

double quint(double t, Ease ease);
double quintIn(double t);
double quintOut(double t);
double quintInOut(double t);

double expo(double t, Ease ease, double power = 10.0);
double expoIn(double t, double power = 10.0);
double expoOut(double t, double power = 10.0);
double expoInOut(double t, double power = 10.0);

double circ(double t, Ease ease);
double circIn(double t);
double circOut(double t);
double circInOut(double t);

double back(double t, Ease ease, double overshoot = 1.70158);
double backIn(double t, double overshoot = 1.70158);
double backOut(double t, double overshoot = 1.70158);
double backInOut(double t, double overshoot = 1.70158);

double elastic(double t, Ease ease);
double elasticIn(double t);
double elasticOut(double t);
double elasticInOut(double t);

double bounce(double t, Ease ease);
double bounceIn(double t);
double bounceOut(double t);
double bounceInOut(double t);

double log(double t, Ease ease);
double logIn(double t);
double logOut(double t);
double logInOut(double t);

double poly(double t, Ease ease, double n);
double polyIn(double t, double n);
double polyOut(double t, double n);
double polyInOut(double t, double n);

double bezier(double t, double ctrl1 = 0.1, double ctrl2 = 0.1);

double step(double t, unsigned int steps = 5);
}