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

static constexpr unsigned short size_classes[] = { 8, 16, 32, 64, 128 };

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