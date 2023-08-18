/**
 * @file virtualfilesystem.h
 * @author Krisna Pranav
 * @brief virtual file system
 * @version 6.0
 * @date 2023-08-11
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/kresult.h>
#include <mods/badge.h>
#include <mods/function.h>
#include <mods/hashmap.h>
#include <mods/nonnullownptrvector.h>
#include <mods/ownptr.h>
#include <mods/refptr.h>
#include <mods/string.h>
#include <kernel/filesystem/filesystem.h>
#include <kernel/filesystem/inodeidentifier.h>
#include <kernel/filesystem/inodemetadata.h>


namespace Kernel 
{

    class Custody;
    class Device;
    class FileDescription;
    struct UnveiledPath;

    struct UidAndGid {
        uid_t uid;
        gid_t gid;
    };

    class VFS 
    {
        MOD_MAKE_ETERNAL
        
    public:
        class Mount 
        {
        public: 
            /**
             * @param host_custody 
             * @param flags 
             */
            Mount(FS&, Custody* host_custody, int flags);

            /**
             * @param source 
             * @param host_custody 
             * @param flags 
             */
            Mount(Inode& source, Custody& host_custody, int flags);

            /**
             * @return const Inode* 
             */
            const Inode* host() const;

            /**
             * @return Inode* 
             */
            Inode* host();

            /**
             * @return const Inode& 
             */
            const Inode& guest() const 
            { 
                return *m_guest; 
            }

            /**
             * @return Inode& 
             */
            Inode& guest() 
            { 
                return *m_guest; 
            }

            /**
             * @return const FS& 
             */
            const FS& guest_fs() const 
            { 
                return *m_guest_fs; 
            }

            /**
             * @return String 
             */
            String absolute_path() const;

            /**
             * @return int 
             */
            int flags() const 
            { 
                return m_flags; 
            }

            /**
             * @param flags 
             */
            void set_flags(int flags) 
            { 
                m_flags = flags; 
            }

        private:
            NonnullRefPtr<Inode> m_guest;
            NonnullRefPtr<FS> m_guest_fs;

            RefPtr<Custody> m_host_custody;

            int m_flags;
        }; // class Mount

        static void initialize();
        static VFS& the();

        VFS();
        ~VFS();

        /**
         * @return true 
         * @return false 
         */
        bool mount_root(FS&);
        
        /**
         * @param mount_point 
         * @param flags 
         * @return KResult 
         */
        KResult mount(FS&, Custody& mount_point, int flags);

        /**
         * @param source 
         * @param mount_point 
         * @param flags 
         * @return KResult 
         */
        KResult bind_mount(Custody& source, Custody& mount_point, int flags);

        /**
         * @param mount_point 
         * @param new_flags 
         * @return KResult 
         */
        KResult remount(Custody& mount_point, int new_flags);

        /**
         * @param guest_inode 
         * @return KResult 
         */
        KResult unmount(Inode& guest_inode);

        /**
         * @param path 
         * @param options 
         * @param mode 
         * @param base 
         * @return KResultOr<NonnullRefPtr<FileDescription>> 
         */
        KResultOr<NonnullRefPtr<FileDescription>> open(StringView path, int options, mode_t mode, Custody& base, Optional<UidAndGid> = {});

        /**
         * @param path 
         * @param options 
         * @param mode 
         * @param parent_custody 
         * @return KResultOr<NonnullRefPtr<FileDescription>> 
         */
        KResultOr<NonnullRefPtr<FileDescription>> create(StringView path, int options, mode_t mode, Custody& parent_custody, Optional<UidAndGid> = {});

        /**
         * @param path 
         * @param mode 
         * @param base 
         * @return KResult 
         */
        KResult mkdir(StringView path, mode_t mode, Custody& base);

        /**
         * @param old_path 
         * @param new_path 
         * @param base 
         * @return KResult 
         */
        KResult link(StringView old_path, StringView new_path, Custody& base);

        /**
         * @param path 
         * @param base 
         * @return KResult 
         */
        KResult unlink(StringView path, Custody& base);

        /**
         * @param target 
         * @param linkpath 
         * @param base 
         * @return KResult 
         */
        KResult symlink(StringView target, StringView linkpath, Custody& base);

        /**
         * @param path 
         * @param base 
         * @return KResult 
         */
        KResult rmdir(StringView path, Custody& base);

        /**
         * @param path 
         * @param base 
         * @return KResult 
         */
        KResult chmod(StringView path, mode_t, Custody& base);
        
        /**
         * @return KResult 
         */
        KResult chmod(Custody&, mode_t);

        /**
         * @param path 
         * @param base 
         * @return KResult 
         */
        KResult chown(StringView path, uid_t, gid_t, Custody& base);

        /**
         * @return KResult 
         */
        KResult chown(Custody&, uid_t, gid_t);

        /**
         * @param path 
         * @param mode 
         * @param base 
         * @return KResult 
         */
        KResult access(StringView path, int mode, Custody& base);

        /**
         * @param path 
         * @param base 
         * @param options 
         * @return KResultOr<InodeMetadata> 
         */
        KResultOr<InodeMetadata> lookup_metadata(StringView path, Custody& base, int options = 0);

        /**
         * @param path 
         * @param base 
         * @param atime 
         * @param mtime 
         * @return KResult 
         */
        KResult utime(StringView path, Custody& base, time_t atime, time_t mtime);

        /**
         * @param oldpath 
         * @param newpath 
         * @param base 
         * @return KResult 
         */
        KResult rename(StringView oldpath, StringView newpath, Custody& base);

        /**
         * @param path 
         * @param base 
         * @return KResult 
         */
        KResult mknod(StringView path, mode_t, dev_t, Custody& base);

        /**
         * @param path 
         * @param base 
         * @return KResultOr<NonnullRefPtr<Custody>> 
         */
        KResultOr<NonnullRefPtr<Custody>> open_directory(StringView path, Custody& base);

        /**
         * @return size_t 
         */
        size_t mount_count() const 
        { 
            return m_mounts.size(); 
        }

        /// @brief: for_each_mount
        void for_each_mount(Function<void(const Mount&)>) const;

        /**
         * @return InodeIdentifier 
         */
        InodeIdentifier root_inode_id() const;

        void sync();

        /**
         * @return Custody& 
         */
        Custody& root_custody();

        /**
         * @param path 
         * @param base 
         * @param out_parent 
         * @param options 
         * @param symlink_recursion_level 
         * @return KResultOr<NonnullRefPtr<Custody>> 
         */
        KResultOr<NonnullRefPtr<Custody>> resolve_path(StringView path, Custody& base, RefPtr<Custody>* out_parent = nullptr, int options = 0, int symlink_recursion_level = 0);

        /**
         * @param path 
         * @param base 
         * @param out_parent 
         * @param options 
         * @param symlink_recursion_level 
         * @return KResultOr<NonnullRefPtr<Custody>> 
         */
        KResultOr<NonnullRefPtr<Custody>> resolve_path_without_veil(StringView path, Custody& base, RefPtr<Custody>* out_parent = nullptr, int options = 0, int symlink_recursion_level = 0);

    private:
        friend class FileDescription;

        /**
         * @param path 
         * @return const UnveiledPath* 
         */
        const UnveiledPath* find_matching_unveiled_path(StringView path);

        /**
         * @param path 
         * @param options 
         * @return KResult 
         */
        KResult validate_path_against_process_veil(StringView path, int options);

        /**
         * @return true 
         * @return false 
         */
        bool is_vfs_root(InodeIdentifier) const;

        /**
         * @return KResult 
         */
        KResult traverse_directory_inode(Inode&, Function<bool(const FS::DirectoryEntryView&)>);

        /**
         * @return Mount* 
         */
        Mount* find_mount_for_host(Inode&);
        Mount* find_mount_for_host(InodeIdentifier);
        Mount* find_mount_for_guest(Inode&);
        Mount* find_mount_for_guest(InodeIdentifier);

        Lock m_lock { "VFSLock" };

        RefPtr<Inode> m_root_inode;

        Vector<Mount, 16> m_mounts;

        RefPtr<Custody> m_root_custody;

    }; // class VFS

} // namespace Kernel
