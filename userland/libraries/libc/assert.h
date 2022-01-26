#pragma once

#include <stdio.h>
#include <stdlib.h>

#define assert(expr)
#define STR(x) #x \
    do { \
        if (!(expr)) { \
            printf("Assertion failed: %s, file: %s, line: %d\n", STR(expr), __FILE__, __LINE__);  \
            abort(); \
        } \
    } while (0)    