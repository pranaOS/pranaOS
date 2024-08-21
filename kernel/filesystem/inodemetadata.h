/**
 * @file inodemetadata.h
 * @author Krisna Pranav
 * @brief InodeMetadata
 * @version 6.0
 * @date 2023-07-15
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/span.h>
#include <kernel/filesystem/inodeidentifier.h>
#include <kernel/kresult.h>
#include <kernel/unixtypes.h>

namespace Kernel 
{

    class Process;

    /**
     * @param major 
     * @param minor 
     * @return constexpr u32 
     */
    constexpr u32 encoded_device(unsigned major, unsigned minor)
    {
        return (minor & 0xff) | (major << 8) | ((minor & ~0xff) << 12);
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_directory(mode_t mode) 
    { 
        return (mode & S_IFMT) == S_IFDIR; 
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_character_device(mode_t mode) 
    { 
        return (mode & S_IFMT) == S_IFCHR; 
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_block_device(mode_t mode) 
    { 
        return (mode & S_IFMT) == S_IFBLK; 
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_regular_file(mode_t mode) 
    { 
        return (mode & S_IFMT) == S_IFREG; 
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_fifo(mode_t mode) 
    { 
        return (mode & S_IFMT) == S_IFIFO; 
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_symlink(mode_t mode) 
    { 
        return (mode & S_IFMT) == S_IFLNK; 
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_socket(mode_t mode) 
    { 
        return (mode & S_IFMT) == S_IFSOCK; 
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_sticky(mode_t mode) 
    { 
        return mode & S_ISVTX; 
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_setuid(mode_t mode) 
    { 
        return mode & S_ISUID; 
    }

    /**
     * @param mode 
     * @return true 
     * @return false 
     */
    inline bool is_setgid(mode_t mode) 
    { 
        return mode & S_ISGID; 
    }

    struct InodeMetadata {

        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const 
        { 
            return inode.is_valid(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool may_read(const Process&) const;

        /**
         * @return true 
         * @return false 
         */
        bool may_write(const Process&) const;

        /**
         * @return true 
         * @return false 
         */
        bool may_execute(const Process&) const;

        /**
         * @param u 
         * @param g 
         * @param eg 
         * @return true 
         * @return false 
         */
        bool may_read(uid_t u, gid_t g, Span<const gid_t> eg) const
        {
            if (u == 0)
                return true;
            if (uid == u)
                return mode & S_IRUSR;
            if (gid == g || eg.contains_slow(gid))
                return mode & S_IRGRP;
            return mode & S_IROTH;
        }

        /**
         * @param u 
         * @param g 
         * @param eg 
         * @return true 
         * @return false 
         */
        bool may_write(uid_t u, gid_t g, Span<const gid_t> eg) const
        {
            if (u == 0)
                return true;
            if (uid == u)
                return mode & S_IWUSR;
            if (gid == g || eg.contains_slow(gid))
                return mode & S_IWGRP;
            return mode & S_IWOTH;
        }

        /**
         * @param u 
         * @param g 
         * @param eg 
         * @return true 
         * @return false 
         */
        bool may_execute(uid_t u, gid_t g, Span<const gid_t> eg) const
        {
            if (u == 0)
                return true;
            if (uid == u)
                return mode & S_IXUSR;
            if (gid == g || eg.contains_slow(gid))
                return mode & S_IXGRP;
            return mode & S_IXOTH;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_directory() const 
        { 
            return Kernel::is_directory(mode); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_character_device() const 
        { 
            return Kernel::is_character_device(mode); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_block_device() const 
        { 
            return Kernel::is_block_device(mode); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_device() const 
        { 
            return is_character_device() || is_block_device(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_regular_file() const 
        { 
            return Kernel::is_regular_file(mode); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_fifo() const 
        { 
            return Kernel::is_fifo(mode); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_symlink() const 
        { 
            return Kernel::is_symlink(mode); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_socket() const 
        { 
            return Kernel::is_socket(mode); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_sticky() const 
        { 
            return Kernel::is_sticky(mode); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_setuid() const 
        { 
            return Kernel::is_setuid(mode); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_setgid() const 
        { 
            return Kernel::is_setgid(mode); 
        }

        /**
         * @param buffer 
         * @return KResult 
         */
        KResult stat(stat& buffer) const
        {
            if (!is_valid())
                return KResult(-EIO);
            buffer.st_rdev = encoded_device(major_device, minor_device);
            buffer.st_ino = inode.index();
            buffer.st_mode = mode;
            buffer.st_nlink = link_count;
            buffer.st_uid = uid;
            buffer.st_gid = gid;
            buffer.st_dev = 0; 
            buffer.st_size = size;
            buffer.st_blksize = block_size;
            buffer.st_blocks = block_count;
            buffer.st_atime = atime;
            buffer.st_mtime = mtime;
            buffer.st_ctime = ctime;
            return KSuccess;
        }

        InodeIdentifier inode;
        off_t size { 0 };
        mode_t mode { 0 };
        uid_t uid { 0 };
        gid_t gid { 0 };
        nlink_t link_count { 0 };
        time_t atime { 0 };
        time_t ctime { 0 };
        time_t mtime { 0 };
        time_t dtime { 0 };
        blkcnt_t block_count { 0 };
        blksize_t block_size { 0 };
        unsigned major_device { 0 };
        unsigned minor_device { 0 };
    };

}
