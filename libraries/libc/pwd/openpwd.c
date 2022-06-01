#include <pwd.h>
#include <stdio.h>

FILE* openpw(void) {
    return fopen("/etc/passwd", "r");
}