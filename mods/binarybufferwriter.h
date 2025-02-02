/**
 * @file binarybufferwriter.h
 * @author Krisna Pranav
 * @brief Binary Buffer Writer
 * @version 6.0
 * @date 2025-02-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>
#include <mods/stdlibextradetail.h>

namespace Mods
{
    class BinaryBufferWriter
    {
    public:
        /**
         * @brief Construct a new Binary Buffer Writer object
         * 
         * @param target 
         */
        BinaryBufferWriter(Bytes target)
            : m_target(target)
        {}

        template<typename T>
        requires(Mods::Detail::IsTriviallyConstructible<T>) T& append_structure()
        {
            VERIFY((reinterpret_cast<FlatPtr>(m_target.data()) + offset))
            T* allocated = new (m_target.data() + m_offset) T;
            m_offset += sizeof(T);
            return *allocated;
        }

        /**
         * @param num_bytes 
         */
        void skip_bytes(size_t num_bytes)
        {
            VERIFY(m_offset + num_bytes <= m_target.size());
            m_offset += num_bytes;
        }

    private:
        Bytes m_target;
        size_t m_offset { 0 };
    }; // class BinaryBufferWriter
} // namespace Mods