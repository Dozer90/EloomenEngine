#include <Maths/CoreMath.h>

#include <Generic/Utilities.h>

#include <EASTL/array.h>

#define INTERP_DEFINITION(name) inline double name(double t, Math::Interpolation::Ease ease) { \
    switch (ease) { \
    case Math::Interpolation::Ease::in:      return name##In(t); \
    case Math::Interpolation::Ease::out:     return name##Out(t); \
    case Math::Interpolation::Ease::inout:   return name##InOut(t); \
    default:            ASSERT_FALSE("Attempting to use " #name " interpolation with unknown ease type %i", \
                                     static_cast<int>(ease)); \
    } \
    return 0.0; \
}

#define INTERP_DEFINITION_PARAM(name, paramType, paramName, defaultVal) \
inline double name(double t, Math::Interpolation::Ease ease, paramType paramName = defaultVal) { \
    switch (ease) { \
    case Math::Interpolation::Ease::in:      return name##In(t, paramName); \
    case Math::Interpolation::Ease::out:     return name##Out(t, paramName); \
    case Math::Interpolation::Ease::inout:   return name##InOut(t, paramName); \
    default:            ASSERT_FALSE("Attempting to use " #name " interpolation with unknown ease type %i", \
                                     static_cast<int>(ease)); \
    } \
    return 0.0; \
}


namespace eloo::Math {
// Sine         //////////////////////////////////////
INTERP_DEFINITION(sine)

inline double sineIn(double t) {
    return 1.0 - Math::cos(t * Math::Consts::dbl::PI * 0.5);
}

inline double sineOut(double t) {
    return Math::sin(t * Math::Consts::dbl::PI * 0.5);
}

inline double sineInOut(double t) {
    return -(Math::cos(t * Math::Consts::dbl::PI) - 1.0) * 0.5;
}

// Quad         //////////////////////////////////////
INTERP_DEFINITION(quad)

inline double quadIn(double t) {
    return t * t;
}

inline double quadOut(double t) {
    const double omt = 1.0 - t;
    return 1.0 - omt * omt;
}

inline double quadInOut(double t) {
    const double omt = 1.0 - t;
    return t < 0.5 ? 2.0 * t * t : 1.0 - 2.0 * omt * omt;
}

// Cubic        //////////////////////////////////////
INTERP_DEFINITION(cubic)

inline double cubicIn(double t) {
    return t * t * t;
}

inline double cubicOut(double t) {
    const double omt = 1.0 - t;
    return 1.0 - omt * omt * omt;
}

inline double cubicInOut(double t) {
    const double omt = 1.0 - t;
    return t < 0.5 ? 4.0 * t * t * t : 1.0 - 4.0 * omt * omt * omt;
}

// Quart        //////////////////////////////////////
INTERP_DEFINITION(quart)

inline double quartIn(double t) {
    return t * t * t * t;
}

inline double quartOut(double t) {
    const double omt = 1.0 - t;
    return 1.0 - omt * omt * omt * omt;
}

inline double quartInOut(double t) {
    const double omt = 1.0 - t;
    return t < 0.5 ? 8.0 * t * t * t * t : 1.0 - 8.0 * omt * omt * omt * omt;
}

// Quint        //////////////////////////////////////
INTERP_DEFINITION(quint)

inline double quintIn(double t) {
    return t * t * t * t * t;
}

inline double quintOut(double t) {
    const double omt = 1.0 - t;
    return 1.0 - omt * omt * omt * omt * omt;
}

inline double quintInOut(double t) {
    const double omt = 1.0 - t;
    return t < 0.5 ? 16.0 * t * t * t * t * t : 1.0 - 16.0 * omt * omt * omt * omt * omt;
}

// Circ         //////////////////////////////////////
INTERP_DEFINITION(circ)

inline double circIn(double t) {
    return 1.0 - Math::sqrt(1.0 - Math::pow(t, 2.0));
}

inline double circOut(double t) {
    return Math::sqrt(1.0 - Math::pow(t - 1.0, 2.0));
}

inline double circInOut(double t) {
    t *= 2.0;
    return (t < 1.0 ? circIn(t) : 1.0 + circOut(t - 1.0)) * 0.5;
}

// Expo         //////////////////////////////////////
INTERP_DEFINITION_PARAM(expo, double, power, 10)

inline double expoIn(double t, double power = 10.0) {
    return t == 0.0 ? 0.0 : Math::pow(2.0, power * t - power);
}

inline double expoOut(double t, double power = 10.0) {
    return t == 1.0 ? 1.0 : 1.0 - Math::pow(2.0, -power * t);
}

inline double expoInOut(double t, double power = 10.0) {
    return t < 0.5 ? Math::pow(2.0 * t, power) * 0.5 : 1.0 - Math::pow(2.0 * (1.0 - t), power) * 0.5;
}

// Back         //////////////////////////////////////
INTERP_DEFINITION_PARAM(back, double, overshoot, 1.70158)

inline double backIn(double t, double overshoot = 1.70158) {
    const double s3 = overshoot + 1.0;
    return s3 * t * t * t - overshoot * t * t;
}

inline double backOut(double t, double overshoot = 1.70158) {
    const double s3 = overshoot + 1.0;
    const double tmo = t - 1.0;
    return 1.0 + s3 * tmo * tmo * tmo + overshoot * tmo * tmo;
}

inline double backInOut(double t, double overshoot = 1.70158) {
    t *= 2.0;
    const double s = overshoot * 1.525;
    return t < 1.0
        ? 0.5 * (t * t * ((s + 1.0) * t - s))
        : 0.5 * ((t - 2.0) * (t - 2.0) * ((s + 1.0) * (t - 2.0) + s) + 2.0);
}

// Elastic      //////////////////////////////////////
INTERP_DEFINITION(elastic)

inline double elasticIn(double t) {
    return -Math::pow(2.0, 10.0 * (t - 1.0)) * Math::sin(((t - 0.075) * Math::Consts::dbl::Tau) * 3.3333333);
}

inline double elasticOut(double t) {
    return 1.0 + Math::pow(2.0, -10.0 * t) * Math::sin(((t - 0.075) * Math::Consts::dbl::Tau) * 3.3333333);
}

inline double elasticInOut(double t) {
    if (t == 0.0 || t == 1.0) return t;
    t *= 2.0;
    if (t < 1.0) return -0.5 * Math::pow(2.0, 10.0 * (t - 1.0)) * Math::sin(((t - 1.075) * Math::Consts::dbl::Tau) * 3.3333333);
    return 0.5 + 0.5 * Math::pow(2.0, -10.0 * (t - 1.0)) * Math::sin(((t - 1.075) * Math::Consts::dbl::Tau) * 3.3333333);
}

// Bounce       //////////////////////////////////////
INTERP_DEFINITION(bounce)

inline double bounceIn(double t) {
    return 1.0 - bounceOut(1.0 - t);
}

inline double bounceOut(double t) {
    constexpr double n1 = 7.5625;
    constexpr double d1 = 0.363636;
    if (t < d1) { return n1 * t * t; }
    if (t < d1 * 2.0) { return n1 * (t - d1 * 1.5) * (t - d1 * 1.5) + 0.75; }
    if (t < d1 * 2.5) { return n1 * (t - d1 * 2.25) * t + 0.9375; }
    return n1 * (t - d1 * 2.625) * t + 0.984375;
}

inline double bounceInOut(double t) {
    t *= 2.0;
    return (t < 1 ? bounceIn(t) : 1.0 + bounceOut(t - 1.0)) * 0.5;
}

// Log          //////////////////////////////////////
INTERP_DEFINITION(log)

inline double logIn(double t) {
    return Math::log(1.0 + t * (Math::Consts::dbl::e - 1.0));
}

inline double logOut(double t) {
    return 1.0 - Math::log(1.0 + (1.0 - t) * (Math::Consts::dbl::e - 1.0));
}

inline double logInOut(double t) {
    t *= 2.0;
    return (t < 1 ? logIn(t) : 1.0 + logOut(t - 1.0)) * 0.5;
}

// Poly         //////////////////////////////////////
INTERP_DEFINITION_PARAM(poly, double, n, 5)

inline double polyIn(double t, double n) {
    return Math::pow(t, n);
}

inline double polyOut(double t, double  n) {
    return 1.0 - Math::pow(1.0 - t, n);
}

inline double polyInOut(double t, double n) {
    t *= 2.0;
    return (t < 1 ? polyIn(t, n) : 1.0 + polyOut(t - 1.0, n)) * 0.5;
}

// Bezier       //////////////////////////////////////
inline double bezier(double t, double ctrl1 = 0.1, double ctrl2 = 0.1) {
    const double omt = 1.0 - t;
    return 3.0 * omt * omt * t * ctrl1 + 3.0 * omt * t * t * ctrl2 + t * t * t;
}

// Step         //////////////////////////////////////
inline double step(double t, unsigned int steps) {
    return Math::round(t * steps) / static_cast<double>(steps);
}

// Spring       //////////////////////////////////////
//https://www.desmos.com/calculator/jr8dd7xjr6
inline double spring(double t, double frequency = 0.64, double oscillation = 5.6, double decay = 1.4) {
    return (Math::sin(t * Math::Consts::dbl::PI * (frequency + oscillation * t * t * t)) * Math::pow(1.0 - t, decay) + t) * (1.0 + (0.8 * pow(1.0 - t, t)));
}
}

using namespace eloo;

double Math::Interpolation::interpolate(double from, double to, double t, Type type, Options options) {
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