/**
 * @file filedescription.h
 * @author Krisna Pranav
 * @brief File Description
 * @version 1.0
 * @date 2023-08-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/badge.h>
#include <mods/byte_buffer.h>
#include <mods/refcounted.h>
#include <kernel/kbuffer.h>
#include <kernel/virtual_address.h>
#include <kernel/filesystem/fifo.h>
#include <kernel/filesystem/inode.h>
#include <kernel/filesystem/inodemetadata.h>
#include <kernel/filesystem/virtualfilesystem.h>

namespace Kernel
{

    class FileDescription : public RefCounted<FileDescription> 
    {
        MAKE_SLAB_ALLOCATED(FileDescription)

    private:
        friend class VFS;

        /// @brief Construct a new File Description object
        explicit FileDescription(File&);

        FileDescription(FIFO&, FIFO::Direction);

        void evaluate_block_conditions()
        {
            block_condition().unblock();
        }

        RefPtr<Custody> m_custody;
        RefPtr<Inode> m_inode;

        NonnullRefPtr<File> m_file;

        off_t m_current_offset { 0 };

        Optional<KBuffer> m_generator_cache;

        u32 m_file_flags { 0 };

        bool m_readable : 1 { false };
        bool m_writable : 1 { false };
        bool m_is_blocking : 1 { true };
        bool m_is_directory : 1 { false };
        bool m_should_append : 1 { false };
        bool m_direct : 1 { false };

        FIFO::Direction m_fifo_direction { FIFO::Direction::Neither };

        Lock m_lock { "FileDescription" };


    } // class FileDescription

} // namespace Kernel