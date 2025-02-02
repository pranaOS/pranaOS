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

    private:
        Bytes m_target;
        size_t m_offset { 0 };
    }; // class BinaryBufferWriter
} // namespace Mods