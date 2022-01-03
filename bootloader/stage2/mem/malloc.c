#include <types.h>

extern uint32_t end_bin;
uint32_t placement_address = ((uint32_t) &end_bin);

void *malloc(size_t size, int aligned)
{   
    if (aligned) {
        placement_address += 0x1000;
    }
    placement_address += size;
    return (void*) (placement_address - size);
}

void free(size_t size)
{
    placement_address -= size;
}