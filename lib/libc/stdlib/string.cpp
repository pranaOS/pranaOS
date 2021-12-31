#include <cstring>
#include <cstdlib>

void* memcpy(void* dest, const void* src, size_t len)
{
    size_t i;

    if((uintptr_t)dest % sizeof(long) == 0 &&
        (uintptr_t)src % sizeof(long) == 0 &&
        len % sizeof(long) == 0)
    {

        long* d = (long int*)dest;
        const long *s = (const long int*)src;

        for (i = 0; i < len / sizeof(long); i++)
        {
            d[i] = s[i];
        }
    }
    else
    {
        char* d = (char*)dest;
        const char* s = (const char*)src;

        for (i = 0; i < len; i++)
        {
            d[i] = s[i];
        }
    }

    return dest;
}

void* memmove(void* dest, const void* src, size_t len)
{
    char* d = (char*)dest;
    char* s = (char*)src;

    if (d < s)
    {
        while (len--)
        {
            *d++ = *s++;
        }
    }
    else
    {
        char* last_src = s + (len-1);
        char* last_dest = d + (len-1);

        while (len--)
        {
            *last_dest-- = *last_src--;
        }
    }

    return dest;
}