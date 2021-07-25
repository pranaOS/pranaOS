/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/PhysicalAddress.h>
#include <kernel/vm/MemoryManager.h>
#include <kernel/vm/VMObject.h>

namespace Kernel {
class ContiguousVMObject final : public VMObject {
public:
    virtual ~ContiguousVMObject() override;

    static RefPtr<ContiguousVMObject> try_create_with_size(size_t);

private:
    explicit ContiguousVMObject(size_t, NonnullRefPtrVector<PhysicalPage>&);
    explicit ContiguousVMObject(ContiguousVMObject const&);

    virtual StringView class_name() const override { return "ContiguousVMObject"sv; }
    virtual RefPtr<VMObject> try_clone() override;

    ContiguousVMObject& operator=(ContiguousVMObject const&) = delete;
    ContiguousVMObject& operator=(ContiguousVMObject&&) = delete;
    ContiguousVMObject(ContiguousVMObject&&) = delete;

    virtual bool is_contiguous() const override { return true; }
};

}