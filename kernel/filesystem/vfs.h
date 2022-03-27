#pragma once

#include "poll.h"
#include <ak/atomic.h>
#include <libc/fcntl.h>
#include <ak/atomic.h>
#include <libc/list/list.h>
#include <libc/sys/types.h>
#include <system/time.h>
#include <locking/semaphore.h>
#include <stddef.h>
#include <stdint.h>

#define MS_NOUSER (1 << 31)

#define FMODE_READ 0x1
#define FMODE_WRITE 0x2
#define FMODE_CAN_READ 0x20000
#define FMODE_CAN_WRITE 0x40000
#define OPEN_FMODE(flag) ((flag + 1) & O_ACCMODE)

struct vm_area_struct;
struct vfs_superblock;

struct address_space {
    struct vm_area_struct *i_mnap;
    struct list_head page;
    uint32_t npages;  
};

struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned short d_type;
    char d_name[];
};

struct kstat {
    dev_t st_dev;		  
	ino_t st_ino;		  
	mode_t st_mode;		  
	nlink_t st_nlink;	  
	uid_t st_uid;		  
	gid_t st_gid;		  
	dev_t st_rdev;		  
	off_t st_size;		  
	blksize_t st_blksize; 
	blkcnt_t st_blocks;	  
};