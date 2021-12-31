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
    if(a == nullptr) return;
    if(b == nullptr) return;

    if(a < b)
    {
        a->size += b->size + sizeof(heap_block);
        a->next = b->next;
        a->next_free = b->next_free;

        b->next->previous = a;
        b->next->previous_free = a;
        b->next_free->previous_free = a;
    }
    else
    {
        b->size += a->size + sizeof(heap_block);
        b->next = a->next;
        b->next_free = a->next_free;

        a->next->previous = b;
        a->next->previous_free = b;
        a->next_free->previous_free = b;
    }
}

void __stdlib_heap_initialise()
{
    heap_root = (heap_block*)heap;
    heap_root->size = (uint64_t)HEAP_SIZE - sizeof(heap_block);

    heap_root->next = nullptr;
    heap_root->next_free = nullptr;
    heap_root->previous = nullptr;
    heap_root->previous_free = nullptr;

    heap_root->is_free = true;
}