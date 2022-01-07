#pragma once

namespace pranaOSLog {
    enum logLevel {
        None,
        Info,
        Warning,
        Error
    };

    void log(logLevel level, char* msg);

    void print();
} 
