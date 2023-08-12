/**
 * @file blockfilesystem.h
 * @author Krisna Pranav
 * @brief block file system
 * @version 1.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/filesystem/filebackedfilesystem.h>

namespace Kernel
{
    class BlockBasedFS : public FileBackedFS
    {
    protected:
        explicit BlockBasedFS(FileDescription&);

        /**
         * @param index 
         * @param buffer 
         * @param count 
         * @param offset 
         * @param allow_cache 
         * @return int 
         */
        int read_block(unsigned index, UserOrKernelBuffer* buffer, size_t count, size_t offset = 0, bool allow_cache = true) const;

        /**
         * @param index 
         * @param count 
         * @param buffer 
         * @param allow_cache 
         * @return int 
         */
        int read_blocks(unsigned index, unsigned count, UserOrKernelBuffer& buffer, bool allow_cache = true) const;

        /**
         * @param index 
         * @param buffer 
         * @return true 
         * @return false 
         */
        bool raw_read(unsigned index, UserOrKernelBuffer& buffer);

        /**
         * @param index 
         * @param buffer 
         * @return true 
         * @return false 
         */
        bool raw_write(unsigned index, const UserOrKernelBuffer& buffer);

        size_t m_logical_block_size { 512 };
        
    private:

        /**
         * @return DiskCache& 
         */
        DiskCache& cache() const;

        /**
         * @param index 
         */
        void flush_specific_block_if_needed(unsigned index);

        mutable OwnPtr<DiskCache> m_cache;
    }; // class BlockBasedFS
} // namespace Kernel