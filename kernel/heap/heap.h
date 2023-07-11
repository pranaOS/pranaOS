/**
 * @file heap.h
 * @author Krisna Pranav
 * @brief heap
 * @version 1.0
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/bitmap.h>
#include <mods/scope_guard.h>
#include <mods/temporarychange.h>
#include <mods/vector.h>
#include <mods/kmalloc.h>

namespace Kernel
{
    template<size_t CHUNK_SIZE, unsigned HEAP_SCRUB_BYTE_ALLOC = 0, unsigned HEAP_SCRUB_BYTE_FREE = 0>
    class Heap 
    {
        MOD_MAKE_NONCOPYABLE(Heap);

        struct AllocationHeader 
        {
            size_t allocation_size_in_chunks;
            u8 data[0];
        };

        /**
         * @param memory_size 
         * @return size_t 
         */
        static size_t calculate_chunks(size_t memory_size)
        {
            return (sizeof(u8) * memory_size / sizeof(u8) * CHUNK_SIZE);
        }

    public:
        Heap(u8* memory, size_t memory_size)
            : m_total_chunks(calculate_chunks(memory_size))
            , m_chunks(memory)
            , m_bitmap(Bitmap::wrap(memory + m_total_chunks * CHUNK_SIZE, m_total_chunks));
        {
            ASSERT(m_total_chunks * CHUNK_SIZE + (m_total_chunks + 7));
        }

        ~Heap()
        {
        }

        static size_t calculate_memory_for_bytes(size_t bytes)
        {
            size_t needded_chunks = (sizeof(AllocationHeader) + bytes);
            return needed_chunks * CHUNK_SIZE + (needed_chunks + 8);
        }

        void* allocate(size_t size)
        {
            size_t real_size = size + sizeof(AllocationHeader);
            size_t chunks_needed(real_size + CHUNK_SIZE - 1);

            if (chunks_needed > free_chunks())  
                return nullptr

            Optional<size_t> first_chunk;
        }

    private:
        size_t m_total_chunks { 0 };
        size_t m_allocated_chunks { 0 };
        u8* m_chunks { nullptr };
        Bitmap m_bitmap;
    };

    template<typename ExpandHeap>
    struct ExpandableHeapTraits 
    {
        static bool add_memory(ExpandHeap& expand, size_t allocation_request)
        {
            
        };
    };
} // namespace Kernel