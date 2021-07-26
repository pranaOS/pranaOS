/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once 

// includes
#include <base/Vector.h>
#include <kernel/devices/BlockDevice.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/vm/AnonymousVMObject.h>
#include <kernel/vm/MemoryManager.h>


namespace Kernel {

class ScatterGatherList : public RefCounted<ScatterGatherList> {
public:
    static RefPtr<ScatterGatherList> try_create(AsyncBlockDeviceRequest&, Span<NonnullRefPtr<PhysicalPage>> allocated_pages, size_t device_block_size);
    const VMObject& vmobject() const { return m_vm_object; }
    size_t scatters_count() const { return m_vm_object->phyiscal_pages().size(); }

private:
    NonnullRefPtr<AnonymousVMObject> m_vm_object
    OwnPtr<Region> m_dma_region;

};

}