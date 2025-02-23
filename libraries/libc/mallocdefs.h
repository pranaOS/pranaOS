/**
 * @file mallocdefs.h
 * @author Krisna Pranav
 * @brief mallocdefs
 * @version 6.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/intrusivelist.h>
#include <mods/types.h>

#define MAGIC_PAGE_HEADER 0x42657274     
#define MAGIC_BIGALLOC_HEADER 0x42697267 
#define MALLOC_SCRUB_BYTE 0xdc
#define FREE_SCRUB_BYTE 0xed

#define PAGE_ROUND_UP(x) ((((size_t)(x)) + PAGE_SIZE - 1) & (~(PAGE_SIZE - 1)))

static constexpr unsigned short size_classes[] = { 16, 32, 64, 128, 256, 496, 1008, 2032, 4080, 8176, 16368, 32752, 0 };

static constexpr size_t num_size_classes = (sizeof(size_classes) / sizeof(unsigned short)) - 1;

#ifndef NO_TLS
extern "C" {
extern __thread bool s_allocation_enabled;
}
#endif

consteval bool check_size_classes_alignment()
{
    for (size_t i = 0; i < num_size_classes; i++) {
        if ((size_classes[i] % 16) != 0)
            return false;
    }
    return true;
}

static_assert(check_size_classes_alignment());

struct CommonHeader {
    size_t m_magic;
    size_t m_size;
}; // struct CommonHeader

struct BigAllocationBlock : public CommonHeader {
    /**
     * @brief Construct a new Big Allocation Block object
     * 
     * @param size 
     */
    BigAllocationBlock(size_t size)
    {
        m_magic = MAGIC_BIGALLOC_HEADER;
        m_size = size;
    }
    alignas(16) unsigned char* m_slot[0];
}; // struct BigAllocationBlock : public CommonHeader

struct FreelistEntry {
    FreelistEntry* next;
}; // struct FreelistEntry

struct ChunkedBlock : public CommonHeader {

    static constexpr size_t block_size = 64 * KiB;
    static constexpr size_t block_mask = ~(block_size - 1);

    /**
     * @brief Construct a new Chunked Block object
     * 
     * @param bytes_per_chunk 
     */
    ChunkedBlock(size_t bytes_per_chunk)
    {
        m_magic = MAGIC_PAGE_HEADER;
        m_size = bytes_per_chunk;
        m_free_chunks = chunk_capacity();
    }

    IntrusiveListNode<ChunkedBlock> m_list_node;
    size_t m_next_lazy_freelist_index { 0 };
    FreelistEntry* m_freelist { nullptr };
    size_t m_free_chunks { 0 };
    alignas(16) unsigned char m_slot[0];

    /**
     * @param index 
     * @return void* 
     */
    void* chunk(size_t index)
    {
        return &m_slot[index * m_size];
    }

    bool is_full() const 
    { 
        return m_free_chunks == 0; 
    }

    /**
     * @return size_t 
     */
    size_t bytes_per_chunk() const 
    { 
        return m_size; 
    }


    size_t free_chunks() const 
    { 
        return m_free_chunks; 
    }
    
    size_t used_chunks() const 
    { 
        return chunk_capacity() - m_free_chunks; 
    }

    size_t chunk_capacity() const 
    { 
        return (block_size - sizeof(ChunkedBlock)) / m_size; 
    }

    using List = IntrusiveList<&ChunkedBlock::m_list_node>;
}; // struct ChunkedBlock : public CommonHeader
