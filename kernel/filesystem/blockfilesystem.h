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
    public:
        /// @brief Destroy the BlockBasedFS object
        virtual ~BlockBasedFS() override;

        /**
         * @return size_t 
         */
        size_t logical_block_size() const 
        { 
            return m_logical_block_size; 
        };

        /// @brief: flush writes + impl
        virtual void flush_writes() override;
        void flush_writes_impl();

    protected:
        /// @brief Construct a new BlockBasedFS object
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

        /**
         * @param index 
         * @param count 
         * @param buffer 
         * @return true 
         * @return false 
         */
        bool raw_read_blocks(unsigned index, size_t count, UserOrKernelBuffer& buffer);

        /**
         * @param index 
         * @param count 
         * @param buffer 
         * @return true 
         * @return false 
         */
        bool raw_write_blocks(unsigned index, size_t count, const UserOrKernelBuffer& buffer);
        
        /**
         * @param index 
         * @param buffer 
         * @param count 
         * @param offset 
         * @param allow_cache 
         * @return int 
         */
        int write_block(unsigned index, const UserOrKernelBuffer& buffer, size_t count, size_t offset = 0, bool allow_cache = true);

        /**
         * @param index 
         * @param count 
         * @param allow_cache 
         * @return int 
         */
        int write_blocks(unsigned index, unsigned count, const UserOrKernelBuffer&, bool allow_cache = true);

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

        /// @brief: DiskCache -> m_cache
        mutable OwnPtr<DiskCache> m_cache;

    }; // class BlockBasedFS

} // namespace Kernel