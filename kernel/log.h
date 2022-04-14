#pragma once

#include <ak/types.h>

namespace Kernel {

    #define LOG_SHOW_MS 1

    enum logLevel {
        Info,
        Warning,
        Error
    };

    void sendLog(logLevel level, const char* __restrict__ format, ...);
    void Print(const char* data, uint32_t length);
}