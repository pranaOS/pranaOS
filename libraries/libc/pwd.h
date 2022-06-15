#ifndef LIBC_PWD_H
#define LIBC_PWD_H

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

/**
 * @brief openpw
 * 
 * @return FILE* 
 */
FILE *openpw(void);

/**
 * @brief getpwnam
 * 
 * @param username 
 * @return struct passwd* 
 */
struct passwd *getpwnam(const char *username);

/**
 * @brief getpwuid
 * 
 * @param uid 
 * @return struct passwd* 
 */
struct passwd *getpwuid(uid_t uid);

/**
 * @brief Get the pwnam r object
 * 
 * @param username 
 * @param ret 
 * @param buf 
 * @param buflen 
 * @param ret_ptr 
 * @return int 
 */
int getpwnam_r(const char *username, struct passwd *ret, char *buf, size_t buflen, struct passwd **ret_ptr);

/**
 * @brief Get the pwuid r object
 * 
 * @param uid 
 * @param ret 
 * @param buf 
 * @param buflen 
 * @param ret_ptr 
 * @return int 
 */
int getpwuid_r(uid_t uid, struct passwd *ret, char *buf, size_t buflen, struct passwd **ret_ptr);

/**
 * @brief fgetpwent_r
 * 
 * @param fp 
 * @param result 
 * @param buf 
 * @param buf_len 
 * @param result_pointer 
 * @return int 
 */
int fgetpwent_r(FILE *fp, struct passwd *result, char *buf, size_t buf_len, struct passwd **result_pointer);
#endif