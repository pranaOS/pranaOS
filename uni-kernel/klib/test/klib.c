#include <kernel.h>

#define Z_LEN 1024

int y = 123;

unsigned long z[Z_LEN];

/**
 * @param x
 * @return int
 */
int foo(int x)
{
    z[0] = x;
    return z[0] + y;
}

/**
 * @param complete
 * @return int
 */
int init(status_handler complete)
{
    for(int i = 0; i < Z_LEN; i++)
        if(z[i] != 0)
            return KLIB_INIT_FAILED;

    int r = foo(1);

    if(r != 124)
        return KLIB_INIT_FAILED;

    unsigned long a = -1ull;
    runtime_memset((void*)&a, 0, sizeof(unsigned long));

    return a == 0 ? KLIB_INIT_OK : KLIB_INIT_FAILED;
}