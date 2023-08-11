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
        , public InlineLinkedListNode<Inode> {
        friend class VFS;
        friend class FS;

    public:
        virtual ~Inode();

        virtual void one_ref_left() { }

        /**
         * @return FS& 
         */
        FS& fs() 
        { 
            return m_fs; 
        }
        
        /**
         * @return const FS& 
         */
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
         * @return true 
         * @return false 
         */
        bool is_symlink() const 
        { 
            return metadata().is_symlink(); 
        }

        bool is_directory() const 
        { 
            return metadata().is_directory(); 
        }

        bool is_character_device() const 
        { 
            return metadata().is_character_device(); 
        }

        /**
         * @return mode_t 
         */
        mode_t mode() const 
        { 
            return metadata().mode; 
        }

        /**
         * @return InodeIdentifier 
         */
        InodeIdentifier identifier() const 
        { 
            return { fsid(), index() }; 
        }

        /**
         * @return InodeMetadata 
         */
        virtual InodeMetadata metadata() const = 0;

        /**
         * @return KResultOr<KBuffer> 
         */
        KResultOr<KBuffer> read_entire(FileDescription* = nullptr) const;

        /**
         * @param buffer 
         * @return ssize_t 
         */
        virtual ssize_t read_bytes(off_t, ssize_t, UserOrKernelBuffer& buffer, FileDescription*) const = 0;

        /**
         * @return KResult 
         */
        virtual KResult traverse_as_directory(Function<bool(const FS::DirectoryEntryView&)>) const = 0;

        /**
         * @param name 
         * @return RefPtr<Inode> 
         */
        virtual RefPtr<Inode> lookup(StringView name) = 0;

        /**
         * @param data 
         * @return ssize_t 
         */
        virtual ssize_t write_bytes(off_t, ssize_t, const UserOrKernelBuffer& data, FileDescription*) = 0;

        /**
         * @param name 
         * @return KResultOr<NonnullRefPtr<Inode>> 
         */
        virtual KResultOr<NonnullRefPtr<Inode>> create_child(const String& name, mode_t, dev_t, uid_t, gid_t) = 0;

        /**
         * @param name 
         * @return KResult 
         */
        virtual KResult add_child(Inode&, const StringView& name, mode_t) = 0;

        /**
         * @param name 
         * @return KResult 
         */
        virtual KResult remove_child(const StringView& name) = 0;

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> directory_entry_count() const = 0;

        /**
         * @return KResult 
         */
        virtual KResult chmod(mode_t) = 0;

        /**
         * @return KResult 
         */
        virtual KResult chown(uid_t, gid_t) = 0;

        /**
         * @return KResult 
         */
        virtual KResult truncate(u64) 
        { 
            return KSuccess; 
        }

        /**
         * @param base 
         * @param out_parent 
         * @param options 
         * @param symlink_recursion_level 
         * @return KResultOr<NonnullRefPtr<Custody>> 
         */
        virtual KResultOr<NonnullRefPtr<Custody>> resolve_as_link(Custody& base, RefPtr<Custody>* out_parent = nullptr, int options = 0, int symlink_recursion_level = 0) const;

        /**
         * @return LocalSocket* 
         */
        LocalSocket* socket() 
        { 
            return m_socket.ptr(); 
        }

        /**
         * @return const LocalSocket* 
         */
        const LocalSocket* socket() const 
        { 
            return m_socket.ptr(); 
        }
        
        /**
         * @return true 
         * @return false 
         */
        bool bind_socket(LocalSocket&);
        bool unbind_socket();

        /**
         * @return FileDescription* 
         */
        virtual FileDescription* preopen_fd() 
        { 
            return nullptr;
        };

        /**
         * @return true 
         * @return false 
         */
        bool is_metadata_dirty() const 
        { 
            return m_metadata_dirty; 
        }

        /**
         * @return int 
         */
        virtual int set_atime(time_t);
        virtual int set_ctime(time_t);
        virtual int set_mtime(time_t);

        /**
         * @return KResult 
         */
        virtual KResult increment_link_count();
        virtual KResult decrement_link_count();

        virtual void flush_metadata() = 0;

        void will_be_destroyed();

        void set_shared_vmobject(SharedInodeVMObject&);

        /**
         * @return RefPtr<SharedInodeVMObject> 
         */
        RefPtr<SharedInodeVMObject> shared_vmobject();
        RefPtr<SharedInodeVMObject> shared_vmobject() const;

        /**
         * @return true 
         * @return false 
         */
        bool is_shared_vmobject(const SharedInodeVMObject&) const;

        /**
         * @return InlineLinkedList<Inode>& 
         */
        static InlineLinkedList<Inode>& all_with_lock();

        static void sync();

        /**
         * @return true 
         * @return false 
         */
        bool has_watchers() const 
        { 
            return !m_watchers.is_empty(); 
        }

        void register_watcher(Badge<InodeWatcher>, InodeWatcher&);
        void unregister_watcher(Badge<InodeWatcher>, InodeWatcher&);

        /**
         * @return FIFO& 
         */
        FIFO& fifo();

        Inode* m_next { nullptr };
        Inode* m_prev { nullptr };

        /**
         * @return SpinLock<u32>& 
         */
        static SpinLock<u32>& all_inodes_lock();

    protected:
        /**
         * @param fs 
         * @param index 
         */
        Inode(FS& fs, unsigned index);

        void set_metadata_dirty(bool);
        void inode_contents_changed(off_t, ssize_t, const UserOrKernelBuffer&);

        /**
         * @param old_size 
         * @param new_size 
         */
        void inode_size_changed(size_t old_size, size_t new_size);

        /**
         * @return KResult 
         */
        KResult prepare_to_write_data();

        void did_add_child(const InodeIdentifier&);
        void did_remove_child(const InodeIdentifier&);

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