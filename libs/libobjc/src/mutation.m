#include <stdio.h>
#include <stdlib.h>
#include <libobjc/runtime.h>

void __attribute__((weak)) objc_enumerationMutation(id obj)
{
    fprintf(stderr, "Mutation occured during enumeration.");
    abort();
}