/**
 * @file pwd.h
 * @author Krisna Pranav
 * @brief PWD
 * @version 6.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <bits/FILE.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct passwd {
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
struct passwd* getpwent(void);

void setpwent(void);
void endpwent(void);

/**
 * @param name 
 * @return struct passwd* 
 */
struct passwd* getpwnam(char const* name);

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

/**
 * @brief Get the pwnam r object
 * 
 * @param name 
 * @param pwd 
 * @param buf 
 * @param buflen 
 * @param result 
 * @return int 
 */
int getpwnam_r(char const* name, struct passwd* pwd, char* buf, size_t buflen, struct passwd** result);

/**
 * @brief Get the pwuid r object
 * 
 * @param pwd 
 * @param buf 
 * @param buflen 
 * @param result 
 * @return int 
 */
int getpwuid_r(uid_t, struct passwd* pwd, char* buf, size_t buflen, struct passwd** result);

__END_DECLS
