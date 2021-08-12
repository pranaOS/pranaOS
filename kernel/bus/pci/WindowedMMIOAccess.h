/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/HashMap.h>
#include <base/NonnullOwnPtrVector.h>
#include <base/OwnPtr.h>
#include <base/Types.h>
#include <kernel/acpi/Definitions.h>
#include <kernel/bus/pci/Access.h>
#include <kernel/bus/pci/MMIOAccess.h>
#include <kernel/memory/AnonymousVMObject.h>
#include <kernel/memory/PhysicalRegion.h>
#include <kernel/memory/Region.h>
#include <kernel/memory/VMObject.h>

namespace Kernel {
namespace PCI {

class DeviceConfigurationSpaceMapping {
public:
    DeviceConfigurationSpaceMapping(Address, const MMIOAccess::MMIOSegment&);
    VirtualAddress vaddr() const { return m_mapped_region->vaddr(); };
    PhysicalAddress paddr() const { return m_mapped_region->physical_page(0)->paddr(); }
    const Address& address() const { return m_device_address; };

private:
    Address m_device_address;
    NonnullOwnPtr<Memory::Region> m_mapped_region;
};

class WindowedMMIOAccess final : public MMIOAccess {
public:
    static void initialize(PhysicalAddress mcfg);

private:
    explicit WindowedMMIOAccess(PhysicalAddress mcfg);
    virtual void write8_field(Address address, u32, u8) override;
    virtual void write16_field(Address address, u32, u16) override;
    virtual void write32_field(Address address, u32, u32) override;
    virtual u8 read8_field(Address address, u32) override;
    virtual u16 read16_field(Address address, u32) override;
    virtual u32 read32_field(Address address, u32) override;

    Optional<VirtualAddress> get_device_configuration_space(Address address);
    NonnullOwnPtrVector<DeviceConfigurationSpaceMapping> m_mapped_device_regions;
};

}
}
