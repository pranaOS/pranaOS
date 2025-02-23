/**
 * @file grp.h
 * @author Krisna Pranav
 * @brief grp
 * @version 6.0
 * @date 2023-07-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <bits/FILE.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct group {
    char* gr_name;
    char* gr_passwd;
    gid_t gr_gid;
    char** gr_mem;
}; // struct group

/**
 * @return struct group* 
 */
struct group* getgrent(void);

void setgrent(void);
void endgrent(void);

/**
 * @param name 
 * @return struct group* 
 */
struct group* getgrnam(char const* name);

/**
 * @return struct group* 
 */
struct group* getgrgid(gid_t);

/**
 * @return int 
 */
int putgrent(const struct group*, FILE*);

/**
 * @param user 
 * @return int 
 */
int initgroups(char const* user, gid_t);

__END_DECLS