/**
 * @file contiguousvmobject.h
 * @author Krisna Pranav
 * @brief contiguous vm object
 * @version 6.0
 * @date 2023-08-09
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/physical_address.h>
#include <kernel/vm/vmobject.h>

namespace Kernel 
{

    class ContiguousVMObject final : public VMObject 
    {
    public:
        virtual ~ContiguousVMObject() override;

        /**
         * @return NonnullRefPtr<ContiguousVMObject> 
         */
        static NonnullRefPtr<ContiguousVMObject> create_with_size(size_t);

    private:
        
        explicit ContiguousVMObject(size_t);
        explicit ContiguousVMObject(const ContiguousVMObject&);

        /**
         * @return const char* 
         */
        virtual const char* class_name() const override 
        { 
            return "ContiguousVMObject"; 
        }

        /**
         * @return NonnullRefPtr<VMObject> 
         */
        virtual NonnullRefPtr<VMObject> clone() override;

        /**
         * @return ContiguousVMObject& 
         */
        ContiguousVMObject& operator=(const ContiguousVMObject&) = delete;
        ContiguousVMObject& operator=(ContiguousVMObject&&) = delete;
        ContiguousVMObject(ContiguousVMObject&&) = delete;

        /**
         * @return true 
         * @return false 
         */
        virtual bool is_contiguous() const override 
        { 
            return true; 
        }
    }; // class ContiguousVMObject

} // namespace Kernel

MOD_BEGIN_TYPE_TRAITS(Kernel::ContiguousVMObject)
static bool is_type(const Kernel::VMObject& vmobject) { return vmobject.is_contiguous(); }
MOD_END_TYPE_TRAITS()