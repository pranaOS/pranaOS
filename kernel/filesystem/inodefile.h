/**
 * @file inodefile.h
 * @author Krisna Pranav
 * @brief inodefile
 * @version 6.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/filesystem/file.h>

namespace Kernel 
{

    class Inode;

    class InodeFile final : public File 
    {
    public:
        /**
         * @param inode 
         * @return NonnullRefPtr<InodeFile> 
         */
        static NonnullRefPtr<InodeFile> create(NonnullRefPtr<Inode>&& inode)
        {
            return adopt(*new InodeFile(move(inode)));
        }

        virtual ~InodeFile() override;

        /**
         * @return const Inode& 
         */
        const Inode& inode() const 
        { 
            return *m_inode; 
        }

        /**
         * @return Inode& 
         */
        Inode& inode() 
        { 
            return *m_inode; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override 
        { 
            return true; 
        }

        virtual bool can_write(const FileDescription&, size_t) const override 
        { 
            return true; 
        }

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override;
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) override;

        /**
         * @param preferred_vaddr 
         * @param offset 
         * @param size 
         * @param prot 
         * @param shared 
         * @return KResultOr<Region*> 
         */
        virtual KResultOr<Region*> mmap(Process&, FileDescription&, VirtualAddress preferred_vaddr, size_t offset, size_t size, int prot, bool shared) override;

        /**
         * @return String 
         */
        virtual String absolute_path(const FileDescription&) const override;

        /**
         * @return KResult 
         */
        virtual KResult truncate(u64) override;

        /**
         * @return KResult 
         */
        virtual KResult chown(FileDescription&, uid_t, gid_t) override;

        /**
         * @return KResult 
         */
        virtual KResult chmod(FileDescription&, mode_t) override;

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "InodeFile"; 
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_seekable() const override 
        { 
            return true; 
        }

        virtual bool is_inode() const override 
        { 
            return true; 
        }

    private:
        explicit InodeFile(NonnullRefPtr<Inode>&&);
        NonnullRefPtr<Inode> m_inode;
    }; // class InodeFile

} // namespace Kernel
