#pragma once

#include <Maths/Math.h>
#include <Maths/float3.h>
#include <Maths/float4.h>

#include <cstdint>

#define ENABLE_TEMPLATE_IF_BASE_OF(base, type) typename = eastl::enable_if_t<eastl::is_base_of<base, type>::value>

#if defined(ASSERTS_ENABLED)
namespace eloo {
void runtimeAssert(const char* file, int line, bool condition, bool once, bool fatal, const char* conditionStr, const char* message, ...);
}

#define ASSERT(cond, message, ...) eloo::runtimeAssert(__FILE__, __LINE__, (cond), false, false, #cond, (message), ##__VA_ARGS__)
#define ASSERT_ONCE(cond, message, ...) eloo::runtimeAssert(__FILE__, __LINE__, (cond), true, false, #cond, (message), ##__VA_ARGS__)
#define ASSERT_FATAL(cond, message, ...) eloo::runtimeAssert(__FILE__, __LINE__, (cond), false, true, #cond, (message), ##__VA_ARGS__)

#define ASSERT_FALSE(message, ...) ASSERT(false, message, ##__VA_ARGS__)
#define ASSERT_ONCE_FALSE(message, ...) ASSERT_ONCE(false, message, ##__VA_ARGS__)
#define ASSERT_FATAL_FALSE(message, ...) ASSERT_FATAL(false, message, ##__VA_ARGS__)

#else
#define ASSERT(cond, message, ...)
#define ASSERT_ONCE(cond, message, ...)
#define ASSERT_FATAL(cond, message, ...)

#define ASSERT_FALSE(message, ...)
#define ASSERT_ONCE_FALSE(message, ...)
#define ASSERT_FATAL_FALSE(message, ...)
#endif // ASSERTS_ENABLED


namespace eloo {
struct Colour {
    union {
        struct {
#if !defined IS_BIG_ENDIAN
            uint8_t r, g, b, a;
#else
            uint8_t a, b, g, r;
#endif
        };
        uint32_t v;
    };

public:
    inline Colour() : v(0x00000000) {}
    inline Colour(uint32_t colourValue) : v(colourValue) {}
    inline Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) { set(red, green, blue, alpha); }
    inline Colour(float red, float green, float blue, float alpha = 1.0f) { set(red, green, blue, alpha); }
    inline Colour(const Math::float3& rgb, float alpha = 1.0f) { set(rgb.x, rgb.y, rgb.z, alpha); }
    inline Colour(const Math::float4& rgba) { set(rgba.x, rgba.y, rgba.z, rgba.w); }

    inline Math::float4 asFloat4() const    { return { asFloat(r), asFloat(g), asFloat(b), asFloat(a) }; }
    inline float getRedFloat() const        { return asFloat(r); }
    inline float getGreenFloat() const      { return asFloat(r); }
    inline float getBlueFloat() const       { return asFloat(r); }
    inline float getAlphaFloat() const      { return asFloat(r); }

    inline void set(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) {
        setRed(red);
        setBlue(blue);
        setGreen(green);
        setAlpha(alpha);
    }
    inline void setRed(uint8_t value)       { r = value; }
    inline void setGreen(uint8_t value)     { g = value; }
    inline void setBlue(uint8_t value)      { b = value; }
    inline void setAlpha(uint8_t value)     { a = value; }

    inline void set(float red, float green, float blue, float alpha = 1.0f) {
        setRed(red);
        setBlue(blue);
        setGreen(green);
        setAlpha(alpha);
    }
    inline void setRed(float value)         { r = fromFloat(value); }
    inline void setGreen(float value)       { g = fromFloat(value); }
    inline void setBlue(float value)        { b = fromFloat(value); }
    inline void setAlpha(float value)       { a = fromFloat(value); }

private:
    inline uint8_t fromFloat(float value) const { return static_cast<uint8_t>(Math::round(Math::saturate(value) * 0xFF)); }
    inline float asFloat(uint8_t value) const { return static_cast<float>(value) / 0xFF; }
};
}