#pragma once

#include <sys/types.h>

#if 0
struct stat
{
	dev_t     st_dev;
	ino_t     st_ino;
	mode_t    st_mode;
	nlink_t   st_nlink;
	uid_t     st_uid;
	gid_t     st_gid;
	dev_t     st_rdev;
	off_t     st_size;
	blksize_t st_blksize;
	blkcnt_t  st_blocks;
	time_t    st_atime;
	time_t    st_mtime;
	time_t    st_ctime;
};
#endif

struct stat
{
	short     st_dev;
	short     st_ino;
	int    st_mode;
	int   st_nlink;
	int     st_uid;
	int     st_gid;
	int     st_rdev;
	int     st_size;
	int st_blksize;
	int  st_blocks;
	int    st_atime;
	int    st_mtime;
	int    st_ctime;
};