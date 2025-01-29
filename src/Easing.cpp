#include "Utilities/Easing.h"

#include "Maths.h"

#include <EASTL/functional.h>

using namespace eloo;
using namespace eloo::Easing;

namespace {
using EastFunc_t = eastl::function<double(double)>;
double easeInOut(double t, EastFunc_t easeInFunc, EastFunc_t easeOutFunc) {
    return t < 0.5
        ? easeInFunc(t * 2.0) / 2.0
        : 0.5 + easeOutFunc((t - 0.5) * 2.0);
}
}

double easeInSine(double t) {
    return 1.0 - Math::cos((t * Math::PI) / 2.0);
}
double easeOutSine(double t) {
    return Math::sin((t * Math::PI) / 2.0);
}
double easeInOutSine(double t) {
    return easeInOut(t, easeInSine, easeOutSine);
}

double easeInQuad(double t) {
    return t * t;
}
double easeOutQuad(double t) {
    const double oneMinusT = 1.0 - t;
    return 1.0 - oneMinusT * oneMinusT;
}
double easeInOutQuad(double t) {
    return easeInOut(t, easeInQuad, easeOutQuad);
}

double easeInCubic(double t) {
    return t * t * t;
}
double easeOutCubic(double t) {
    const double oneMinusT = 1.0 - t;
    return 1.0 - oneMinusT * oneMinusT * oneMinusT;
}
double easeInOutCubic(double t) {
    return easeInOut(t, easeInCubic, easeOutCubic);
}

double easeInQuart(double t) {
    return t * t * t * t;
}
double easeOutQuart(double t) {
    const double oneMinusT = 1.0 - t;
    return 1.0 - oneMinusT * oneMinusT * oneMinusT * oneMinusT;
}
double easeInOutQuart(double t) {
    return easeInOut(t, easeInQuart, easeOutQuart);
}

double easeInExpo(double t) {
    return t == 0.0 ? 0.0 : Math::pow(2.0, 10.0 * t - 10.0);
}
double easeOutExpo(double t) {
    return t == 1.0 ? 1.0 : 1.0 - Math::pow(2.0, -10.0 * t);
}
double easeInOutExpo(double t) {
    return easeInOut(t, easeInExpo, easeOutExpo);
}

double easeInCirc(double t) {
    return 1.0 - ;
}
double easeOutCirc(double t) {
    return t == 1.0 ? 1.0 : 1.0 - Math::pow(2.0, -10.0 * t);
}
double easeInOutCirc(double t) {
    return easeInOut(t, easeInCirc, easeOutCirc);
}