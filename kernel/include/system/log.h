#pragma once

#include <ak/types.h>

namespace pranaOS {

    namespace system {

        #define LOG_SHOW_MS 1

        enum LogLevel
        {
            Info,
            Warning,
            Error
        };

        void log(LogLevel level, const char* __restrict__ format, ...);
        void print(const char* data, ak::uint32_t length);
    }
}