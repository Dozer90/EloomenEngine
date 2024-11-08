#pragma once

#define ENABLE_TEMPLATE_IF_BASE_OF(base, type) typename = eastl::enable_if_t<eastl::is_base_of<base, type>::value>

#if defined(ASSERTS_ENABLED)
namespace eloo
{
void runtimeAssert(const char* file, int line, bool condition, bool once, bool fatal, const char* conditionStr, const char* message, ...);
}

#define ASSERT(cond, message, ...) eloo::runtimeAssert(__FILE__, __LINE__, (cond), false, false, #cond, (message), ##__VA_ARGS__)
#define ASSERT_ONCE(cond, message, ...) eloo::runtimeAssert(__FILE__, __LINE__, (cond), true, false, #cond, (message), ##__VA_ARGS__)
#define ASSERT_FATAL(cond, message, ...) eloo::runtimeAssert(__FILE__, __LINE__, (cond), false, true, #cond, (message), ##__VA_ARGS__)

#else
#define ASSERT(cond)
#define ASSERT_ONCE(cond)
#define ASSERT_FATAL(cond)
#endif // ASSERTS_ENABLED

#define ASSERT_FALSE(message, ...) ASSERT(false, message, ##__VA_ARGS__)
#define ASSERT_ONCE_FALSE(message, ...) ASSERT_ONCE(false, message, ##__VA_ARGS__)
#define ASSERT_FATAL_FALSE(message, ...) ASSERT_FATAL(false, message, ##__VA_ARGS__)