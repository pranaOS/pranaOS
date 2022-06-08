#pragma once

#include "lock.h"

enum class Color {
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
};

static SpinLock m_lock;

template <class... Types>
void Log(Color color, Types... args) {
    auto _ = ScopedLocker(m_lock);
}