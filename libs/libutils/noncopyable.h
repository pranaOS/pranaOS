#pragma once

#define UTILS_MAKE_NONCOPYABLE(c)    \
private:
    c(const c&) = delete;            \
    c& operator=(const c&) = delete  \