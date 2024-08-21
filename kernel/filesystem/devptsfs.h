/**
 * @file devptsfs.h
 * @author Krisna Pranav
 * @brief dev pts fs.
 * @version 6.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/filesystem/filesystem.h>
#include <kernel/filesystem/inode.h>
#include <mods/types.h>

namespace Kernel 
{

    class SlavePTY;
    class DevPtsFSInode;

    class DevPtsFS final : public FS 
    {
        friend class DevPtsFSInode;

    public:
        /// @brief Destroy the DevPtsFS object
        virtual ~DevPtsFS() override;

        /**
         * @return NonnullRefPtr<DevPtsFS> 
         */
        static NonnullRefPtr<DevPtsFS> create();

        /**
         * @return true 
         * @return false 
         */
        virtual bool initialize() override;

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "DevPtsFS"; 
        }

        /**
         * @return NonnullRefPtr<Inode> 
         */
        virtual NonnullRefPtr<Inode> root_inode() const override;

        /// @brief register & unregister
        /// @param SlavePTY
        static void register_slave_pty(SlavePTY&);
        static void unregister_slave_pty(SlavePTY&);

    private:
        /// @brief Construct a new DevPtsFS object
        DevPtsFS();

        /**
         * @return RefPtr<Inode> 
         */
        RefPtr<Inode> get_inode(InodeIdentifier) const;

        RefPtr<DevPtsFSInode> m_root_inode;
    }; // class DevPtsFS

    class DevPtsFSInode final : public Inode 
    {
        friend class DevPtsFS;

    public:
        /// @brief Destroy the DevPtsFS Inode object
        virtual ~DevPtsFSInode() override;

    private:
        /**
         * @param index 
         */
        DevPtsFSInode(DevPtsFS&, unsigned index, SlavePTY*);

        /**
         * @param buffer 
         * @return ssize_t 
         */
        virtual ssize_t read_bytes(off_t, ssize_t, UserOrKernelBuffer& buffer, FileDescription*) const override;

        /**
         * @return InodeMetadata 
         */
        virtual InodeMetadata metadata() const override;

        /**
         * @return KResult 
         */
        virtual KResult traverse_as_directory(Function<bool(const FS::DirectoryEntryView&)>) const override;

        /**
         * @param name 
         * @return RefPtr<Inode> 
         */
        virtual RefPtr<Inode> lookup(StringView name) override;

        /// @brief flush_metadata
        virtual void flush_metadata() override;

        /**
         * @param buffer 
         * @return ssize_t 
         */
        virtual ssize_t write_bytes(off_t, ssize_t, const UserOrKernelBuffer& buffer, FileDescription*) override;

        /**
         * @param name 
         * @return KResultOr<NonnullRefPtr<Inode>> 
         */
        virtual KResultOr<NonnullRefPtr<Inode>> create_child(const String& name, mode_t, dev_t, uid_t, gid_t) override;

        /**
         * @param name 
         * @return KResult 
         */
        virtual KResult add_child(Inode&, const StringView& name, mode_t) override;

        /**
         * @param name 
         * @return KResult 
         */
        virtual KResult remove_child(const StringView& name) override;

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> directory_entry_count() const override;

        /**
         * @return KResult 
         */
        virtual KResult chmod(mode_t) override;

        /**
         * @return KResult 
         */
        virtual KResult chown(uid_t, gid_t) override;

        WeakPtr<SlavePTY> m_pty;

        InodeMetadata m_metadata;

    }; // class DevPtsFSInode

} // namespace Kernel 
