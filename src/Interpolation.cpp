#include <Maths/CoreMath.h>

#include <Generic/Utilities.h>

#include <EASTL/array.h>

#define INTERP_DEFINITION(name) double Math::Interpolation::name(double t, Ease ease) { \
    switch (ease) { \
    case Ease::in:      return name##In(t); \
    case Ease::out:     return name##Out(t); \
    case Ease::inout:   return name##InOut(t); \
    default:            ASSERT_FALSE("Attempting to call Math::Interpolation::" #name "() with unknown ease type %i", \
                                     static_cast<int>(ease)); \
    } \
    return 0.0; \
}

#define INTERP_DEFINITION_PARAM(name, paramType, paramName) \
double Math::Interpolation::name(double t, Ease ease, paramType paramName) { \
    switch (ease) { \
    case Ease::in:      return name##In(t, paramName); \
    case Ease::out:     return name##Out(t, paramName); \
    case Ease::inout:   return name##InOut(t, paramName); \
    default:            ASSERT_FALSE("Attempting to call Math::Interpolation::" #name "() with unknown ease type %i", \
                                     static_cast<int>(ease)); \
    } \
    return 0.0; \
}


using namespace eloo;

double Math::Interpolation::interpolate(double from, double to, double t, Type type, Options options) {
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
        case Type::log:         return from + diff * log(t, ease);

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

        //////////////////////////////////////////////
        // Types with REQUIRED data

        case Type::poly: {
            if (eastl::holds_alternative<PolyOptions>(options)) {
                const PolyOptions& data = eastl::get<PolyOptions>(options);
                return from + diff * poly(t, data.ease, data.n);
            }
            ASSERT_FALSE("Attempting to call Math::Interpolation::interpolate() of Type::poly without passing in a PolyOptions struct.");
            break;
        }

        case Type::step: {
            if (eastl::holds_alternative<StepOptions>(options)) {
                const StepOptions& data = eastl::get<StepOptions>(options);
                return from + diff * step(t, data.steps);
            }
            ASSERT_FALSE("Attempting to call Math::Interpolation::interpolate() of Type::steps without passing in a StepOptions struct.");
            break;
        }

        // Unknown type
        default:
            ASSERT_FALSE("Attempting to call Math::Interpolation::interpolate() with unknown type %i",
                         static_cast<int>(type));
            return to;
    }
}


// Sine         //////////////////////////////////////
INTERP_DEFINITION(sine)

double Math::Interpolation::sineIn(double t) {
    t = Math::clamp01(t);
    return 1.0 - Math::cos(t * Math::Consts::dbl::PI * 0.5);
}

double Math::Interpolation::sineOut(double t) {
    t = Math::clamp01(t);
    return Math::sin(t * Math::Consts::dbl::PI * 0.5);
}

double Math::Interpolation::sineInOut(double t) {
    t = Math::clamp01(t);
    return -(Math::cos(t * Math::Consts::dbl::PI) - 1.0) * 0.5;
}

// Quad         //////////////////////////////////////
INTERP_DEFINITION(quad)

double Math::Interpolation::quadIn(double t) {
    t = Math::clamp01(t);
    return t * t;
}

double Math::Interpolation::quadOut(double t) {
    t = Math::clamp01(t);
    const double omt = 1.0 - t;
    return 1.0 - omt * omt;
}

double Math::Interpolation::quadInOut(double t) {
    t = Math::clamp01(t);
    const double omt = 1.0 - t;
    return t < 0.5 ? 2.0 * t * t : 1.0 - 2.0 * omt * omt;
}

// Cubic        //////////////////////////////////////
INTERP_DEFINITION(cubic)

double Math::Interpolation::cubicIn(double t) {
    t = Math::clamp01(t);
    return t * t * t;
}

double Math::Interpolation::cubicOut(double t) {
    t = Math::clamp01(t);
    const double omt = 1.0 - t;
    return 1.0 - omt * omt * omt;
}

double Math::Interpolation::cubicInOut(double t) {
    t = Math::clamp01(t);
    const double omt = 1.0 - t;
    return t < 0.5 ? 4.0 * t * t * t : 1.0 - 4.0 * omt * omt * omt;
}

// Quart        //////////////////////////////////////
INTERP_DEFINITION(quart)

double Math::Interpolation::quartIn(double t) {
    t = Math::clamp01(t);
    return t * t * t * t;
}

double Math::Interpolation::quartOut(double t) {
    t = Math::clamp01(t);
    const double omt = 1.0 - t;
    return 1.0 - omt * omt * omt * omt;
}

double Math::Interpolation::quartInOut(double t) {
    t = Math::clamp01(t);
    const double omt = 1.0 - t;
    return t < 0.5 ? 8.0 * t * t * t * t : 1.0 - 8.0 * omt * omt * omt * omt;
}

// Quint        //////////////////////////////////////
INTERP_DEFINITION(quint)

double Math::Interpolation::quintIn(double t) {
    t = Math::clamp01(t);
    return t * t * t * t * t;
}

double Math::Interpolation::quintOut(double t) {
    t = Math::clamp01(t);
    const double omt = 1.0 - t;
    return 1.0 - omt * omt * omt * omt * omt;
}

double Math::Interpolation::quintInOut(double t) {
    t = Math::clamp01(t);
    const double omt = 1.0 - t;
    return t < 0.5 ? 16.0 * t * t * t * t * t : 1.0 - 16.0 * omt * omt * omt * omt * omt;
}

// Expo         //////////////////////////////////////
INTERP_DEFINITION_PARAM(expo, double, power)

double Math::Interpolation::expoIn(double t, double power) {
    t = Math::clamp01(t);
    return t == 0.0 ? 0.0 : Math::pow(2.0, power * t - power);
}

double Math::Interpolation::expoOut(double t, double power) {
    t = Math::clamp01(t);
    return t == 1.0 ? 1.0 : 1.0 - Math::pow(2.0, -power * t);
}

double Math::Interpolation::expoInOut(double t, double power) {
    t = Math::clamp01(t);
    return t < 0.5 ? Math::pow(2.0 * t, power) * 0.5 : 1.0 - Math::pow(2.0 * (1.0 - t), power) * 0.5;
}

// Circ         //////////////////////////////////////
INTERP_DEFINITION(circ)

double Math::Interpolation::circIn(double t) {
    t = Math::clamp01(t);
    return 1.0 - Math::sqrt(1.0 - Math::pow(t, 2.0));
}

double Math::Interpolation::circOut(double t) {
    t = Math::clamp01(t);
    return Math::sqrt(1.0 - Math::pow(t - 1.0, 2.0));
}

double Math::Interpolation::circInOut(double t) {
    t *= 2.0;
    return (t < 1.0 ? circIn(t) : 1.0 + circOut(t - 1.0)) * 0.5;
}

// Back         //////////////////////////////////////
INTERP_DEFINITION_PARAM(back, double, overshoot)

double Math::Interpolation::backIn(double t, double overshoot) {
    t = Math::clamp01(t);
    const double s3 = overshoot + 1.0;
    return s3 * t * t * t - overshoot * t * t;
}

double Math::Interpolation::backOut(double t, double overshoot) {
    t = Math::clamp01(t);
    const double s3 = overshoot + 1.0;
    const double tmo = t - 1.0;
    return 1.0 + s3 * tmo * tmo * tmo + overshoot * tmo * tmo;
}

double Math::Interpolation::backInOut(double t, double overshoot) {
    t *= 2.0;
    const double s = overshoot * 1.525;
    return t < 1.0
        ? 0.5 * (t * t * ((s + 1.0) * t - s))
        : 0.5 * ((t - 2.0) * (t - 2.0) * ((s + 1.0) * (t - 2.0) + s) + 2.0);
}

// Elastic      //////////////////////////////////////
INTERP_DEFINITION(elastic)

double Math::Interpolation::elasticIn(double t) {
    t = Math::clamp01(t);
    return -Math::pow(2.0, 10.0 * (t - 1.0)) * Math::sin(((t - 0.075) * Math::Consts::dbl::Tau) * 3.3333333);
}

double Math::Interpolation::elasticOut(double t) {
    t = Math::clamp01(t);
    return 1.0 + Math::pow(2.0, -10.0 * t) * Math::sin(((t - 0.075) * Math::Consts::dbl::Tau) * 3.3333333);
}

double Math::Interpolation::elasticInOut(double t) {
    t = Math::clamp01(t);
    if (t == 0.0 || t == 1.0) return t;
    t *= 2.0;
    if (t < 1.0) return -0.5 * Math::pow(2.0, 10.0 * (t - 1.0)) * Math::sin(((t - 1.075) * Math::Consts::dbl::Tau) * 3.3333333);
    return 0.5 + 0.5 * Math::pow(2.0, -10.0 * (t - 1.0)) * Math::sin(((t - 1.075) * Math::Consts::dbl::Tau) * 3.3333333);
}

// Bounce       //////////////////////////////////////
INTERP_DEFINITION(bounce)

double Math::Interpolation::bounceIn(double t) {
    return 1.0 - bounceOut(1.0 - t);
}

double Math::Interpolation::bounceOut(double t) {
    constexpr double n1 = 7.5625;
    constexpr double d1 = 0.363636;
    t = Math::clamp01(t);
    if (t < d1)         { return n1 * t * t; }
    if (t < d1 * 2.0)   { return n1 * (t - d1 * 1.5) * (t - d1 * 1.5) + 0.75; }
    if (t < d1 * 2.5)   { return n1 * (t - d1 * 2.25) * t + 0.9375; }
    return n1 * (t - d1 * 2.625) * t + 0.984375;
}

double Math::Interpolation::bounceInOut(double t) {
    t *= 2.0;
    return (t < 1 ? bounceIn(t) : 1.0 + bounceOut(t - 1.0)) * 0.5;
}

// Log          //////////////////////////////////////
INTERP_DEFINITION(log)

double Math::Interpolation::logIn(double t) {
    t = Math::clamp01(t);
    return Math::log(1.0 + t * (Math::Consts::dbl::e - 1.0));
}

double Math::Interpolation::logOut(double t) {
    t = Math::clamp01(t);
    return 1.0 - Math::log(1.0 + (1.0 - t) * (Math::Consts::dbl::e - 1.0));
}

double Math::Interpolation::logInOut(double t) {
    t *= 2.0;
    return (t < 1 ? logIn(t) : 1.0 + logOut(t - 1.0)) * 0.5;
}

// Poly         //////////////////////////////////////
INTERP_DEFINITION_PARAM(poly, double, n)

double Math::Interpolation::polyIn(double t, double n) {
    t = Math::clamp01(t);
    return Math::pow(t, n);
}

double Math::Interpolation::polyOut(double t, double  n) {
    t = Math::clamp01(t);
    return 1.0 - Math::pow(1.0 - t, n);
}

double Math::Interpolation::polyInOut(double t, double n) {
    t *= 2.0;
    return (t < 1 ? polyIn(t, n) : 1.0 + polyOut(t - 1.0, n)) * 0.5;
}

// Bezier       //////////////////////////////////////
double bezier(double t, double ctrl1, double ctrl2) {
    t = Math::clamp01(t);
    const double omt = 1.0 - t;
    return 3.0 * omt * omt * t * ctrl1 + 3.0 * omt * t * t * ctrl2 + t * t * t;
}

// Step         //////////////////////////////////////
double step(double t, unsigned int steps) {
    t = Math::clamp01(t);
    return Math::round(t * steps) / static_cast<double>(steps);