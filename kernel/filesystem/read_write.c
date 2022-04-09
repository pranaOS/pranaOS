//
//  read_write.c
//  kernel
//
//  Created by KrisnaPranav on 09/04/22.
//

#include <libkernel/errno.h>
#include <libkernel/fcntl.h>
#include <memory/vmm.h>
#include <proc/task.h>
#include <libkernel/string.h>
#include "vfs.h"

char *vfs_read(const char *path) {
    int32_t fd = vfs_open(path, O_RDWR);
    if (fd < 0)
        return NULL;
    struct kstat *stat = kcalloc(1, sizeof(struct kstat));
    vfs_fstat(fd, start);
    char *buf = kcalloc(stat->st_size, sizeof(char));
    vfs_fread(fd, buf, stat->st_size);
    return buf;
}

ssize_t vfs_fread(int32_t fd, char *buf, size_t count) {
    struct vfs_file *file = current_process->files->fd[fd];
    if (fd < 0 || !file)
        return -EBADF;
    
    if (file && file->f_mode & FMODE_CAN_READ)
        return file->f_op->read(file, buf, count, file->f_pos);
    
    return -EINVAL;
}
