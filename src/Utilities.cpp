#include "Generic/Utilities.h"

#include <EASTL/string.h>
#include <EASTL/unordered_set.h>

#include <Windows.h>

using namespace eloo;

#if defined(ASSERTS_ENABLED)

namespace {
eastl::unordered_set<eastl::string> gAssertOnceIDs;
}

void runtimeAssert(const char* file, int line, bool condition, bool once, bool fatal, const char* conditionStr, const char* message, ...) {
    if (condition) [[likely]]
    {
        return;
    }

    if (!fatal && once) {
        // If we are only wanting to assert once, make sure the unique file+line combo
        // has not been seen before
        if (!gAssertOnceIDs.insert(eastl::string().sprintf("%s%d", file, line)).second) {
            return;
        }
    }

    eastl::string formattedMessage;
    formattedMessage.sprintf("%s failed in %s at line %d.\nCondition: %s\n\n", fatal ? "Fatal assert" : "Assert", file, line, conditionStr);

    va_list args;
    va_start(args, message);
    formattedMessage.append_sprintf_va_list(message, args);
    va_end(args);

    if (fatal) {
        MessageBoxA(nullptr, formattedMessage.c_str(), "Fatal Assert", MB_ICONERROR | MB_OK);
        exit(1);
        return;
    }

    const int response = MessageBoxA(nullptr, formattedMessage.c_str(), "Assert", MB_ICONERROR | MB_ABORTRETRYIGNORE | MB_DEFBUTTON2);

    switch (response) {
    case IDABORT:
        exit(1);
        break;
    case IDRETRY:
        if (IsDebuggerPresent()) {
            __debugbreak();
        }
        break;
    case IDIGNORE:
        break;
    }
}

#endif // ASSERTS_ENABLED