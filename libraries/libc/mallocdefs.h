/**
 * @file mallocdefs.h
 * @author Krisna Pranav
 * @brief mallocdefs
 * @version 1.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/inlinelinkedlist.h>
#include <mods/types.h>

#define MAGIC_PAGE_HEADER 0x42657274     
#define MAGIC_BIGALLOC_HEADER 0x42697267 
#define MALLOC_SCRUB_BYTE 0xdc
#define FREE_SCRUB_BYTE 0xed

static constexpr unsigned short size_classes[] = { 8, 16, 32, 64, 128, 256, 500, 1016, 2032, 4088, 8184, 16376, 32752, 0 };
static constexpr size_t num_size_classes = (sizeof(size_classes) / sizeof(unsigned short)) - 1;

struct CommonHeader 
{
    size_t m_magic;
    size_t m_size;
}; // struct CommonHeader

struct BigAllocationBlock : public CommonHeader 
{
    /**
     * @param size 
     */
    BigAllocationBlock(size_t size)
    {
        m_magic = MAGIC_BIGALLOC_HEADER;
        m_size = size;
    }

    unsigned char* m_slot[0];
}; // struct BigAllocationBlock

struct FreelistEntry 
{
    FreelistEntry* next;
}; // struct FreelistEntry

struct ChunkedBlock : public CommonHeader, public InlineLinkedListNode<ChunkedBlock> {

    static constexpr size_t block_size = 64 * KiB;
    static constexpr size_t block_mask = ~(block_size - 1);

    /**
     * @param bytes_per_chunk 
     */
    ChunkedBlock(size_t bytes_per_chunk)
    {
        m_magic = MAGIC_PAGE_HEADER;
        m_size = bytes_per_chunk;
        m_free_chunks = chunk_capacity();
        m_freelist = (FreelistEntry*)chunk(0);

        for (size_t i = 0; i < chunk_capacity(); ++i) {
            auto* entry = (FreelistEntry*)chunk(i);
            if (i != chunk_capacity() - 1)
                entry->next = (FreelistEntry*)chunk(i + 1);
            else
                entry->next = nullptr;
        }
    }

    ChunkedBlock* m_prev { nullptr };
    ChunkedBlock* m_next { nullptr };
    FreelistEntry* m_freelist { nullptr };
    size_t m_free_chunks { 0 };

    [[gnu::aligned(8)]] unsigned char m_slot[0];

    /**
     * @param index 
     * @return void* 
     */
    void* chunk(size_t index)
    {
        return &m_slot[index * m_size];
    }

    /**
     * @return true 
     * @return false 
     */
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

    /**
     * @return size_t 
     */
    size_t free_chunks() const 
    { 
        return m_free_chunks; 
    }

    /**
     * @return size_t 
     */
    size_t used_chunks() const 
    { 
        return chunk_capacity() - m_free_chunks; 
    }

    /**
     * @return size_t 
     */
    size_t chunk_capacity() const 
    { 
        return (block_size - sizeof(ChunkedBlock)) / m_size; 
    }
}; // struct ChunkedBlock
