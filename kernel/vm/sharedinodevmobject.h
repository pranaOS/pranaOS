/**
 * @file sharedinodevmobject.h
 * @author Krisna Pranav
 * @brief Shared inode vm object
 * @version 6.0
 * @date 2023-08-09
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

    class SharedInodeVMObject final : public InodeVMObject 
    {
        MOD_MAKE_NONMOVABLE(SharedInodeVMObject);

    public:
        /**
         * @brief Destroy the Shared Inode VM Object object
         * 
         */
        virtual ~SharedInodeVMObject() override;

        /**
         * @return NonnullRefPtr<SharedInodeVMObject> 
         */
        static NonnullRefPtr<SharedInodeVMObject> create_with_inode(Inode&);

        /**
         * @return NonnullRefPtr<VMObject> 
         */
        virtual NonnullRefPtr<VMObject> clone() override;

    private:
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_shared_inode() const override 
        { 
            return true; 
        }

        /**
         * @brief Construct a new Shared Inode V M Object object
         * 
         */
        explicit SharedInodeVMObject(Inode&, size_t);

        /**
         * @brief Construct a new Shared Inode V M Object object
         * 
         */
        explicit SharedInodeVMObject(const SharedInodeVMObject&);

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "SharedInodeVMObject"; 
        }

        /**
         * @return SharedInodeVMObject& 
         */
        SharedInodeVMObject& operator=(const SharedInodeVMObject&) = delete;
    };

}