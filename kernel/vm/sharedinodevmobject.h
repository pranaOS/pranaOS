/**
 * @file sharedinodevmobject.h
 * @author Krisna Pranav
 * @brief Shared inode vm object
 * @version 1.0
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

        explicit SharedInodeVMObject(Inode&, size_t);   
        explicit SharedInodeVMObject(const SharedInodeVMObject&);

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override
        {
            return "SharedInodeVMObject";
        }

        
    }; // SharedInodeVMObject
} // namespace Kernel   