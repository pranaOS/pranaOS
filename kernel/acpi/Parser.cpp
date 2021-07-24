/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/Format.h>
#include <base/StringView.h>
#include <kernel/acpi/Parser.h>
#include <kernel/arch/pc/BIOS.h>
#include <kernel/arch/x86/InterruptDisabler.h>
#include <kernel/bus/pci/Access.h>
#include <kernel/Debug.h>
#include <kernel/IO.h>
#include <kernel/Sections.h>
#include <kernel/StdLib.h>
#include <kernel/vm/TypedMapping.h>

namespace Kernel::ACPI {

static Parser* s_acpi_parser;

Parser* Parser::the()
{
    return s_acpi_parser;
}

UNMAP_AFTER_INIT NonnullRefPtr<ACPISysFSComponent> ACPISysFSComponent::create(String name, PhysicalAddress paddr, size_t table_size)
{
    return adpot_ref(*new (nothrow) ACPISysFSComponent(name, paddr, table_size));
}

KResultOr<size_t> ACPISysFSComponent::read_bytes(off_t offset, size_t count, UserOrKernelBuffer& buffer, FileDescription*) const
{
    auto blob = try_to_generate_buffer();
    if (!blob)
        return KResult(EFAULT);

    if ((size_t)offset >= blob->size())
        return KSuccess;

    ssize_t nread = min(static_cast<off_t>(blob->size() - offset), static_cast<off_t>(count));
    if (!buffer.write(blob->data() + offset, nread))
        return KResult(EFAULT);
    return nread;
}

OwnPtr<KBuffer> ACPISysFSComponent::try_to_generate_buffer() const
{
    auto acpi_blob = map_typed<u8>((m_paddr), m_length);
    return KBuffer::try_create_with_bytes(Span<u8> { acpi_blob.ptr(), m_length });
}

UNMAP_AFTER_INIT ACPISysFSComponent::ACPISysFSComponent(String name, PhysicalAddress paddr, size_t table_size)
    : SysFSComponent(name)
    , m_paddr(paddr)
    , m_length(table_size)
{
}

}