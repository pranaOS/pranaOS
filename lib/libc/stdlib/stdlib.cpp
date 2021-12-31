#include <cstdlib>
#include <cstdio>
#include <cstring>

void* calloc(size_t size)
{
    void* mem = malloc(size);
    memset(mem, 0, size);

    return mem;
}

template <typename T>
static char* int_to_string(T value, char* str, int base)
{
    char* rc;
    char* ptr;
    char* low;

    if(base < 2 || base > 36)
    {
        *str = '\0';

        return str;
    }
    rc = ptr = str;

    if(value < 0 && base == 10)
    {
        *ptr++ = '-';
    }

    low = ptr;
    do
    {
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    }
    while(value);

    *ptr-- = '\0';

    while(low < ptr)
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }

    return rc;
}

