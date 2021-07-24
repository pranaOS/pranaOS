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

UNMAP_AFTER_INIT void ACPISysFSDirectory::initialize()
{
    auto acpi_directory = adopt_ref(*new (nothrow) ACPISysFSDirectory());
    SysFSComponentRegistry::the().register_new_component(acpi_directory);
}

UNMAP_AFTER_INIT ACPISysFSDirectory::ACPISysFSDirectory()
    : SysFSDirectory("acpi", SysFSComponentRegistry::the().root_directory())
{
    NonnullRefPtrVector<SysFSComponent> components;
    size_t ssdt_count = 0;
    ACPI::Parser::the()->enumerate_static_tables([&](const StringView& signature, PhysicalAddress p_table, size_t length) {
        if (signature == "SSDT") {
            components.append(ACPISysFSComponent::create(String::formatted("{:4s}{}", signature.characters_without_null_termination(), ssdt_count), p_table, length));
            ssdt_count++;
            return;
        }
        components.append(ACPISysFSComponent::create(signature, p_table, length));
    });
    m_components = components;

    auto rsdp = map_typed<Structures::RSDPDescriptor20>(ACPI::Parser::the()->rsdp());
    m_components.append(ACPISysFSComponent::create("RSDP", ACPI::Parser::the()->rsdp(), rsdp->base.revision == 0 ? sizeof(Structures::RSDPDescriptor) : rsdp->length));

    auto main_system_description_table = map_typed<Structures::SDTHeader>(ACPI::Parser::the()->main_system_description_table());
    if (ACPI::Parser::the()->is_xsdt_supported()) {
        m_components.append(ACPISysFSComponent::create("XSDT", ACPI::Parser::the()->main_system_description_table(), main_system_description_table->length));
    } else {
        m_components.append(ACPISysFSComponent::create("RSDT", ACPI::Parser::the()->main_system_description_table(), main_system_description_table->length));
    }
}

void Parser::enumerate_static_tables(Function<void(const StringView&, PhysicalAddress, size_t)> callback)
{
    for (auto& p_table : m_std_pointers) {
        auto table = map_typed<Structures::SDTHeader>(p_table);
        callback({  table->sig, 4 }, p_table, table->length);

    }
}

void Parser::set_the(Parser& parser)
{
    VERIFY(!s_acpi_parser);
    s_acpi_parser = &parser;
}


static bool match_table_signature(PhysicalAddress table_header, cosnt StringView& signature)
static PhysicalAddress search_table_in_xsdt(PhysicalAddress xsdt, const StringView& signature);
static PhysicalAddress search_table_in_rsdt(PhysicalAddress rsdt, const StringView& signature);
static bool validate_table(const Structures::SDTHeader&, size_t length);

}