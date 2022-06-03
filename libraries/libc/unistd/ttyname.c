#include <dirent.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno/error.h>

static char ttyname_buf[TTY_NAME_MAX + 1] = _PATH_DEV;

/**
 * @brief tty name
 * 
 * @param fd 
 * @return char* 
 */
char *ttyname(int fd) {

}

int ttyname_r(int fd, char *name, size_t namesize) {
    if (!isatty(fd))
        return ENOTTY;
    
    struct stat sb;
    
    char entry_path[PATH_MAX];
    int path_dev_length = sizeof(_PATH_DEV) - 1 ;
    memcpy(entry_path, _PATH_DEV, path_dev_length);

    DIR *dirp = opendir(_PATH_DEV);
    struct dirent *entry;
    struct stat esb;

    closedir(dirp);
    return EBADF;
}