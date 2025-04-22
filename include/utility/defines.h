#pragma once

// Asserts
#if defined(ASSERTS_ENABLED)
namespace eloo {
    void runtime_assert(const char* file, int line, bool condition, bool once, bool fatal, const char* conditionStr, const char* message, ...);
}

#define ASSERT(cond, message, ...) eloo::runtime_assert(__FILE__, __LINE__, (cond), false, false, #cond, (message), ##__VA_ARGS__)
#define ASSERT_ONCE(cond, message, ...) eloo::runtime_assert(__FILE__, __LINE__, (cond), true, false, #cond, (message), ##__VA_ARGS__)
#define ASSERT_FATAL(cond, message, ...) eloo::runtime_assert(__FILE__, __LINE__, (cond), false, true, #cond, (message), ##__VA_ARGS__)

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

#define EASTL_FLT_NAN eastl::numeric_limits<float>::quiet_NaN()
#define FLT_NAN std::numeric_limits<float>::quiet_NaN()

#define ELOO_DECLARE_ID_T \
    struct id_t { \
        constexpr id_t(size_t value) ELOO_NOEXCEPT : mID(value) {} \
        constexpr operator size_t() const ELOO_NOEXCEPT { return mID; } \
        auto operator <=>(const id_t& other) const ELOO_NOEXCEPT = default; \
    private: \
        size_t mID = 0; \
    };

// Force inline
#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif

// Likely/unlikely branch prediction hints
#if defined(__GNUC__) || defined(__clang__)
#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define LIKELY(x)   (x)
#define UNLIKELY(x) (x)
#endif

// Export/import (for DLLs, future-proofing)
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#else
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#endif

// Noexcept
#if defined(__cpp_noexcept_function_type) && __cpp_noexcept_function_type >= 201510L
#define ELOO_NOEXCEPT noexcept
#else
#define ELOO_NOEXCEPT
#endif

// No copy
#define NO_COPY(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No move
#define NO_MOVE(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete;

// No copy and move
#define NO_COPY_MOVE(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor
#define NO_DEFAULT(ClassName) \
    ClassName() = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No destructor
#define NO_DESTRUCTOR(ClassName) \
    ~ClassName() = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No copy and move constructor
#define NO_COPY_MOVE_CONSTRUCTOR(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No copy and move assignment
#define NO_COPY_MOVE_ASSIGNMENT(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No copy and move constructor and assignment
#define NO_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT(ClassName) \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor and destructor
#define NO_DEFAULT_DESTRUCTOR(ClassName) \
    ClassName() = delete; \
    ~ClassName() = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor and copy constructor
#define NO_DEFAULT_COPY_CONSTRUCTOR(ClassName) \
    ClassName() = delete; \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor and move constructor
#define NO_DEFAULT_MOVE_CONSTRUCTOR(ClassName) \
    ClassName() = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor and copy assignment
#define NO_DEFAULT_COPY_ASSIGNMENT(ClassName) \
    ClassName() = delete; \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor and move assignment
#define NO_DEFAULT_MOVE_ASSIGNMENT(ClassName) \
    ClassName() = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor and copy and move constructor
#define NO_DEFAULT_COPY_MOVE_CONSTRUCTOR(ClassName) \
    ClassName() = delete; \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor and copy and move assignment
#define NO_DEFAULT_COPY_MOVE_ASSIGNMENT(ClassName) \
    ClassName() = delete; \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor and copy and move constructor and assignment
#define NO_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT(ClassName) \
    ClassName() = delete; \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete;

// No default constructor and copy and move constructor and assignment and destructor
#define NO_DEFAULT_COPY_MOVE_CONSTRUCTOR_ASSIGNMENT_DESTRUCTOR(ClassName) \
    ClassName() = delete; \
    ClassName(const ClassName&) = delete; \
    ClassName& operator=(const ClassName&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName&&) = delete; \
    ~ClassName() = delete;