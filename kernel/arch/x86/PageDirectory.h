/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Badge.h>
#include <base/Types.h>
#include <kernel/PhysicalAddress.h>

namespace Kernel {

class PageDirectory;
class PageTableEntry;

class PageDirectoryEntry {
public:

    PhysicalPtr page_table_base() const { return PhysicalAddress::phyiscal_page_base(m_raw); }

    bool is_null() const { return m_raw == 0; }
    void clear() { m_raw = 0; }

    void copy_from(Badge<PageDirectory>, const PageDirectoryEntry& other) { m_raw = other.m_raw; }

        void set_present(bool b) { set_bit(Present, b); }

    bool is_user_allowed() const { return raw() & UserSupervisor; }
    void set_user_allowed(bool b) { set_bit(UserSupervisor, b); }

    bool is_huge() const { return raw() & Huge; }
    void set_huge(bool b) { set_bit(Huge, b); }

    bool is_writable() const { return raw() & ReadWrite; }
    void set_writable(bool b) { set_bit(ReadWrite, b); }

    bool is_write_through() const { return raw() & WriteThrough; }
    void set_write_through(bool b) { set_bit(WriteThrough, b); }

    bool is_cache_disabled() const { return raw() & CacheDisabled; }
    void set_cache_disabled(bool b) { set_bit(CacheDisabled, b); }

    bool is_global() const { return raw() & Global; }
    void set_global(bool b) { set_bit(Global, b); }

    bool is_execute_disabled() const { return raw() & NoExecute; }
    void set_execute_disabled(bool b) { set_bit(NoExecute, b); }

    void set_bit(u64 bit, bool value)
    {
        if (value)
            m_raw |= bit;
        else
            m_raw &= ~bit;
    }

};

}