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

char* ltoa(int64_t value, char* str, int base)
{
    return int_to_string<int64_t>(value, str, base);
}

char* ultoa(uint64_t value, char* str, int base)
{
    return int_to_string<uint64_t>(value, str, base);
}

char* utoa(uint32_t value, char* str, int base)
{
    return int_to_string<uint32_t>(value, str, base);
}

char* itoa(int value, char* str, int base)
{
    return int_to_string<int>(value, str, base);
}

#ifndef BUILD_DISABLE_FPA

void ftoa(float value, char* str, int precision)
{

}

double pow(double x, double y)
{
    double result = x;

    for(double d = 0.0f; d < y; d++)
    {
        result *= x;
    }

    return result;
}

#endif

int abs(int i)
{
    return i < 0 ? -i : i;
}

int sqrt(int x)
{
    int quotient = 0;
    int i = 0;

    bool answer_found = false;
    while(answer_found == false)
    {
        if(i * i == x)
        {
          quotient = i;
          answer_found = true;
        }

        i++;
    }

    return quotient;
}