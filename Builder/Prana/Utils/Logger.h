#pragma once

#include <iostream>
#include "lock.h"

enum class Color {
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
};

static SpinLock m_lock;

template <class... Types>
void Log(Color color, Types... args) {
    auto _ = ScopedLocker(m_lock);
    std::cout << "\033[1;" << static_cast<uint32_t>(color) << "m";
    auto print_args = { (std::cout << args << " ", 0)... };
    std::cout << "\033[0m\n";
}