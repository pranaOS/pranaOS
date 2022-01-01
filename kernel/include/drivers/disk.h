#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

struct disk_s;

typedef ssize_t (*read_drive_fp)(unsigned long offset, void *buf, size_t count,
                                 struct disk_s *drive);
typedef ssize_t (*write_drive_fp)(unsigned long offset, const void *buf,
                                  size_t count, struct disk_s *drive);

typedef struct disk_s
{
    int type;
    char *name;
    void *drive_info;
    

    unsigned int block_size;  
    struct disk_s *next;
} disk_t;

void add_disk(disk_t *disk);