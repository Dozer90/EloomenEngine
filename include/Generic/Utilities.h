#pragma once

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


// ===============================================================
// Swizzles

// The following macros are used purely to create each type of swizzle combination for
// the given type. The number of swizzles grow exponentially, with SWIZZLE_FLOAT2
// creating just 4 (2x2) functions: xx, xy, yx, yy... but SWIZZLE_FLOAT4 will create
// 256 (4x4x4x4) + 27 (3x3x3) + 4 (2x2) function combinations as it will also include
// the xxx and xx swizzles also!
// 
// Is this overkill... absolutely!
// 
// Am I going to keep it... we will see.

// float2 expanders --------------------------------------------------------
#define SWIZZLE_FLOAT2_XX_EXPAND2(a,b)          float2 a##b() const { return { a, b }; }
#define SWIZZLE_FLOAT2_XX_EXPAND1(a)            SWIZZLE_FLOAT2_XX_EXPAND2(a,x) SWIZZLE_FLOAT2_XX_EXPAND2(a,y)
#define SWIZZLE_FLOAT2_XX                       SWIZZLE_FLOAT2_XX_EXPAND1(x)   SWIZZLE_FLOAT2_XX_EXPAND1(y)
// float3 expanders --------------------------------------------------------
#define SWIZZLE_FLOAT3_XX_EXPAND2(a,b)          float2 a##b() const { return { a, b }; }
#define SWIZZLE_FLOAT3_XX_EXPAND1(a)            SWIZZLE_FLOAT3_XX_EXPAND2(a,x) SWIZZLE_FLOAT3_XX_EXPAND2(a,y) SWIZZLE_FLOAT3_XX_EXPAND2(a,z)
#define SWIZZLE_FLOAT3_XX                       SWIZZLE_FLOAT3_XX_EXPAND1(x)   SWIZZLE_FLOAT3_XX_EXPAND1(y)   SWIZZLE_FLOAT3_XX_EXPAND1(z)
#define SWIZZLE_FLOAT3_XXX_EXPAND3(a,b,c)       float3 a##b##c() const { return { a, b, c }; }
#define SWIZZLE_FLOAT3_XXX_EXPAND2(a,b)         SWIZZLE_FLOAT3_XXX_EXPAND3(a,b,x) SWIZZLE_FLOAT3_XXX_EXPAND3(a,b,y) SWIZZLE_FLOAT3_XXX_EXPAND3(a,b,z)
#define SWIZZLE_FLOAT3_XXX_EXPAND1(a)           SWIZZLE_FLOAT3_XXX_EXPAND2(a,x)   SWIZZLE_FLOAT3_XXX_EXPAND2(a,y)   SWIZZLE_FLOAT3_XXX_EXPAND2(a,z)
#define SWIZZLE_FLOAT3_XXX                      SWIZZLE_FLOAT3_XXX_EXPAND1(x)     SWIZZLE_FLOAT3_XXX_EXPAND1(y)     SWIZZLE_FLOAT3_XXX_EXPAND1(z)
// float4 expanders --------------------------------------------------------
#define SWIZZLE_FLOAT4_XX_EXPAND2(a,b)          float2 a##b() const { return { a, b }; }
#define SWIZZLE_FLOAT4_XX_EXPAND1(a)            SWIZZLE_FLOAT4_XX_EXPAND2(a,x) SWIZZLE_FLOAT4_XX_EXPAND2(a,y) SWIZZLE_FLOAT4_XX_EXPAND2(a,z) SWIZZLE_FLOAT4_XX_EXPAND2(a,w)
#define SWIZZLE_FLOAT4_XX                       SWIZZLE_FLOAT4_XX_EXPAND1(x)   SWIZZLE_FLOAT4_XX_EXPAND1(y)   SWIZZLE_FLOAT4_XX_EXPAND1(z)   SWIZZLE_FLOAT4_XX_EXPAND1(w)
#define SWIZZLE_FLOAT4_XXX_EXPAND3(a,b,c)       float3 a##b##c() const { return { a, b, c }; }
#define SWIZZLE_FLOAT4_XXX_EXPAND2(a,b)         SWIZZLE_FLOAT4_XXX_EXPAND3(a,b,x) SWIZZLE_FLOAT4_XXX_EXPAND3(a,b,y) SWIZZLE_FLOAT4_XXX_EXPAND3(a,b,z) SWIZZLE_FLOAT4_XXX_EXPAND3(a,b,w)
#define SWIZZLE_FLOAT4_XXX_EXPAND1(a)           SWIZZLE_FLOAT4_XXX_EXPAND2(a,x)   SWIZZLE_FLOAT4_XXX_EXPAND2(a,y)   SWIZZLE_FLOAT4_XXX_EXPAND2(a,z)   SWIZZLE_FLOAT4_XXX_EXPAND2(a,w)
#define SWIZZLE_FLOAT4_XXX                      SWIZZLE_FLOAT4_XXX_EXPAND1(x)     SWIZZLE_FLOAT4_XXX_EXPAND1(y)     SWIZZLE_FLOAT4_XXX_EXPAND1(z)     SWIZZLE_FLOAT4_XXX_EXPAND1(w)
#define SWIZZLE_FLOAT4_XXXX_EXPAND4(a,b,c,d)    float4 a##b##c##d() const { return { a, b, c, d }; }
#define SWIZZLE_FLOAT4_XXXX_EXPAND3(a,b,c)      SWIZZLE_FLOAT4_XXXX_EXPAND4(a,b,c,x) SWIZZLE_FLOAT4_XXXX_EXPAND4(a,b,c,y) SWIZZLE_FLOAT4_XXXX_EXPAND4(a,b,c,z) SWIZZLE_FLOAT4_XXXX_EXPAND4(a,b,c,w)
#define SWIZZLE_FLOAT4_XXXX_EXPAND2(a,b)        SWIZZLE_FLOAT4_XXXX_EXPAND3(a,b,x)   SWIZZLE_FLOAT4_XXXX_EXPAND3(a,b,y)   SWIZZLE_FLOAT4_XXXX_EXPAND3(a,b,z)   SWIZZLE_FLOAT4_XXXX_EXPAND3(a,b,w)
#define SWIZZLE_FLOAT4_XXXX_EXPAND1(a)          SWIZZLE_FLOAT4_XXXX_EXPAND2(a,x)     SWIZZLE_FLOAT4_XXXX_EXPAND2(a,y)     SWIZZLE_FLOAT4_XXXX_EXPAND2(a,z)     SWIZZLE_FLOAT4_XXXX_EXPAND2(a,w)
#define SWIZZLE_FLOAT4_XXXX                     SWIZZLE_FLOAT4_XXXX_EXPAND1(x)       SWIZZLE_FLOAT4_XXXX_EXPAND1(y)       SWIZZLE_FLOAT4_XXXX_EXPAND1(z)       SWIZZLE_FLOAT4_XXXX_EXPAND1(w)
// -------------------------------------------------------------------------
// \/   USE THESE   \/

#define SWIZZLE_FLOAT2                          SWIZZLE_FLOAT2_XX
#define SWIZZLE_FLOAT3                          SWIZZLE_FLOAT3_XX; SWIZZLE_FLOAT3_XXX
#define SWIZZLE_FLOAT4                          SWIZZLE_FLOAT4_XX; SWIZZLE_FLOAT4_XXX; SWIZZLE_FLOAT4_XXXX