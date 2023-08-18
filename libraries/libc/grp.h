/**
 * @file grp.h
 * @author Krisna Pranav
 * @brief grp
 * @version 6.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

/// @brief group
struct group {
    char* gr_name;
    char* gr_passwd;
    gid_t gr_gid;
    char** gr_mem;
};

/**
 * @return struct group* 
 */
struct group* getgrent();

/**
 * @param name 
 * @return struct group* 
 */
struct group* getgrnam(const char* name);

/**
 * @return struct group* 
*/
struct group* getgrgid(gid_t);

/**
 * @param user 
 * @return int 
 */
int initgroups(const char* user, gid_t);

__END_DECLS