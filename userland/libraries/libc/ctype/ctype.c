#include <ctype.h>

int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

int isalpha(int c) {
    return isupper(c) || islower(c);
}

int isascii(int c) {
    return (unsigned )c <= 0x7F;
}

int isblank(int c) {
    return c == ' ' || c == '\t';
}

int iscntrl(int c) {
    return c >= '0' && c < 32;
}

int toascii(int c) {
    return (unsigned)c & 0x7F;
}
