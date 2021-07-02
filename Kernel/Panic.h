#pragma once

namespace Kernel {

[[noreturn]] void __panic(const char* file, unsigned int line, const char* function);

#define PANIC(...)                                        \
    do {                                                  \
        dmesgln("KERNEL PANIC! :^(");                     \
        dmesgln(__VA_ARGS__);                             \
        __panic(__FILE__, __LINE__, __PRETTY_FUNCTION__); \
    } while (0)

}
