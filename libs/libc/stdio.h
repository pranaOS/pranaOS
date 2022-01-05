#pragma once

#include <fs/dirent.h>
#include <fs/vfs.h>
#include <stdint.h>

#define BUFSIZ 0x400
#define EOF    -1

int putchar(int);
int puts(const char *);
int readline(char *data, int dataend, char *buf, int size);