#ifndef LIBC_MNTENT_H
#define LIBC_MNTENT_H

#include <paths.h>

#ifndef __FILE_defined
#define __FILE_defined
#include <file.h>
typedef struct __FILE FILE;
#endif

#define MNTTAB _PATH_MNTTAB 

#define MOUNTED _PATH_MOUNTED 

struct mntent {
	char *mnt_fsname; 
	char *mnt_dir;	  
	char *mnt_type;	  
	char *mnt_opts;	  
	int mnt_freq;	  
	int mnt_passno;	  
};

/**
 * @brief setmentent
 * 
 * @param filename 
 * @param type 
 * @return FILE* 
 */
FILE *setmntent(const char *filename, const char *type);

/**
 * @brief mntent
 * 
 * @param stream 
 * @return struct mntent* 
 */
struct mntent *getmntent(FILE *stream);

/**
 * @brief addmntent
 * 
 * @param stream 
 * @param mnt 
 * @return int 
 */
int addmntent(FILE *stream, const struct mntent *mnt);

/**
 * @brief endmntent
 * 
 * @param streamp 
 * @return int 
 */
int endmntent(FILE *streamp);

/**
 * @brief hasmntopt
 * 
 * @param mnt 
 * @param opt 
 * @return char* 
 */
char *hasmntopt(const struct mntent *mnt, const char *opt);

#endif