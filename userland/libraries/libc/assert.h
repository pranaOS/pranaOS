#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <sys/cdefs.h>

enum debug_level {
    DEBUG_TRACE = 0,
    DEBUG_INFO = 1,
    DEBUG_WARNING = 2,
    DEBUG_ERROR = 3,
    DEBUG_FATAL = 4,
};

void __dbg(enum debug_level level, bool prefix, const char *file, int line, const char *func, ...);

#ifndef NDEBUG
/* TODO */

#define log(...) ((void)0)
#define err(...) ((void)0)
#define dlog(...) ((void)0)
#define derr(...) ((void)0)

#define assert(expression, ...) ((expression))
#endif 