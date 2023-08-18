/**
 * @file filesystem.h
 * @author Krisna Pranav
 * @brief filesystem
 * @version 6.0
 * @date 2023-08-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/string.h>
#include <string_view.h>
#include <kernel/filesystem/inodeidentifier.h>
#include <kernel/lock.h>
#include <kernel/unixtypes.h>
#include <kernel/userorkernelbuffer.h>

namespace Kernel 
{

    static const u32 mepoch = 476763780;

    class Inode;
    class FileDescription;
    class LocalSocket;
    class VMObject;

    class FS : public RefCounted<FS> 
    {
        friend class Inode;

    public:
        virtual ~FS();

        /**
         * @return unsigned 
         */
        unsigned fsid() const 
        { 
            return m_fsid; 
        }

        /**
         * @return FS* 
         */
        static FS* from_fsid(u32);

        static void sync();
        static void lock_all();

        virtual bool initialize() = 0;

        /**
         * @return const char* 
         */
        virtual const char* class_name() const = 0;

        /**
         * @return NonnullRefPtr<Inode> 
         */
        virtual NonnullRefPtr<Inode> root_inode() const = 0;

        /**
         * @return true 
         * @return false 
         */
        virtual bool supports_watchers() const 
        { 
            return false; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_readonly() const 
        { 
            return m_readonly; 
        }

        /**
         * @return unsigned 
         */
        virtual unsigned total_block_count() const 
        { 
            return 0; 
        }

        virtual unsigned free_block_count() const 
        { 
            return 0; 
        }

        virtual unsigned total_inode_count() const 
        { 
            return 0; 
        }

        virtual unsigned free_inode_count() const 
        { 
            return 0; 
        }

        /**
         * @return KResult 
         */
        virtual KResult prepare_to_unmount() const 
        { 
            return KSuccess; 
        }

        struct DirectoryEntryView 
        {
            /**
             * @param name 
             * @param file_type 
             */
            DirectoryEntryView(const StringView& name, InodeIdentifier, u8 file_type);

            StringView name;

            InodeIdentifier inode;

            u8 file_type { 0 };
        }; // struct DirectoryEntryView

        virtual void flush_writes() { }

        /**
         * @return size_t 
         */
        size_t block_size() const 
        { 
            return m_block_size; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_file_backed() const 
        { 
            return false; 
        }

        /**
         * @param entry 
         * @return u8 
         */
        virtual u8 internal_file_type_to_directory_entry_type(const DirectoryEntryView& entry) const 
        { 
            return entry.file_type; 
        }

    protected:
        /// @brief Construct a new FS object
        FS();

        void set_block_size(size_t);

        mutable Lock m_lock { "FS" };

    private:
        unsigned m_fsid { 0 };

        size_t m_block_size { 0 };

        bool m_readonly { false };
    };

    /**
     * @return FS* 
     */
    inline FS* InodeIdentifier::fs()
    {
        return FS::from_fsid(m_fsid);
    }

    /**
     * @return const FS* 
     */
    inline const FS* InodeIdentifier::fs() const
    {
        return FS::from_fsid(m_fsid);
    }

} // namespace Kernel

namespace Mods
{

    template<>
    struct Traits<Kernel::InodeIdentifier> : public GenericTraits<Kernel::InodeIdentifier> 
    {   
        /**
         * @param inode 
         * @return unsigned 
         */
        static unsigned hash(const Kernel::InodeIdentifier& inode) 
        { 
            return pair_int_hash(inode.fsid(), inode.index()); 
        }
    };

} // namspace Mods 
