/**
 * @file pwd.h
 * @author Krisna Pranav
 * @brief PWD
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <sys/cdefs.h>
#include <sys/types.h>
#include <bits/file.h>

__BEGIN_DECLS

struct passwd
{
    char* pw_name;
    char* pw_passwd;
    uid_t pw_uid;
    gid_t pw_gid;
    char* pw_gecos;
    char* pw_dir;
    char* pw_shell;
}; // struct passwd

/**
 * @return struct passwd* 
 */
struct passwd* getpwent();

void setpwent();
void endpwent();

/**
 * @param name 
 * @return struct passwd* 
 */
struct passwd* getpwnam(const char* name);

/**
 * @return struct passwd* 
 */
struct passwd* getpwuid(uid_t);

/**
 * @param p 
 * @param stream 
 * @return int 
 */
int putpwent(const struct passwd* p, FILE* stream);

__END_DECLS