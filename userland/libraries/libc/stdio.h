#pragma once

#include <stdint.h>

namespace Stdio {
    class Std {
      public:
        int putchar(int);
        int puts(const char *);
        int readline(char *data, int dataend, char *buf, int size);
    }
}