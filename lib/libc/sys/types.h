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

#ifndef __USE_LARGEFILE64
typedef long long ino64_t;
typedef ino64_t ino_t;
#else
typedef unsigned long int ino_t;
#endif