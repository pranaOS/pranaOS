#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

__attribute__((noreturn)) void _assert_fail(const char* expression, const char* filename, const char* function, int line);

#define assert(__condition) ((__condition) ? (void)0 : _assert_fail(#__condition, __FILE__, __FUNCTION__, __LINE__))

__END_DECLS