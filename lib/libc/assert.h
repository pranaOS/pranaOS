#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

__attribute__((noreturn)) void _assert_fail(const char* expression, const char* filename, const char* function, int line);

__END_DECLS