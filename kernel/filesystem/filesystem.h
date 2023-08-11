/**
 * @file filesystem.h
 * @author Krisna Pranav
 * @brief filesystem
 * @version 1.0
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
         * @return KResult 
         */
        virtual KResult prepare_to_unmount() const
        {
            return KSuccess;
        }

        struct DirectoryEntryView 
        {
            DirectoryEntryView(const StringView& name, InodeIdentifier, u8 file_type);

            StringView name;
            InodeIdentifier inode;
            u8 file_type { 0 };
        }; // struct

        /**
         * @return size_t 
         */
        size_t block_size() const
        {
            return m_block_size;
        }


    protected:
        FS();
        
        void set_block_size(size_t);

        mutable Lock m_lock { "FS" };

    private:
        unsigned m_fsid { 0 };
        size_t m_block_size { 0 };
        bool m_readonly { false };
    }; // class FS
    
} // namespace Kernel