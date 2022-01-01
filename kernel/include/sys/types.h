#pragma once

#include <stddef.h>
#include <stdint.h>

#define KB *1024
#define MB KB * 1024

#if __WORDSIZE == 64

typedef uint64_t uword_t;
typedef int64_t  sword_t;

#else

typedef uint32_t uword_t;
typedef int32_t  sword_t;

typedef uint64_t udword_t;
typedef int64_t  sdword_t;

#endif

typedef sword_t ssize_t;

typedef sword_t blkcnt_t;
typedef uword_t blksize_t;
typedef uword_t fsblkcnt_t;
typedef uword_t fsfilcnt_t;
typedef uword_t fsinocnt_t;

typedef uword_t ino_t;
typedef uword_t dev_t;
typedef uword_t pid_t;
typedef uword_t uid_t;
typedef uword_t gid_t;

typedef sword_t off_t;
typedef uword_t offset_t;

typedef uword_t id_t;

typedef uword_t phys_addr_t;

typedef uint64_t       time_t;
typedef unsigned long  clock_t;
typedef unsigned char  nlink_t;
typedef unsigned short mode_t;
typedef unsigned int   flags_t;

typedef uint32_t     reg_t;
typedef unsigned int uint;

typedef unsigned long suseconds_t;