#pragma once

// Asserts
#if defined(ELOO_ASSERTS_ENABLED)
namespace eloo {
    void runtime_assert(const char* file, int line, bool condition, bool forced, bool once, bool fatal, const char* conditionStr, const char* message, ...);
}

#define ELOO_ASSERT(cond, message, ...) eloo::runtime_assert(__FILE__, __LINE__, (cond), false, false, false, #cond, (message), ##__VA_ARGS__)
#define ELOO_ASSERT_ONCE(cond, message, ...) eloo::runtime_assert(__FILE__, __LINE__, (cond), false, true, false, #cond, (message), ##__VA_ARGS__)
#define ELOO_ASSERT_FATAL(cond, message, ...) eloo::runtime_assert(__FILE__, __LINE__, (cond), false, false, true, #cond, (message), ##__VA_ARGS__)

#define ELOO_ASSERT_FALSE(message, ...) eloo::runtime_assert(__FILE__, __LINE__, false, true, false, false, "(FORCED FALSE)", (message), ##__VA_ARGS__)
#define ELOO_ASSERT_ONCE_FALSE(message, ...) eloo::runtime_assert(__FILE__, __LINE__, false, true, true, false, "(FORCED FALSE)", (message), ##__VA_ARGS__)
#define ELOO_ASSERT_FATAL_FALSE(message, ...) eloo::runtime_assert(__FILE__, __LINE__, false, true, false, true, "(FORCED FALSE)", (message), ##__VA_ARGS__)
#else
#define ELOO_ASSERT(cond, message, ...) (cond)
#define ELOO_ASSERT_ONCE(cond, message, ...) (cond)
#define ELOO_ASSERT_FATAL(cond, message, ...) (cond)

#define ELOO_ASSERT_FALSE(message, ...)
#define ELOO_ASSERT_ONCE_FALSE(message, ...)
#define ELOO_ASSERT_FATAL_FALSE(message, ...)
#endif // ELOO_ASSERTS_ENABLED

#define EASTL_FLT_NAN eastl::numeric_limits<float>::quiet_NaN()
#define FLT_NAN std::numeric_limits<float>::quiet_NaN()

#define ELOO_DECLARE_ID_T \
    struct id_t { \
        constexpr id_t(size_t value) ELOO_NOEXCEPT : mID(value) {} \
        constexpr operator size_t() const ELOO_NOEXCEPT { return mID; } \
        constexpr bool operator == (const size_t& other) const { return mID == other; } \
        static constexpr size_t INVALID = static_cast<size_t>(-1); \
    private: \
        size_t mID = 0; \
    };

// Force inline
#if defined(_MSC_VER)
#define ELOO_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define ELOO_FORCE_INLINE inline __attribute__((always_inline))
#else
#define ELOO_FORCE_INLINE inline
#endif

// Likely/unlikely branch prediction hints
#if defined(__GNUC__) || defined(__clang__)
#define ELOO_LIKELY(x)   __builtin_expect(!!(x), 1)
#define ELOO_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define ELOO_LIKELY(x)   (x)
#define ELOO_UNLIKELY(x) (x)
#endif

// Export/import (for DLLs, future-proofing)
#ifdef _WIN32
#define ELOO_EXPORT __declspec(dllexport)
#define ELOO_IMPORT __declspec(dllimport)
#else
#define ELOO_EXPORT __attribute__((visibility("default")))
#define ELOO_IMPORT
#endif

// Nodiscard
#if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
#define ELOO_NODISCARD [[nodiscard]]
#else
#define ELOO_NODISCARD
#endif

// Noexcept
#if defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510L
#define ELOO_NOEXCEPT noexcept
#else
#define ELOO_NOEXCEPT
#endif

// Rule of Three
#define ELOO_CTOR_RULE_OF_THREE(ClassName) \
    ClassName() = default; \
    ClassName(const ClassName&) = default; \
    ClassName& operator=(const ClassName&) = default; \
    ~ClassName() = default;

// Rule of Five
#define ELOO_CTOR_RULE_OF_FIVE(ClassName) \
    ClassName() = default; \
    ClassName(const ClassName&) = default; \
    ClassName& operator=(const ClassName&) = default; \
    ClassName(ClassName&&) ELOO_NOEXCEPT = default; \
    ClassName& operator=(ClassName&&) ELOO_NOEXCEPT = default; \
    ~ClassName() = default;

// No default constructor
#define ELOO_CTOR_DELETE_DEFAULT(ClassName) \
    ClassName() = delete;

// No copy constructor
#define ELOO_CTOR_DELETE_COPY(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete;

// No move constructor
#define ELOO_CTOR_DELETE_MOVE(ClassName) \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;