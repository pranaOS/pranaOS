/**
 * @file inodevmobject.h
 * @author Krisna Pranav
 * @brief inodevmobject
 * @version 1.0
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/unixtypes.h>
#include <kernel/vm/vmobject.h>
#include <mods/bitmap.h>
#include <mods/typecast.h>

namespace Kernel
{
    class InodeVMObject : public VMObject 
    {
    public:
        virtual ~InodeVMObject() override;

        /**
         * @return Inode& 
         */
        Inode& inode() 
        {
            return *m_inode;
        }

        /**
         * @return const Inode& 
         */
        const Inode& inode() const
        {
            return *m_inode;
        }   

        void inode_contents_changed(Badge<Inode>, off_t, ssize_t);

        /**
         * @param old_size 
         * @param new_size 
         */
        void inode_size_changes(Badge<Inode>, size_t old_size, size_t new_size);

        size_t amount_dirty() const;
        size_t amount_clear() const;


    protected:
        explicit InodeVMObject(Inode&, size_t);
        explicit InodeVMObject(const InodeVMObject&);

        virtual bool is_inode() const final
        {
            return true;
        }

        int release_all_clean_pages_impl();

        NonnullRefPtr<Inode> m_inode;
        Bitmap m_dirty_pages;

    }; // class InodeVMObject
} // namespace Kernel

MOD_BEGIN_TYPE_TRAITS(Kernel::InodeVMObject)
static bool is_type(const Kernel::VMObject& vmobject)
{
    return vmobject.is_inode();
}
MOD_END_TYPE_TRAITS()