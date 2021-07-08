/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <libabi/Syscalls.h>
#include <assert.h>
#include <pranaos/Plugs.h>
#include <stdlib.h>
#include <string.h>

#define LIBALLOC_MAGIC 0xc001c0de
#define LIBALLOC_DEAD 0xdeaddead

#define USE_CASE1
#define USE_CASE2
#define USE_CASE3
#define USE_CASE4
#define USE_CASE5

#define MIN(__x, __y) ((__x) < (__y) ? (__x) : (__y))
#define MAX(__x, __y) ((__x) > (__y) ? (__x) : (__y))

struct MinorBlock;

struct MajorBlock
{
    size_t pages;

    size_t size;

    size_t usage;

    MajorBlock *prev;

    MajorBlock *next;


    MinorBlock *first;
};

#define MAJOR_BLOCK_HEADER_SIZE (ALIGN_UP(sizeof(MajorBlock), 16))

struct MinorBlock
{
    size_t magic;
 
    size_t size;

    size_t req_size;

    MinorBlock *prev;

    MinorBlock *next;

    MajorBlock *block;
};

#define MINOR_BLOCK_HEADER_SIZE (ALIGN_UP(sizeof(MinorBlock), 16))

static MajorBlock *_heap_root = nullptr;

static MajorBlock *_best_bet = nullptr;

static constexpr size_t _page_size = 4096;

// The number of pages to request per chunk. Set up in liballoc_init.
static constexpr size_t _page_count = 16;

static MajorBlock *heap_major_block_create(size_t size)
{
    size_t st = size + MAJOR_BLOCK_HEADER_SIZE;
    st += MINOR_BLOCK_HEADER_SIZE;

    if ((st % _page_size) == 0)
    {
        st = st / (_page_size);
    }
    else
    {
        st = st / (_page_size) + 1;
    }
    
    st = MAX(st, _page_count);

    MajorBlock *maj = (MajorBlock *)__plug_memory_alloc(st * _page_size);

    if (maj == nullptr)
    {
        return nullptr;
    }

    maj->prev = nullptr;
    maj->next = nullptr;
    maj->pages = st;
    maj->size = st * _page_size;
    maj->usage = MAJOR_BLOCK_HEADER_SIZE;
    maj->first = nullptr;

    return maj;
}

bool check_minor_magic(MinorBlock *min, void *ptr, void *caller)
{
    if (min->magic == LIBALLOC_MAGIC)
    {
        return true;
    }

    if (((min->magic & 0xFFFFFF) == (LIBALLOC_MAGIC & 0xFFFFFF)) ||
        ((min->magic & 0xFFFF) == (LIBALLOC_MAGIC & 0xFFFF)) ||
        ((min->magic & 0xFF) == (LIBALLOC_MAGIC & 0xFF)))
    {
        UNUSED(caller);
        UNUSED(ptr);

    }

    if (min->magic == LIBALLOC_DEAD)
    {

    }
    else
    {
    }

    __plug_memory_unlock();

    return false;
}

void *malloc(size_t req_size)
{
    req_size = ALIGN_UP(req_size, 16);

    unsigned long long bestSize = 0;
    unsigned long size = req_size;

    __plug_memory_lock();

    if (size == 0)
    {

        __plug_memory_unlock();

        return malloc(1);
    }


    if (_heap_root == nullptr)
    {
        _heap_root = heap_major_block_create(size);

        if (_heap_root == nullptr)
        {
            __plug_memory_unlock();
            return nullptr;
        }
    }

    MajorBlock *maj = _heap_root;
    bool started_with_bestbet = false;

    if (_best_bet != nullptr)
    {
        bestSize = _best_bet->size - _best_bet->usage;

        if (bestSize > (size + MINOR_BLOCK_HEADER_SIZE))
        {
            maj = _best_bet;
            started_with_bestbet = true;
        }
    }

    while (maj != nullptr)
    {
        size_t diff = maj->size - maj->usage;

        if (bestSize < diff)
        {
            _best_bet = maj;
            bestSize = diff;
        }

        if (diff < (size + MINOR_BLOCK_HEADER_SIZE))
        {

            if (maj->next != nullptr)
            {
                maj = maj->next; 
                continue;
            }

            if (started_with_bestbet)
            {
                maj = _heap_root;
                started_with_bestbet = false;
                continue;
            }

            maj->next = heap_major_block_create(size);

            if (maj->next == nullptr)
            {
                break; 
            }

            maj->next->prev = maj;
            maj = maj->next;

        }

        if (maj->first == nullptr)
        {
            maj->first = (MinorBlock *)((uintptr_t)maj + MAJOR_BLOCK_HEADER_SIZE);

            maj->first->magic = LIBALLOC_MAGIC;
            maj->first->prev = nullptr;
            maj->first->next = nullptr;
            maj->first->block = maj;
            maj->first->size = size;
            maj->first->req_size = req_size;
            maj->usage += size + MINOR_BLOCK_HEADER_SIZE;

            void *p = (void *)((uintptr_t)(maj->first) + MINOR_BLOCK_HEADER_SIZE);

            __plug_memory_unlock();
            return p;
        }

        {
            // CASE 3: Block in use and enough space at the start of the block.
            size_t diff = (uintptr_t)(maj->first);
            diff -= (uintptr_t)maj;
            diff -= MAJOR_BLOCK_HEADER_SIZE;

            if (diff >= (size + MINOR_BLOCK_HEADER_SIZE))
            {
                // Yes, space in front. Squeeze in.
                maj->first->prev = (MinorBlock *)((uintptr_t)maj + MAJOR_BLOCK_HEADER_SIZE);
                maj->first->prev->next = maj->first;
                maj->first = maj->first->prev;

                maj->first->magic = LIBALLOC_MAGIC;
                maj->first->prev = nullptr;
                maj->first->block = maj;
                maj->first->size = size;
                maj->first->req_size = req_size;
                maj->usage += size + MINOR_BLOCK_HEADER_SIZE;

                void *p = (void *)((uintptr_t)(maj->first) + MINOR_BLOCK_HEADER_SIZE);
                __plug_memory_unlock(); // release the lock
                return p;
            }
        }

        MinorBlock *min = maj->first;

        while (min != nullptr)
        {
            if (min->next == nullptr)
            {
                size_t diff = (uintptr_t)(maj) + maj->size;
                diff -= (uintptr_t)min;
                diff -= MINOR_BLOCK_HEADER_SIZE;
                diff -= min->size;


                if (diff >= (size + MINOR_BLOCK_HEADER_SIZE))
                {
                    min->next = (MinorBlock *)((uintptr_t)min + MINOR_BLOCK_HEADER_SIZE + min->size);
                    min->next->prev = min;
                    min = min->next;
                    min->next = nullptr;
                    min->magic = LIBALLOC_MAGIC;
                    min->block = maj;
                    min->size = size;
                    min->req_size = req_size;
                    maj->usage += size + MINOR_BLOCK_HEADER_SIZE;

                    void *p = (void *)((uintptr_t)min + MINOR_BLOCK_HEADER_SIZE);
                    __plug_memory_unlock(); // release the lock
                    return p;
                }
            }

            // CASE 4.2: Is there space between two minors?
            if (min->next != nullptr)
            {
                // is the difference between here and next big enough?
                size_t diff = (uintptr_t)(min->next);
                diff -= (uintptr_t)min;
                diff -= MINOR_BLOCK_HEADER_SIZE;
                diff -= min->size;
                // minus our existing usage.

                if (diff >= (size + MINOR_BLOCK_HEADER_SIZE))
                {
                    MinorBlock *new_min = (MinorBlock *)((uintptr_t)min + MINOR_BLOCK_HEADER_SIZE + min->size);

                    new_min->magic = LIBALLOC_MAGIC;
                    new_min->next = min->next;
                    new_min->prev = min;
                    new_min->size = size;
                    new_min->req_size = req_size;
                    new_min->block = maj;
                    min->next->prev = new_min;
                    min->next = new_min;
                    maj->usage += size + MINOR_BLOCK_HEADER_SIZE;

                    void *p = (void *)((uintptr_t)new_min + MINOR_BLOCK_HEADER_SIZE);

                    __plug_memory_unlock();

                    return p;
                }
            }  

            min = min->next;
        } 

        if (maj->next == nullptr)
        {

            if (started_with_bestbet)
            {
                 
                maj = _heap_root;
                started_with_bestbet = false;
                continue;
            }

           
            maj->next = heap_major_block_create(size);

            if (maj->next == nullptr)
            {
                 
                break;
            }

            maj->next->prev = maj;
        }

        maj = maj->next;
    }

    __plug_memory_unlock();


    return nullptr;
}

void free(void *ptr)
{
    if (ptr == nullptr)
    {

        return;
    }

    __plug_memory_lock();

    MinorBlock *min = (MinorBlock *)((uintptr_t)ptr - MINOR_BLOCK_HEADER_SIZE);

    if (!check_minor_magic(min, ptr, __builtin_return_address(0)))
    {
        return;
    }

    MajorBlock *maj = min->block;

    maj->usage -= (min->size + MINOR_BLOCK_HEADER_SIZE);
    min->magic = LIBALLOC_DEAD;

    if (min->next != nullptr)
    {
        min->next->prev = min->prev;
    }

    if (min->prev != nullptr)
    {
        min->prev->next = min->next;
    }

    if (min->prev == nullptr)
    {
        maj->first = min->next;
    }

    if (maj->first == nullptr)
    {
        if (_heap_root == maj)
        {
            _heap_root = maj->next;
        }

        if (_best_bet == maj)
        {
            _best_bet = nullptr;
        }

        if (maj->prev != nullptr)
        {
            maj->prev->next = maj->next;
        }

        if (maj->next != nullptr)
        {
            maj->next->prev = maj->prev;
        }

        __plug_memory_free(maj, maj->pages * _page_size);
    }
    else
    {
        if (_best_bet != nullptr)
        {
            int bestSize = _best_bet->size - _best_bet->usage;
            int majSize = maj->size - maj->usage;

            if (majSize > bestSize)
            {
                _best_bet = maj;
            }
        }
    }

    __plug_memory_unlock();
}

void malloc_cleanup(void *buffer)
{
    if (*(void **)buffer)
    {
        free(*(void **)buffer);
        *(void **)buffer = nullptr;
    }
}

__attribute__((optimize("O0"))) void *calloc(size_t nobj, size_t size)
{
    size_t real_size = nobj * size;

    assert(size != 0 && real_size / size == nobj);

    void *p = malloc(real_size);
    memset(p, 0, real_size);

    return p;
}

void *realloc(void *ptr, size_t size)
{
    size = ALIGN_UP(size, 16);

    if (size == 0)
    {
        free(ptr);

        return nullptr;
    }

    if (ptr == nullptr)
    {
        return malloc(size);
    }

    __plug_memory_lock();

    MinorBlock *min = (MinorBlock *)((uintptr_t)ptr - MINOR_BLOCK_HEADER_SIZE);

    if (!check_minor_magic(min, ptr, __builtin_return_address(0)))
    {
        __plug_memory_unlock();
        return nullptr;
    }

    if (min->size >= size)
    {
        min->req_size = size;
        __plug_memory_unlock();
        return ptr;
    }

    __plug_memory_unlock();

    void *new_ptr = malloc(size);
    memcpy(new_ptr, ptr, min->req_size);
    free(ptr);

    return new_ptr;
}