/**
 * @file filedescription.h
 * @author Krisna Pranav
 * @brief File Description
 * @version 6.0
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
    public:

        /**
         * @return NonnullRefPtr<FileDescription> 
         */
        static NonnullRefPtr<FileDescription> create(Custody&);
        static NonnullRefPtr<FileDescription> create(File&);

        /// @brief Destroy the File Description object
        ~FileDescription();

        /**
         * @return Thread::FileBlocker::BlockFlags 
         */
        Thread::FileBlocker::BlockFlags should_unblock(Thread::FileBlocker::BlockFlags) const;

        /**
         * @return true 
         * @return false 
         */
        bool is_readable() const 
        { 
            return m_readable; 
        }

        bool is_writable() const 
        { 
            return m_writable; 
        }

        /**
         * @param b 
         */
        void set_readable(bool b) 
        { 
            m_readable = b; 
        }

        void set_writable(bool b) 
        { 
            m_writable = b; 
        }

        /**
         * @param options 
         */
        void set_rw_mode(int options)
        {
            set_readable(options & O_RDONLY);
            set_writable(options & O_WRONLY);
        }

        /**
         * @return KResult 
         */
        KResult close();

        /**
         * @param whence 
         * @return off_t 
         */
        off_t seek(off_t, int whence);

        /**
         * @return KResultOr<size_t> 
         */
        KResultOr<size_t> read(UserOrKernelBuffer&, size_t);
        KResultOr<size_t> write(const UserOrKernelBuffer& data, size_t);

        /**
         * @return KResult 
         */
        KResult stat(::stat&);
        KResult chmod(mode_t);

        /**
         * @return true 
         * @return false 
         */
        bool can_read() const;
        bool can_write() const;

        /**
         * @param buffer 
         * @return ssize_t 
         */
        ssize_t get_dir_entries(UserOrKernelBuffer& buffer, ssize_t);

        /**
         * @return KResultOr<KBuffer> 
         */
        KResultOr<KBuffer> read_entire_file();

        /**
         * @return String 
         */
        String absolute_path() const;

        /**
         * @return true 
         * @return false 
         */
        bool is_direct() const 
        { 
            return m_direct; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_directory() const 
        { 
            return m_is_directory; 
        }

        /**
         * @return File& 
         */
        File& file() 
        { 
            return *m_file; 
        }

        /**
         * @return const File& 
         */
        const File& file() const 
        { 
            return *m_file; 
        }

        bool is_device() const;

        /**
         * @return const Device* 
         */
        const Device* device() const;

        /**
         * @return Device* 
         */
        Device* device();

        /// @brief: tty
        bool is_tty() const;
        const TTY* tty() const;
        TTY* tty();

        
        /// @breif: masterptty
        bool is_master_pty() const;
        const MasterPTY* master_pty() const;
        MasterPTY* master_pty();

        /**
         * @return InodeMetadata 
         */
        InodeMetadata metadata() const;

        /**
         * @return Inode* 
         */
        Inode* inode() 
        { 
            return m_inode.ptr(); 
        }

        /**
         * @return const Inode* 
         */
        const Inode* inode() const 
        { 
            return m_inode.ptr(); 
        }

        /**
         * @return Custody* 
         */
        Custody* custody() 
        { 
            return m_custody.ptr(); 
        }

        /**
         * @return const Custody* 
         */
        const Custody* custody() const 
        { 
            return m_custody.ptr(); 
        }   

        /**
         * @param offset 
         * @param prot 
         * @param shared 
         * @return KResultOr<Region*> 
         */
        KResultOr<Region*> mmap(Process&, VirtualAddress, size_t offset, size_t, int prot, bool shared);

        /**
         * @return true 
         * @return false 
         */
        bool is_blocking() const 
        { 
            return m_is_blocking; 
        }

        /**
         * @param b 
         */
        void set_blocking(bool b) 
        { 
            m_is_blocking = b; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool should_append() const 
        { 
            return m_should_append; 
        }

        /**
         * @param s 
         */
        void set_should_append(bool s) 
        { 
            m_should_append = s; 
        }

        u32 file_flags() const 
        { 
            return m_file_flags; 
        }

        void set_file_flags(u32);

        bool is_socket() const;
        Socket* socket();
        const Socket* socket() const;

        bool is_fifo() const;
        FIFO* fifo();
        FIFO::Direction fifo_direction() 
        { 
            return m_fifo_direction; 
        }

        void set_fifo_direction(Badge<FIFO>, FIFO::Direction direction) 
        { 
            m_fifo_direction = direction; 
        }

        Optional<KBuffer>& generator_cache() 
        { 
            return m_generator_cache; 
        }

        void set_original_inode(Badge<VFS>, NonnullRefPtr<Inode>&& inode) 
        { 
            m_inode = move(inode); 
        }

        KResult truncate(u64);

        off_t offset() const 
        { 
            return m_current_offset; 
        }

        KResult chown(uid_t, gid_t);

        FileBlockCondition& block_condition();

    private:
        friend class VFS;
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
    };

} // namespace Kernel
