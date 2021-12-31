#include <cstring>
#include <cstdlib>

void* memcpy(void *dest, const void *src, size_t len)
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

void* memset(void* ptr, int value, size_t num)
{
    uint8_t* p = (uint8_t*)ptr;
    uint8_t x = value & 0xff;
    uint32_t leftover = num & 0x7;

    if(num < 1) return ptr;

    num = (num + 7) >> 3;

    switch(leftover)
    {
    case 0:
        do
        {
            *p++ = x;
        case 7:
            *p++ = x;
        case 6:
            *p++ = x;
        case 5:
            *p++ = x;
        case 4:
            *p++ = x;
        case 3:
            *p++ = x;
        case 2:
            *p++ = x;
        case 1:
            *p++ = x;
        }
        while(--num > 0);
    }

    return ptr;
}