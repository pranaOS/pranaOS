/**
 * @file circularduplex_stream.h
 * @author Krisna Pranav
 * @brief CircularDuplexStream
 * @version 6.0
 * @date 2023-07-05
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once


#include <mods/circularqueue.h>
#include <mods/stream.h>

namespace Mods
{
    /**
     * @tparam Capacity 
     */
    template<size_t Capacity>
    class CircularDuplexStream : public Mods::DuplexStream 
    {
    public:
        /**
         * @param bytes 
         * @return size_t 
         */
        size_t write(ReadonlyBytes bytes) override
        {
            auto const nwritten = min(bytes.size(), Capacity - m_queue.size());

            for (size_t idx = 0; idx < nwritten; ++idx)
                m_queue.enqueue(bytes[idx]);

            m_total_written += nwritten;
            return nwritten;
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool write_or_error(ReadonlyBytes bytes) override
        {
            if (Capacity - m_queue.size() < bytes.size()) {
                set_recoverable_error();
                return false;
            }

            auto const nwritten = write(bytes);
            VERIFY(nwritten == bytes.size());

            return true;
        }

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t read(Bytes bytes) override
        {
            if (has_any_error())
                return 0;

            auto const nread = min(bytes.size(), m_queue.size());

            for (size_t idx = 0; idx < nread; ++idx)
                bytes[idx] = m_queue.dequeue();

            return nread;
        }

        /**
         * @param bytes 
         * @param seekback 
         * @return size_t 
         */
        size_t read(Bytes bytes, size_t seekback)
        {
            if (seekback > Capacity || seekback > m_total_written) {
                set_recoverable_error();
                return 0;
            }

            auto const nread = min(bytes.size(), seekback);

            for (size_t idx = 0; idx < nread; ++idx) {
                auto const index = (m_total_written - seekback + idx) % Capacity;
                bytes[idx] = m_queue.m_storage[index];
            }

            return nread;
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool read_or_error(Bytes bytes) override
        {
            if (m_queue.size() < bytes.size()) {
                set_recoverable_error();
                return false;
            }

            read(bytes);

            return true;
        }

        /**
         * @param count 
         * @return true 
         * @return false 
         */
        bool discard_or_error(size_t count) override
        {
            if (m_queue.size() < count) {
                set_recoverable_error();
                return false;
            }

            for (size_t idx = 0; idx < count; ++idx)
                m_queue.dequeue();

            return true;
        }

        /**
         * @return true 
         * @return false 
         */
        bool unreliable_eof() const override 
        { 
            return eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool eof() const 
        { 
            return m_queue.size() == 0; 
        }

        /**
         * @return size_t 
         */
        size_t remaining_contiguous_space() const
        {
            return min(Capacity - m_queue.size(), m_queue.capacity() - (m_queue.head_index() + m_queue.size()) % Capacity);
        }

        /**
         * @param count 
         * @return Bytes 
         */
        Bytes reserve_contiguous_space(size_t count)
        {
            VERIFY(count <= remaining_contiguous_space());

            Bytes bytes { m_queue.m_storage + (m_queue.head_index() + m_queue.size()) % Capacity, count };

            m_queue.m_size += count;
            m_total_written += count;

            return bytes;
        }

    private:
        CircularQueue<u8, Capacity> m_queue;
        size_t m_total_written { 0 };
    }; // class CircularDuplexStream : public Mods::DuplexStream 
} // namespace Mods

using Mods::CircularDuplexStream;
