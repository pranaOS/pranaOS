/**
 * @file shadow.h
 * @author Krisna Pranav
 * @brief shadow
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <bits/FILE.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct spwd {
    char* sp_namp;
    char* sp_pwdp;
    long int sp_lstchg;
    long int sp_min;
    long int sp_max;
    long int sp_warn;
    long int sp_inact;
    long int sp_expire;
    unsigned long int sp_flag;
};

struct spwd* getspent(void);
void setspent(void);
void endspent(void);

/**
 * @param name 
 * @return struct spwd* 
 */
struct spwd* getspnam(char const* name);

/**
 * @param p 
 * @param stream 
 * @return int 
 */
int putspent(struct spwd* p, FILE* stream);

/**
 * @brief Get the spent r object
 * 
 * @param spbuf 
 * @param buf 
 * @param buflen 
 * @param spbufp 
 * @return int 
 */
int getspent_r(struct spwd* spbuf, char* buf, size_t buflen, struct spwd** spbufp);

/**
 * @brief Get the spnam r object
 * 
 * @param name 
 * @param spbuf 
 * @param buf 
 * @param buflen 
 * @param spbufp 
 * @return int 
 */
int getspnam_r(char const* name, struct spwd* spbuf, char* buf, size_t buflen, struct spwd** spbufp);

/**
 * @param fp 
 * @param spbuf 
 * @param buf 
 * @param buflen 
 * @param spbufp 
 * @return int 
 */
int fgetspent_r(FILE* fp, struct spwd* spbuf, char* buf, size_t buflen, struct spwd** spbufp);

/**
 * @param s 
 * @param spbuf 
 * @param buf 
 * @param buflen 
 * @param spbufp 
 * @return int 
 */
int sgetspent_r(char const* s, struct spwd* spbuf, char* buf, size_t buflen, struct spwd** spbufp);

__END_DECLS
