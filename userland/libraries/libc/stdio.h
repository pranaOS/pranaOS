#pragma once

#include <stdint.h>

namespace LibC {
    class Stio {
      public:
        int putchar(int);
        int puts(const char *);
        int readline(char *data, int dataend, char *buf, int size);
    }
}