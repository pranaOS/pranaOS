#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
// #include <arpa/inet.h>
#include <libc/arpa/inet.h>
// #include <math.h>
// #include <sockios.h>
#include <libc/ether/if_ether.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unistd.h>

#define BUF_SIZE 512
char buf[BUF_SIZE];

void cat(int fd)
{
    int n = 0;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        if (fwrite(buf, n, 1, stdout) != n) {
            exit(1);
        }
    }
}

int main(int argc, char** argv)
{
    int fd, i;

    if (argc <= 1) {
        cat(0);
        return 0;
    }

    for (i = 1; i < argc; i++) {
        if ((fd = open(argv[i], 0)) < 0) {
            // printf(1, "cat: cannot open %s\n", argv[i]);
            return 1;
        }
        cat(fd);
        close(fd);
    }
    return 0;
}

