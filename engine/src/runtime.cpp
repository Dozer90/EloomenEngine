#include "utility/defines.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <set>
#include <string>
#include <new>
#include <cstddef>

void sge::runtime_assert(const char* file, int line, bool condition, bool, bool once,
    bool fatal, const char* expression, const char* message, ...) {
    if (condition) { return; }
    static std::mutex mutex;
    static std::set<std::pair<std::string, int>> reported;
    if (once) {
        std::lock_guard lock(mutex);
        if (!reported.emplace(file, line).second && !fatal) { return; }
    }
    std::fprintf(stderr, "%s:%d: assertion failed: %s: ", file, line, expression);
    va_list args;
    va_start(args, message);
    std::vfprintf(stderr, message, args);
    va_end(args);
    std::fputc('\n', stderr);
    if (fatal) { std::abort(); }
}

// EASTL's default allocator requests its named allocation overloads.
void* operator new[](size_t size, const char*, int, unsigned, const char*, int) {
    return ::operator new[](size);
}
void* operator new[](size_t size, size_t alignment, size_t offset, const char*, int,
    unsigned, const char*, int) {
    // Default EASTL storage is released with delete[]; do not mix aligned allocation APIs.
    if (alignment > alignof(std::max_align_t) || offset != 0) { throw std::bad_alloc(); }
    return ::operator new[](size);
}
