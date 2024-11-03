#pragma once

#include <source_location>

#define ENABLE_TEMPLATE_IF_BASE_OF(base, type) typename = eastl::enable_if_t<eastl::is_base_of<base, type>::value>

namespace eloo
{

//void runtimeAssert(bool condition, const char* message, const std::source_location& location = std::source_location::current())
void runtimeAssert(bool condition, const char* message)
{
    if (condition) [[likely]] { return; }
    throw std::runtime_error(message);
}

}