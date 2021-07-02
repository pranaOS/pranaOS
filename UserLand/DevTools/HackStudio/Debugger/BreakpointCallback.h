#pragma once

// includes
#include <AK/Function.h>
#include <AK/String.h>
#include <AK/Types.h>

namespace HackStudio {

enum class BreakpointChange {
    Added,
    Removed,
};

typedef Function<void(const String& file, size_t line, BreakpointChange)> BreakpointChangeCallback;

}
