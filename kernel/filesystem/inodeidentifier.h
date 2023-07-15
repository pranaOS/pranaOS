/**
 * @file inodeidentifier.h
 * @author Krisna Pranav
 * @brief inodeIdentifier
 * @version 1.0
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/byte_buffer.h>
#include <mods/string.h>
#include <mods/types.h>

namespace Kernel
{
    class FS;
    class InodeMetadata;

    class InodeIdentifier {
    public:
        InodeIdentifier() {}

        InodeIdentifier(u32 fsid, u32 inode) : m_fsid(fsid), m_index(index)
        {}

        /** 
         * @return true 
         * @return false 
         */
        bool is_valid() const
        {
            return m_fsid;
        }

        /**
         * @return u32 
         */
        u32 fsid() const
        {
            return m_fsid;
        }

        /**
         * @return u32 
         */
        u32 index() const
        {
            return m_index;
        }

        /**
         * @return FS* 
         */
        FS* fs();

        /**
         * @return const FS* 
         */
        const FS* fs() const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const InodeIdentifier& other) const
        {
            return m_fsid == other.m_fsid && m_index == other.m_index;
        }

        /**
         * @return String 
         */
        String to_string() const
        {
            return String::format("%u:%u", m_fsid, m_index);
        }

    private:
        u32 m_fsid { 0 };
        u32 m_index { 0 };
    };
}