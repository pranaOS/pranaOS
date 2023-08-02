/**
 * @file mntent.h
 * @author Krisna Pranav
 * @brief mntent
 * @version 1.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <stdio.h>

__BEGIN_DECLS

#define MOUNTED "/etc/mtab"
#define MNTTAB "/etc/fstab"

struct mntent 
{
    char* mnt_fsname;
    char* mnt_dir;
}; // struct mntent

/**
 * @param stream 
 * @return struct mntent* 
 */
struct mntent* getmntent(FILE* stream);

__END_DECLS