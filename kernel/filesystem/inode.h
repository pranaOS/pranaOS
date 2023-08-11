/**
 * @file inode.h
 * @author Krisna Pranav
 * @brief inode
 * @version 1.0
 * @date 2023-08-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once


#include <mods/function.h>
#include <mods/hashtable.h>
#include <mods/inlinelinkedlist.h>
#include <mods/refcounted.h>
#include <mods/string.h>
#include <mods/weakptr.h>
#include <kernel/forward.h>
#include <kernel/kresult.h>
#include <kernel/lock.h>
#include <kernel/filesystem/fifo.h>
#include <kernel/filesystem/filesystem.h>
#include <kernel/filesystem/inodeidentifier.h>
#include <kernel/filesystem/inodemetadata.h>

namespace Kernel
{
    class Inode : public RefCounted<Inode>
        , public Weakable<Inode>
        , public InlineLinkedList<Inode> {

    public:
        virtual ~Inode();

        FS& fs() 
        {
            return m_fs;
        }

        const FS& fs() const 
        {
            return m_fs;
        }

        /**
         * @return unsigned 
         */
        unsigned fsid() const
        {
            return m_fs.fsid();
        }

        unsigned index() const
        {
            return m_index;
        }

        /**
         * @return size_t 
         */
        size_t size() const
        {
            return metadata().size;
        }

        /**
         * @return InodeIdentifier 
         */
        InodeIdentifier identifier() const
        {
            return { fsid(), index(); }
        }

        /**
         * @return InodeMetadata 
         */
        virtual InodeMetadata metadata() const = 0;

    protected:
        /**
         * @param fs 
         * @param index 
         */
        Inode(FS& fs, unsigned index);
        void set_metadata_dirty(bool);

        void inode_contents_changed(off_t, ssize_t, const UserOrKernelBuffer);

        /**
         * @param old_size 
         * @param new_size 
         */
        void inode_size_changed(size_t old_size, size_t new_size);

        /**
         * @return KResult 
         */
        KResult prepare_to_write_data();

        void did_all_child(const InodeIdentifier&);


        mutable Lock m_lock { "Inode" };

    private:
        FS& m_fs;

        unsigned m_index { 0 };

        WeakPtr<SharedInodeVMObject> m_shared_vmobject;

        RefPtr<LocalSocket> m_socket;

        HashTable<InodeWatcher*> m_watchers;

        bool m_metadata_dirty { false };

        RefPtr<FIFO> m_fifo;

    }; // class Inode
} // namespace Kernel