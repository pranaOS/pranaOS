/**
 * @file privateinodevmobject.h
 * @author Krisna Pranav
 * @brief private inode vm object
 * @version 1.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/bitmap.h>
#include <kernel/unixtypes.h>
#include <kernel/vm/inodevmobject.h>

namespace Kernel
{
    class PrivateInodeVMObject final : public InodeVMObject 
    {
        MOD_MAKE_NONMOVABLE(PrivateInodeVMObject);

    public: 
        /**
         * @return NonnullRefPtr<VMObject> 
         */
        virtual NonnullRefPtr<VMObject> clone() override;

    private:
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_private_inode() const override
        {
            return true;
        }

        explicit PrivateInodeVMObject(Inode&, size_t);
        explicit PrivateInodeVMObject(const PrivateInodeVMObject&);

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override
        {
            return "PrivateInodeVMObject";
        }

        /**
         * @return PrivateInodeVMObject& 
         */
        PrivateInodeVMObject& operator=(const PrivateInodeVMObject&) = delete;
    }; // class PrivateInodeVMObject
} // namespace Kernel
