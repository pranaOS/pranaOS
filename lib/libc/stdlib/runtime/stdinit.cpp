#include "sys/cdefs.h"
#include "stdlib/runtime/ubsan.h"

void __stdlib_heap_initialize();

void __stdlib_init()
{
    __stdlib_heap_initialize();
    __ubsan_set_output_file(stderr);
}