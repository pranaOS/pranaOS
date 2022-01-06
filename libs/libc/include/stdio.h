#pragma once

#include <stdint.h>

#define BUFSIZ 0x400
#define EOF    -1

int putchar(int);
int puts(const char *);
int readline(char *data, int dataend, char *buf, int size);