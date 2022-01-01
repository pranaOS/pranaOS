#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define DISK_TYPE_RAMDISK  0x00
#define DISK_TYPE_HARDDISK 0x01

struct disk_s;

typedef ssize_t (*read_drive_fp)(unsigned long offset, void *buf, size_t count,
                                 struct disk_s *drive);
typedef ssize_t (*write_drive_fp)(unsigned long offset, const void *buf,
                                  size_t count, struct disk_s *drive);

typedef struct disk_s
{
	int   type;
	char *name;
	void *drive_info; 

	unsigned int   block_size;
	read_drive_fp  read;
	write_drive_fp write;

	struct disk_s *next;
} disk_t;

void add_disk(disk_t *disk);

ssize_t disk_read(unsigned long offset, void *buf, size_t size, disk_t *disk);
ssize_t disk_write(unsigned long offset, const void *buf, size_t size,
                   disk_t *disk);
