/**
 * @file mntent.h
 * @author Krisna Pranav
 * @brief mntent
 * @version 6.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <stdio.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#define MOUNTED "/etc/mtab"
#define MNTTAB "/etc/fstab"

struct mntent {
    char* mnt_fsname;
    char* mnt_dir;
    char* mnt_type;
    char* mnt_opts;
    int mnt_freq;
    int mnt_passno;
};

/**
 * @param stream 
 * @return struct mntent* 
 */
struct mntent* getmntent(FILE* stream);

/**
 * @param filename 
 * @param type 
 * @return FILE* 
 */
FILE* setmntent(char const* filename, char const* type);

/**
 * @param streamp 
 * @return int 
 */
int endmntent(FILE* streamp);

/**
 * @brief Get the mntent r object
 * 
 * @param streamp 
 * @param mntbuf 
 * @param buf 
 * @param buflen 
 * @return struct mntent* 
 */
struct mntent* getmntent_r(FILE* streamp, struct mntent* mntbuf, char* buf, int buflen);

__END_DECLS