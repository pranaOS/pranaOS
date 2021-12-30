#pragma once

#if __USE_LARGEFILE64
typedef int ssize_t;
#else
typedef long ssize_t;
#endif

typedef unsigned int gid_t;
typedef unsigned short uid_t;
typedef short dev_t;
typedef unsigned int mode_t;

#ifdef __USE_LARGEFILE64
typedef long long ino64_t;
typedef ino64_t ino_t;
#else
typedef unsigned long int ino_t;
#endif

#ifdef __USE_LARGEFILE64
typedef signed int off64_t;
typedef off64_t off_t;
#else
typedef long int off_t;
#endif


typedef long time_t;
typedef unsigned long useconds_t;
typedef long suseconds_t;
typedef int pid_t;