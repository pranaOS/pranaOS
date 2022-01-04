#pragma once

#include <stddef.h>
#include <stdint.h>

struct drive_s;

typedef ssize_t (*read_drive_fp)(unsigned long offset, size_t count, void *buf,
                                struct drive_s drive);

typedef struct drive_s {
    int type;
    char *name;
    void *drive_info;

    read_drive_fp read;
} drive_t;