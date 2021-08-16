/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <kernel/KBuffer.h>
#include <kernel/locking/Mutex.h>
#include <kernel/Thread.h>
#include <kernel/UserOrKernelBuffer.h>

namespace Kernel {

class DoubleBuffer {
public:
    [[nodiscard]] static OwnPtr<DoubleBuffer> try_create(size_t capacity = 65536);
    [[nodiscard]] KResultOr<size_t> write(const UserOrKernelBuffer&, size_t);
    [[nodiscard]] KResultOr<size_t> write(const u8* data, size_t size)
    {
        return write(UserOrKernelBuffer::for_kernel_buffer(const_cast<u8*>(data)), size);
    }
    [[nodiscard]] KResultOr<size_t> read(UserOrKernelBuffer&, size_t);
    [[nodiscard]] KResultOr<size_t> read(u8* data, size_t size)
    {
        auto buffer = UserOrKernelBuffer::for_kernel_buffer(data);
        return read(buffer, size);
    }
    [[nodiscard]] KResultOr<size_t> peek(UserOrKernelBuffer&, size_t);
    [[nodiscard]] KResultOr<size_t> peek(u8* data, size_t size)
    {
        auto buffer = UserOrKernelBuffer::for_kernel_buffer(data);
        return peek(buffer, size);
    }

    bool is_empty() const { return m_empty; }

    size_t space_for_writing() const { return m_space_for_writing; }

    void set_unblock_callback(Function<void()> callback)
    {
        VERIFY(!m_unblock_callback);
        m_unblock_callback = move(callback);
    }

private:
    explicit DoubleBuffer(size_t capacity, NonnullOwnPtr<KBuffer> storage);
    void flip();
    void compute_lockfree_metadata();

    struct InnerBuffer {
        u8* data { nullptr };
        size_t size;
    };

    InnerBuffer* m_write_buffer { nullptr };
    InnerBuffer* m_read_buffer { nullptr };
    InnerBuffer m_buffer1;
    InnerBuffer m_buffer2;

    NonnullOwnPtr<KBuffer> m_storage;
    Function<void()> m_unblock_callback;
    size_t m_capacity { 0 };
    size_t m_read_buffer_index { 0 };
    size_t m_space_for_writing { 0 };
    bool m_empty { true };
    mutable Mutex m_lock { "DoubleBuffer" };
};

}
