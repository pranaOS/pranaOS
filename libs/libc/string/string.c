#include <string.h>

#ifdef __i386__
void* memset(void* dest, int fill, size_t nbytes)
{
    for (int i = 0; i < nbytes; ++i)
        ((uint8_t*)dest)[i] = fill;

    return dest;
}
#endif 

void* memmove(void* dest, const void* src, size_t nbytes)
{
    if (src > dest) {
        memcpy(dest, src, nbytes);
        return dest;
    }

    for (int i = nbytes - 1; i >= 0; --i)
        ((uint8_t*)dest)[i] = ((uint8_t*)src)[i];

    return dest;
}

void* memcpy(void* __restrict dest, const void* __restrict src, size_t nbytes)
{
    size_t chunks, rest, i;

    rest = nbytes % 4;
    chunks = (nbytes - rest) >> 2;

    if (!chunks)
        goto skip_chunks;

    for (i = 0; i < chunks; i++)
        ((uint32_t*)dest)[i] = ((uint32_t*)src)[i];

skip_chunks:

    chunks <<= 2;

    for (i = 0; i < rest; i++)
        ((uint8_t*)dest)[chunks + i] = ((uint8_t*)src)[chunks + i];

    return dest;
}

void* memccpy(void* dest, const void* src, int stop, size_t nbytes)
{
    for (int i = 0; i < nbytes; i++) {
        *((uint8_t*)dest + i) = *((uint8_t*)src + i);

        if (*((uint8_t*)src + i) == stop)
            return ((uint8_t*)dest + i + 1);
    }
    return NULL;
}

int memcmp(const void* src1, const void* src2, size_t nbytes)
{
    uint8_t *first, *second;

    for (int i = 0; i < nbytes; i++) {
        first = (uint8_t*)src1 + i;
        second = (uint8_t*)src2 + i;

        if (*first != *second)
            return *first - *second;
    }

    return 0;
}

size_t strlen(const char* str)
{
    size_t i = 0;
    while (str[i])
        ++i;
    return i;
}

char* strcpy(char* dest, const char* src)
{
    size_t i;
    for (i = 0; src[i] != 0; i++)
        dest[i] = src[i];

    dest[i] = '\0';
    return dest;
}

char* strncpy(char* dest, const char* src, size_t nbytes)
{
    size_t i;

    for (i = 0; i < nbytes && src[i] != 0; i++)
        dest[i] = src[i];

    for (; i < nbytes; i++)
        dest[i] = 0;

    return dest;
}