/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefPtr.h>
#include <kernel/acpi/Parser.h>
#include <kernel/interrupts/IRQHandler.h>
#include <kernel/locking/Mutex.h>
#include <kernel/memory/PhysicalPage.h>
#include <kernel/PhysicalAddress.h>

namespace Kernel::ACPI {

class DynamicParser final
    : public IRQHandler
    , public Parser {
    friend class Parser;

public:
    virtual void enable_aml_interpretation() override;
    virtual void enable_aml_interpretation(File& dsdt_file) override;
    virtual void enable_aml_interpretation(u8* physical_dsdt, u32 dsdt_payload_legnth) override;
    virtual void disable_aml_interpretation() override;
    virtual void try_acpi_shutdown() override;
    virtual bool can_shutdown() override { return true; }
    virtual StringView purpose() const override { return "ACPI Parser"; }

protected:
    explicit DynamicParser(PhysicalAddress rsdp);

private:
    void build_namespace();
    
    virtual bool handle_irq(const RegisterState&) override;

    OwnPtr<Memory::Region> m_acpi_namespace;
};

}
