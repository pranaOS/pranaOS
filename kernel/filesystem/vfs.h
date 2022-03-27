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

/**
 * @brief FMODE[READ, WRITE, CAN_READ, CAN_WRITE, OPEN]
 * 
 */
#define FMODE_READ 0x1
#define FMODE_WRITE 0x2
#define FMODE_CAN_READ 0x20000
#define FMODE_CAN_WRITE 0x40000
#define OPEN_FMODE(flag) ((flag + 1) & O_ACCMODE)

/**
 * @brief vm area struct + vfs superblock
 * 
 */
struct vm_area_struct;
struct vfs_superblock;

/**
 * @brief address space
 * 
 */
struct address_space {
    struct vm_area_struct *i_mnap;
    struct list_head page;
    uint32_t npages;  
};

/**
 * @brief dirent
 * 
 */
struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned short d_type;
    char d_name[];
};

/**
 * @brief kstat 
 * 
 */
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

    struct timespec st_atim;
    struct timespec st_mtim;
    struct timespec st_ctim;

};

/**
 * @brief ATTR[MODE, UID, GID, SIZE, ATIME, MTIME, CTIME, ATIME_SET, MTIME_SET, FORCE, ATTR_FLAG, ATTR_SUID, ATTR_SGID]
 * 
 */
#define ATTR_MODE 1
#define ATTR_UID 2
#define ATTR_GID 4
#define ATTR_SIZE 8
#define ATTR_ATIME 16
#define ATTR_MTIME 32
#define ATTR_CTIME 64
#define ATTR_ATIME_SET 128
#define ATTR_MTIME_SET 256
#define ATTR_FORCE 512 
#define ATTR_ATTR_FLAG 1024
#define ATTR_KILL_SUID 2048
#define ATTR_KILL_SGID 4096