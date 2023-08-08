/**
 * @file anonymousvmobject.h
 * @author Krisna Pranav
 * @brief anonymous vm object
 * @version 1.0
 * @date 2023-08-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/physical_address.h>
#include <kernel/vm/vmobject.h>

namespace Kernel
{

    class AnonymousVMObject : public VMObject 
    {
    public:
        virtual ~AnonymousVMObject() override;

        static NonnullRefPtr<AnonymousVMObject> create_with_size(size_t);

        /**
         * @return RefPtr<AnonymousVMObject> 
         */
        static RefPtr<AnonymousVMObject> create_for_physical_range(PhysicalAddress, size_t);

        /**
         * @return NonnullRefPtr<AnonymousVMObject> 
         */
        static NonnullRefPtr<AnonymousVMObject> create_wit_physical_page(PhysicalAddress);

        /**
         * @return NonnullRefPtr<VMObject> 
         */
        virtual NonnullRefPtr<VMObject> clone() override;

    private:
        AnonymousVMObject(PhysicalAddress, size_t);

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_anonymous() const override 
        {
            return true;
        }
    }

} // namespace Kernel
