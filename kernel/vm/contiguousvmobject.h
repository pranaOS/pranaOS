/**
 * @file contiguousvmobject.h
 * @author Krisna Pranav
 * @brief contiguous vm object
 * @version 1.0
 * @date 2023-08-09
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/physical_address.h>
#include <kernel/vm/vmobject.h>

namespace Kernel
{
    class ContiguousVMObject final : public VMObject
    {
    private:
        explicit ContiguousVMObject(size_t);
        explicit ContiguousVMObject(const ContiguousVMObject&);

        virtual const char* class_name() const override
        {
            return "ContiguousVMObject";
        }

        virtual NonnullRefPtr<VMObject> clone() override;

        ContiguousVMObject& operator=(const ContiguousVMObject&) = delete;
        ContiguousVMObject& operator=(ContiguousVMObject&&) = delete;
        ContiguousVMObject(ContiguousVMObject&&) = delete;

        virtual bool is_contiguous() const override
        {   
            return true;
        }
    }; // ContiguousVMObject
} // namespace Kernel
