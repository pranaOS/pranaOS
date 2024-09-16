/**
 * @file pagedirectory.h
 * @author Krisna Pranav
 * @brief Page Directory
 * @version 6.0
 * @date 2024-09-16
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/badge.h>
#include <mods/types.h>
#include <mods/platform.h>
#include <kernel/forward.h>
#include <kernel/physical_address.h>

VALIDATE_IS_X86()

namespace Kernel 
{
    class PageDirectoryEntry 
    {
    public:
        /**
         * @return PhysicalPtr 
         */
        PhysicalPtr page_table_base() const 
        { 
            return PhysicalAddress::physical_page_base(m_raw); 
        }

        /**
         * @brief Set the page table base object
         * 
         * @param value 
         */
        void set_page_table_base(u32 value)
        {
            m_raw &= 0x8000000000000fffULL;
            m_raw |= PhysicalAddress::physical_page_base(value);
        }

        bool is_null() const 
        { 
            return m_raw == 0; 
        }

        void clear() 
        { 
            m_raw = 0; 
        }

        u64 raw() const 
        { 
            return m_raw; 
        }

        /**
         * @param other 
         */
        void copy_from(Badge<Memory::PageDirectory>, const PageDirectoryEntry& other) 
        { 
            m_raw = other.m_raw; 
        }

        enum Flags 
        {
            Present = 1 << 0,
            ReadWrite = 1 << 1,
            UserSupervisor = 1 << 2,
            WriteThrough = 1 << 3,
            CacheDisabled = 1 << 4,
            Huge = 1 << 7,
            Global = 1 << 8,
            NoExecute = 0x8000000000000000ULL,
        }; // enum Flags

        bool is_present() const 
        { 
            return (raw() & Present) == Present; 
        }

        /**
         * @brief Set the present object
         * 
         * @param b 
         */
        void set_present(bool b) 
        { 
            set_bit(Present, b); 
        }

        bool is_user_allowed() const 
        { 
            return (raw() & UserSupervisor) == UserSupervisor; 
        }

        /**
         * @brief Set the user allowed object
         * 
         * @param b 
         */
        void set_user_allowed(bool b) 
        { 
            set_bit(UserSupervisor, b); 
        }

        bool is_huge() const 
        { 
            return (raw() & Huge) == Huge; 
        }

        /**
         * @brief Set the huge object
         * 
         * @param b 
         */
        void set_huge(bool b) 
        { 
            set_bit(Huge, b); 
        }

        bool is_writable() const 
        { 
            return (raw() & ReadWrite) == ReadWrite; 
        }

        /**
         * @brief Set the writable object
         * 
         * @param b 
         */
        void set_writable(bool b) 
        { 
            set_bit(ReadWrite, b); 
        }

        bool is_write_through() const 
        { 
            return (raw() & WriteThrough) == WriteThrough; 
        }

        /**
         * @brief Set the write through object
         * 
         * @param b 
         */
        void set_write_through(bool b) 
        { 
            set_bit(WriteThrough, b); 
        }

        bool is_cache_disabled() const 
        { 
            return (raw() & CacheDisabled) == CacheDisabled; 
        }

        /**
         * @brief Set the cache disabled object
         * 
         * @param b 
         */
        void set_cache_disabled(bool b) 
        { 
            set_bit(CacheDisabled, b); 
        }

        bool is_global() const 
        { 
            return (raw() & Global) == Global; 
        }

        /**
         * @brief Set the global object
         * 
         * @param b 
         */
        void set_global(bool b) 
        { 
            set_bit(Global, b); 
        }

        bool is_execute_disabled() const 
        { 
            return (raw() & NoExecute) == NoExecute; 
        }

        /**
         * @brief Set the execute disabled object
         * 
         * @param b 
         */
        void set_execute_disabled(bool b) 
        { 
            set_bit(NoExecute, b); 
        }

    private:

        /**
         * @brief Set the bit object
         * 
         * @param bit 
         * @param value 
         */
        void set_bit(u64 bit, bool value)
        {
            if (value)
                m_raw |= bit;
            else
                m_raw &= ~bit;
        }

        u64 m_raw;
    }; // class PageDirectoryEntry

    class PageTableEntry 
    {
    public:
        /**
         * @return PhysicalPtr 
         */
        PhysicalPtr physical_page_base() const 
        { 
            return PhysicalAddress::physical_page_base(m_raw); 
        }

        /**
         * @brief Set the physical page base object
         * 
         * @param value 
         */
        void set_physical_page_base(PhysicalPtr value)
        {
            m_raw &= 0x8000000000000fffULL;
            m_raw |= PhysicalAddress::physical_page_base(value);
        }

        u64 raw() const 
        { 
            return m_raw; 
        }

        enum Flags 
        {
            Present = 1 << 0,
            ReadWrite = 1 << 1,
            UserSupervisor = 1 << 2,
            WriteThrough = 1 << 3,
            CacheDisabled = 1 << 4,
            PAT = 1 << 7,
            Global = 1 << 8,
            NoExecute = 0x8000000000000000ULL,
        };

        bool is_present() const 
        { 
            return (raw() & Present) == Present; 
        }

        /**
         * @brief Set the present object
         * 
         * @param b 
         */
        void set_present(bool b) 
        { 
            set_bit(Present, b); 
        }

        bool is_user_allowed() const 
        { 
            return (raw() & UserSupervisor) == UserSupervisor; 
        }

        /**
         * @brief Set the user allowed object
         * 
         * @param b 
         */
        void set_user_allowed(bool b) 
        { 
            set_bit(UserSupervisor, b); 
        }

        bool is_writable() const 
        { 
            return (raw() & ReadWrite) == ReadWrite; 
        }

        /**
         * @brief Set the writable object
         * 
         * @param b 
         */
        void set_writable(bool b) 
        { 
            set_bit(ReadWrite, b); 
        }

        bool is_write_through() const 
        { 
            return (raw() & WriteThrough) == WriteThrough; 
        }

        /**
         * @brief Set the write through object
         * 
         * @param b 
         */
        void set_write_through(bool b) 
        { 
            set_bit(WriteThrough, b); 
        }

        bool is_cache_disabled() const 
        { 
            return (raw() & CacheDisabled) == CacheDisabled; 
        }

        /**
         * @brief Set the cache disabled object
         * 
         * @param b 
         */
        void set_cache_disabled(bool b) 
        { 
            set_bit(CacheDisabled, b); 
        }

        bool is_global() const 
        { 
            return (raw() & Global) == Global; 
        }

        /**
         * @brief Set the global object
         * 
         * @param b 
         */
        void set_global(bool b) 
        { 
            set_bit(Global, b); 
        }

        bool is_execute_disabled() const 
        { 
            return (raw() & NoExecute) == NoExecute; 
        }

        /**
         * @brief Set the execute disabled object
         * 
         * @param b 
         */
        void set_execute_disabled(bool b) 
        { 
            set_bit(NoExecute, b); 
        }
        
        bool is_pat() const 
        { 
            return (raw() & PAT) == PAT; 
        }

        /**
         * @brief Set the pat object
         * 
         * @param b 
         */
        void set_pat(bool b) 
        { 
            set_bit(PAT, b); 
        }

        bool is_null() const 
        { 
            return m_raw == 0; 
        }

        void clear() 
        { 
            m_raw = 0; 
        }

    private:

        /**
         * @brief Set the bit object
         * 
         * @param bit 
         * @param value 
         */
        void set_bit(u64 bit, bool value)
        {
            if (value)
                m_raw |= bit;
            else
                m_raw &= ~bit;
        }

        u64 m_raw;
    }; // class PageTableEntry

    static_assert(AssertSize<PageDirectoryEntry, 8>());
    static_assert(AssertSize<PageTableEntry, 8>());

    class PageDirectoryPointerTable 
    {
    public:
        /**
         * @param index 
         * @return PageDirectoryEntry* 
         */
        PageDirectoryEntry* directory(size_t index)
        {
            VERIFY(index <= (NumericLimits<size_t>::max() << 30));
            return (PageDirectoryEntry*)(PhysicalAddress::physical_page_base(raw[index]));
        }

        u64 raw[512];
    }; // class PageDirectoryPointerTable
} // namespace Kernel
