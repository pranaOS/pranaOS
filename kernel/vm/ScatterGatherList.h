/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

/*
 * Copyright (c) 2021, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <ak/Vector.h>
#include <kernel/devices/BlockDevice.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/vm/AnonymousVMObject.h>
#include <kernel/vm/MemoryManager.h>

namespace Kernel {

class ScatterGatherList : public RefCounted<ScatterGatherList> {
public:
    static RefPtr<ScatterGatherList> try_create(AsyncBlockDeviceRequest&, Span<NonnullRefPtr<PhysicalPage>> allocated_pages, size_t device_block_size);
    const VMObject& vmobject() const { return m_vm_object; }
    VirtualAddress dma_region() const { return m_dma_region->vaddr(); }
    size_t scatters_count() const { return m_vm_object->physical_pages().size(); }

private:
    ScatterGatherList(NonnullRefPtr<AnonymousVMObject>, AsyncBlockDeviceRequest&, size_t device_block_size);
    NonnullRefPtr<AnonymousVMObject> m_vm_object;
    OwnPtr<Region> m_dma_region;
};

}