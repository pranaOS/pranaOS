/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/EnumBits.h>
#include <base/IntrusiveList.h>
#include <base/Weakable.h>
#include <kernel/arch/x86/PageFault.h>
#include <kernel/Forward.h>
#include <kernel/heap/SlabAllocator.h>
#include <kernel/KString.h>
#include <kernel/Sections.h>
#include <kernel/UnixTypes.h>
#include <kernel/vm/PageFaultResponse.h>
#include <kernel/vm/RangeAllocator.h>

namespace Kernel {

enum class ShouldFlashTLB {
    No,
    Yes,
};

class Region final
    : public Weakable<Region> {
    friend class MemoryManager;

    MAKE_SLAB_ALLOCATED(Region)
public:
        enum Access : u8 {
        None = 0,
        Read = 1,
        Write = 2,
        Execute = 4,
        HasBeenReadable = 16,
        HasBeenWritable = 32,
        HasBeenExecutable = 64,
    };

    enum class Cacheable {
        No = 0,
        Yes,
    };

    static OwnPtr<Region> try_create_user_accessible(Range const&, NonnullRefPtr<VMObject>, size_t offset_in_vmobject, OwnPtr<KString> name, Region::Access access, Cacheable, bool shared);
    static OwnPtr<Region> try_create_kernel_only(Range const&, NonnullRefPtr<VMObject>, size_t offset_in_vmobject, OwnPtr<KString> name, Region::Access access, Cacheable = Cacheable::Yes);

    ~Region();

    Range const& range() const { return m_range; }
    VirtualAddress vaddr() const { return m_range.base(); }
    size_t size() const { return m_range.size(); }
    bool is_readable() const { return m_access & Access::Read; }
    bool is_writable() const { return m_access & Access::Write; }
    bool is_executable() const { return m_access & Access::Execute; }

    bool has_been_readable() const { return m_access & Access::HasBeenReadable; }
    bool has_been_writable() const { return m_access & Access::HasBeenWritable; }
    bool has_been_executable() const { return m_access & Access::HasBeenExecutable; }

    bool is_cacheable() const { return m_cacheable; }
    StringView name() const { return m_name ? m_name->view() : StringView {}; }
    OwnPtr<KString> take_name() { return move(m_name); }
    Region::Access access() const { return static_cast<Region::Access>(m_access); }

    void set_name(OwnPtr<KString> name) { m_name = move(name); }

    VMObject const& vmobject() const { return *m_vmobject; }
    VMObject& vmobject() { return *m_vmobject; }
    void set_vmobject(NonnullRefPtr<VMObject>&&);

    bool is_shared() const { return m_shared; }
    void set_shared(bool shared) { m_shared = shared; }

    bool is_stack() const { return m_stack; }
    void set_stack(bool stack) { m_stack = stack; }

    bool is_mmap() const { return m_mmap; }
    void set_mmap(bool mmap) { m_mmap = mmap; }

    bool is_user() const { return !is_kernel(); }
    bool is_kernel() const { return vaddr().get() < 0x00800000 || vaddr().get() >= kernel_base; }

    PageFaultResponse handle_fault(PageFault const&);

    OwnPtr<Region> clone();

    }

}