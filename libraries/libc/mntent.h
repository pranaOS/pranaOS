#pragma once

#include <paths.h>

#ifndef __FILE_DEFINED
#define __FILE_DEFINED
#include <file.h>
typedef struct __FILE FILE;
#endif

#define MNTTAB _PATH_MNTTAB

#define MOUNTED _PATH_MOUNTED

struct mntent {
    char *mnt_fsname;
    char *mnt_dir;
    char *mnt_type;
    char *mnt_opts;
    int mnt_freq;
    int mnt_passno;
};  

FILE *setmntent(const char *filename, const char *type);
struct mntent *getmntent(FILE *stream);
int addmntent(FILE *stream, const struct mntent *mnt);