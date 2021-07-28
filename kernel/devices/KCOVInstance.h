/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/SpinLock.h>
#include <kernel/vm/AnonymousVMObject.h>

namespace Kernel {

typedef volatile u64 kcov_pc_t;
#define KCOV_ENTRY_SIZE sizeof(kcov_pc_t)
#define KCOV_MAX_ENTRIES (10 * 1024 * 1024)

class KCOVInstance final {
public:
    explicit KCOVInstance(ProcessID pid);

    KResult buffer_allocate(size_t buffer_size_in_entries);
    bool has_buffer() const { return m_buffer != nullptr; }
    void buffer_add_pc(u64 pc);

    SpinLock<u8> lock;
    enum {
        UNUSED = 0,
        OPENED = 1,
        TRACING = 2,
    } state;

    RefPtr<AnonymousVMObject> vmobject;

private:
    ProcessID m_pid = { 0 };
    u64 m_buffer_size_in_entries = { 0 };
    size_t m_buffer_size_in_bytes = { 0 };
    kcov_pc_t* m_buffer = { nullptr };

    OwnPtr<Region> m_kernel_region;
};

}