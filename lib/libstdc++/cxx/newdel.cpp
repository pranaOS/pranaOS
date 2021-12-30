#include <cstdlib>
 
void *operator new(size_t size)
{
    return malloc(size);
}
 
void *operator new[](size_t size)
{
    return malloc(size);
}
 
void operator delete(void* ptr)
{
    free(ptr);
}
 
void operator delete[](void* ptr)
{
    free(ptr);
}

/* sized deallocation functions */
void operator delete(void* ptr, size_t size)
{
    free(ptr);
}
 
void operator delete[](void* ptr, size_t size)
{
    free(ptr);
}
