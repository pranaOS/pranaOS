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

#include <sys/cdefs.h>
#include <stdio.h>

__BEGIN_DECLS

/// @brief: path of the mtab and fstab
#define MOUNTED "/etc/mtab"
#define MNTTAB "/etc/fstab"

struct mntent 
{
    char* mnt_fsname;
    char* mnt_dir;
    char* mnt_type;
    char* mnt_opts;
    int mnt_freq;
    int mnt_passno;
}; // struct mntent

/**
 * @param stream 
 * @return struct mntent* 
 */
struct mntent* getmntent(FILE* stream);

__END_DECLS
