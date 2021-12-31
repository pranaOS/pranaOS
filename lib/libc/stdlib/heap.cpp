#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <sys/cdefs.h>

struct heap_block
{
    uint64_t size;

    heap_block* next;
    heap_block* next_free;
    heap_block* previous;
    heap_block* previous_free;

    bool is_free;
};

static heap_block* heap_root;

#define HEAP_SIZE 0x10000
uint8_t heap[HEAP_SIZE];

static void heap_merge_blocks(heap_block* a, heap_block* b)
{
    if (a == nullptr) return;
    if (b == nullptr) return;

    if (a < b)
    {
        
    }
}