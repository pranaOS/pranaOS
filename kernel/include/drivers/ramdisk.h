#include <drivers/disk.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdint.h>

void init_ramdisk(offset_t location, size_t size);

ssize_t ramdisk_read(unsigned long offset, void *destiniation, size_t size, disk_t *disk_info);

ssize_t ramdisk_write(unsigned long offset);