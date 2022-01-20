#pragma once

namespace LibC {

    enum logLevel {
        None,
        Info,
        Warning,
        Error
    };

    void log(logLevel level, char* msg);
    void print(const char* __restrict__ format, ...);
}