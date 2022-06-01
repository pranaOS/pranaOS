#ifndef _LIBC_PWD_H
#define _LIBC_PWD_H

#include <stddef.h>
#include <sys/types.h>

#ifndef __FILE_defined
#define __FILE_defined
#include <file.h>
typedef struct __FILE FILE;
#endif

struct passwd {
    char *pw_name;
    uid_t pw_uid;
    gid_t pw_gid;
    char *pw_dir;
    char *pw_shell;
    char *pw_gecos;
    char *pw_passwd;
};

FILE *openpw(void);
struct passwd *getpwnam(const char *username);
struct passwd *getpwuid(uid_t uid);

#endif