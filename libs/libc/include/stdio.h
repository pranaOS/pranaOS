#pragma once

#include <stdint.h>

namespace pranaOSStdio {

    class Stdio {

    #define BUFSIZ 0x400
    #define EOF    -1

    public:
        int putchar(int);
        int puts(const char *);
        int readline(char *data, int dataend, char *buf, int size);
    }
}