/**
 * @file doublebuffer.cpp
 * @author Krisna Pranav
 * @brief double buffer
 * @version 6.0
 * @date 2023-09-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string_view.h>
#include <kernel/doublebuffer.h>

namespace Kernel 
{

    /**
     * @brief ::compute_lockfree_metadata()
     */
    inline void DoubleBuffer::compute_lockfree_metadata()
    {
        InterruptDisabler disabler;
        
        m_empty = m_read_buffer_index >= m_read_buffer->size && m_write_buffer->size == 0;
        m_space_for_writing = m_capacity - m_write_buffer->size;
    }

    /**
     * @param capacity 
     */
    DoubleBuffer::DoubleBuffer(size_t capacity)
        : m_write_buffer(&m_buffer1)
        , m_read_buffer(&m_buffer2)
        , m_storage(KBuffer::create_with_size(capacity * 2, Region::Access::Read | Region::Access::Write, "DoubleBuffer"))
        , m_capacity(capacity)
    {
        m_buffer1.data = m_storage.data();
        m_buffer1.size = 0;
        m_buffer2.data = m_storage.data() + capacity;
        m_buffer2.size = 0;
        m_space_for_writing = capacity;
    }

    /**
     * @brief ::flip()
     * 
     */
    void DoubleBuffer::flip()
    {
        ASSERT(m_read_buffer_index == m_read_buffer->size);
        swap(m_read_buffer, m_write_buffer);

        m_write_buffer->size = 0;
        m_read_buffer_index = 0;

        compute_lockfree_metadata();
    }

    /**
     * @param data 
     * @param size 
     * @return ssize_t 
     */
    ssize_t DoubleBuffer::write(const UserOrKernelBuffer& data, size_t size)
    {
        if (!size)
            return 0;

        ASSERT(size > 0);
        LOCKER(m_lock);

        size_t bytes_to_write = min(size, m_space_for_writing);
        u8* write_ptr = m_write_buffer->data + m_write_buffer->size;

        m_write_buffer->size += bytes_to_write;

        compute_lockfree_metadata();

        if (!data.read(write_ptr, bytes_to_write))
            return -EFAULT;

        if (m_unblock_callback && !m_empty)
            m_unblock_callback();

        return (ssize_t)bytes_to_write;
    }

    /**
     * @param data 
     * @param size 
     * @return ssize_t 
     */
    ssize_t DoubleBuffer::read(UserOrKernelBuffer& data, size_t size)
    {
        if (!size)
            return 0;

        ASSERT(size > 0);
        LOCKER(m_lock);
        
        if (m_read_buffer_index >= m_read_buffer->size && m_write_buffer->size != 0)
            flip();

        if (m_read_buffer_index >= m_read_buffer->size)
            return 0;

        size_t nread = min(m_read_buffer->size - m_read_buffer_index, size);

        if (!data.write(m_read_buffer->data + m_read_buffer_index, nread))
            return -EFAULT;
            
        m_read_buffer_index += nread;

        compute_lockfree_metadata();

        if (m_unblock_callback && m_space_for_writing > 0)
            m_unblock_callback();
            
        return (ssize_t)nread;
    }

} // namespace Kernel