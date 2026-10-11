#pragma once

typedef double float64_t;
typedef float float32_t;

// Asserts
#if defined(SGE_ASSERTS_ENABLED)
namespace sge {
    void runtime_assert(const char* file, int line, bool condition, bool forced, bool once, bool fatal, const char* conditionStr, const char* message, ...);
}

#define SGE_ASSERT(cond, message, ...) sge::runtime_assert(__FILE__, __LINE__, (cond), false, false, false, #cond, (message), ##__VA_ARGS__)
#define SGE_ASSERT_ONCE(cond, message, ...) sge::runtime_assert(__FILE__, __LINE__, (cond), false, true, false, #cond, (message), ##__VA_ARGS__)
#define SGE_ASSERT_FATAL(cond, message, ...) sge::runtime_assert(__FILE__, __LINE__, (cond), false, false, true, #cond, (message), ##__VA_ARGS__)

#define SGE_ASSERT_FALSE(message, ...) sge::runtime_assert(__FILE__, __LINE__, false, true, false, false, "(FORCED FALSE)", (message), ##__VA_ARGS__)
#define SGE_ASSERT_ONCE_FALSE(message, ...) sge::runtime_assert(__FILE__, __LINE__, false, true, true, false, "(FORCED FALSE)", (message), ##__VA_ARGS__)
#define SGE_ASSERT_FATAL_FALSE(message, ...) sge::runtime_assert(__FILE__, __LINE__, false, true, false, true, "(FORCED FALSE)", (message), ##__VA_ARGS__)
#else
#define SGE_ASSERT(cond, message, ...) (cond)
#define SGE_ASSERT_ONCE(cond, message, ...) (cond)
#define SGE_ASSERT_FATAL(cond, message, ...) (cond)

#define SGE_ASSERT_FALSE(message, ...)
#define SGE_ASSERT_ONCE_FALSE(message, ...)
#define SGE_ASSERT_FATAL_FALSE(message, ...)
#endif // SGE_ASSERTS_ENABLED

#define EASTL_FLT_NAN eastl::numeric_limits<float>::quiet_NaN()
#define FLT_NAN std::numeric_limits<float>::quiet_NaN()

#define SGE_DECLARE_ID_T \
    struct id_t { \
        constexpr id_t(size_t value) SGE_NOEXCEPT : mID(value) {} \
        constexpr operator size_t() const SGE_NOEXCEPT { return mID; } \
        constexpr bool operator == (const size_t& other) const { return mID == other; } \
        static constexpr size_t INVALID = static_cast<size_t>(-1); \
    private: \
        size_t mID = 0; \
    };

// Force inline
#if defined(_MSC_VER)
#define SGE_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define SGE_FORCE_INLINE inline __attribute__((always_inline))
#else
#define SGE_FORCE_INLINE inline
#endif

// Likely/unlikely branch prediction hints
#if defined(__GNUC__) || defined(__clang__)
#define SGE_LIKELY(x)   __builtin_expect(!!(x), 1)
#define SGE_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define SGE_LIKELY(x)   (x)
#define SGE_UNLIKELY(x) (x)
#endif

// Export/import (for DLLs, future-proofing)
#ifdef _WIN32
#define SGE_EXPORT __declspec(dllexport)
#define SGE_IMPORT __declspec(dllimport)
#else
#define SGE_EXPORT __attribute__((visibility("default")))
#define SGE_IMPORT
#endif

// Nodiscard
#if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
#define SGE_NODISCARD [[nodiscard]]
#else
#define SGE_NODISCARD
#endif

// Noexcept
#if defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510L
#define SGE_NOEXCEPT noexcept
#else
#define SGE_NOEXCEPT
#endif

// Rule of Three
#define SGE_CTOR_RULE_OF_THREE(ClassName) \
    ClassName() = default; \
    ClassName(const ClassName&) = default; \
    ClassName& operator=(const ClassName&) = default; \
    ~ClassName() = default;

// Rule of Five
#define SGE_CTOR_RULE_OF_FIVE(ClassName) \
    ClassName() = default; \
    ClassName(const ClassName&) = default; \
    ClassName& operator=(const ClassName&) = default; \
    ClassName(ClassName&&) SGE_NOEXCEPT = default; \
    ClassName& operator=(ClassName&&) SGE_NOEXCEPT = default; \
    ~ClassName() = default;

// No default constructor
#define SGE_CTOR_DELETE_DEFAULT(ClassName) \
    ClassName() = delete;

// No copy constructor
#define SGE_CTOR_DELETE_COPY(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete;

// No move constructor
#define SGE_CTOR_DELETE_MOVE(ClassName) \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;