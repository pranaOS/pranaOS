/**
 * @file anonymousvmobject.h
 * @author Krisna Pranav
 * @brief anonymous vm object
 * @version 6.0
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

        /**
         * @return NonnullRefPtr<AnonymousVMObject> 
         */
        static NonnullRefPtr<AnonymousVMObject> create_with_size(size_t);

        /**
         * @return RefPtr<AnonymousVMObject> 
         */
        static RefPtr<AnonymousVMObject> create_for_physical_range(PhysicalAddress, size_t);

        /**
         * @return NonnullRefPtr<AnonymousVMObject> 
         */
        static NonnullRefPtr<AnonymousVMObject> create_with_physical_page(PhysicalPage&);

        /**
         * @return NonnullRefPtr<VMObject> 
         */
        virtual NonnullRefPtr<VMObject> clone() override;

    protected:
        /// @brief Construct a new Anonymous V M Object object
        explicit AnonymousVMObject(size_t);
        explicit AnonymousVMObject(const AnonymousVMObject&);

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "AnonymousVMObject"; 
        }

    private:
        AnonymousVMObject(PhysicalAddress, size_t);

        /**
         * @return AnonymousVMObject& 
         */
        AnonymousVMObject& operator=(const AnonymousVMObject&) = delete;

        /**
         * @return AnonymousVMObject& 
         */
        AnonymousVMObject& operator=(AnonymousVMObject&&) = delete;
        AnonymousVMObject(AnonymousVMObject&&) = delete;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_anonymous() const override 
        { 
            return true; 
        }
    };

} // namespace Kernel

MOD_BEGIN_TYPE_TRAITS(Kernel::AnonymousVMObject)
static bool is_type(const Kernel::VMObject& vmobject) 
{ 
    return vmobject.is_anonymous(); 
}
MOD_END_TYPE_TRAITS()
