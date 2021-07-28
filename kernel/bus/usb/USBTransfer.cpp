/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/bus/usb/USBTransfer.h>
#include <kernel/vm/MemoryManager.h>

namespace Kernel::USB {

RefPtr<Transfer> Transfer::try_create(Pipe& pipe, u16 len)
{
    auto vmobject = AnonymousVMObject::try_create_physically_contiguous_with_size(PAGE_SIZE);
    if (!vmobject)
        return nullptr;

    return AK::try_create<Transfer>(pipe, len, *vmobject);
}

Transfer::Transfer(Pipe& pipe, u16 len, AnonymousVMObject& vmobject)
    : m_pipe(pipe)
    , m_transfer_data_size(len)
{
    m_data_buffer = MemoryManager::the().allocate_kernel_region_with_vmobject(vmobject, PAGE_SIZE, "USB Transfer Buffer", Region::Access::Read | Region::Access::Write);
}

Transfer::~Transfer()
{
}

void Transfer::set_setup_packet(const USBRequestData& request)
{
    auto* request_data = reinterpret_cast<USBRequestData*>(buffer().as_ptr());

    request_data->request_type = request.request_type;
    request_data->request = request.request;
    request_data->value = request.value;
    request_data->index = request.index;
    request_data->length = request.length;

    m_request = request;
}

}