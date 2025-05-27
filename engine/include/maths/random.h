#pragma once

#include <random>
#include <chrono>

#define NUMERIC_TEMPLATE template<typename T> inline eastl::enable_if<eastl::is_arithmetic<T>::value, T>::type

namespace eloo::Maths::Random {
namespace {
unsigned int gSeed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
std::mt19937 gRandomGenerator(gSeed);
}

inline void setSeed(unsigned int seed)          { gRandomGenerator.seed(seed); gSeed = seed; }
inline unsigned int getSeed()                   { return gSeed; }

NUMERIC_TEMPLATE value()                        { return std::uniform_real<T>()(gRandomGenerator); }
NUMERIC_TEMPLATE range(T minVal, T maxVal)      { return std::uniform_real_distribution<T>(minVal, maxVal)(gRandomGenerator); }
NUMERIC_TEMPLATE gaussian(T mean, T dev)        { return std::normal_distribution<T>(mean, dev)(gRandomGenerator); }
inline double range01()                         { return range(0.0, 1.0); }
inline bool boolean(double trueChance = 0.5)    { return std::bernoulli_distribution(trueChance)(gRandomGenerator); }
}