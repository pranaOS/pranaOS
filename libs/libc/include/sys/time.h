#pragma once

#include <bits/time.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

int gettimeofday(timeval_t* tv, timezone_t* tz);
int settimeofday(const timeval_t* tv, const timezone_t* tz);

__END_DECLS