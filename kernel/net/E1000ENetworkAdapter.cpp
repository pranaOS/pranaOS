/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/MACAddress.h>
#include <kernel/bus/PCI/IDs.h>
#include <kernel/net/E1000ENetworkAdapter.h>
#include <kernel/Sections.h>

namespace Kernel {

#define REG_EEPROM 0x0014

static bool is_valid_device_id(u16 device_id)
{
    switch (device_id) {
    case 0x10D3: 
        return true;
    case 0x1000: 
    case 0x0438: 
    case 0x043A: 
    case 0x043C: 
    case 0x0440: 
    case 0x1001: 
    case 0x1004: 
    case 0x1008:  
    case 0x1009:  
    case 0x100C:  
    case 0x100D:  
    case 0x100E:  
    case 0x100F:  
    case 0x1010:  
    case 0x1011:  
    case 0x1012:  
    case 0x1013:  
    case 0x1014:  
    case 0x1015:  
    case 0x1016:  
    case 0x1017:  
    case 0x1018:  
    case 0x1019:  
    case 0x101A:  
    case 0x101D:  
    case 0x101E:  
    case 0x1026:  
    case 0x1027:  
    case 0x1028:  
    case 0x1049:  
    case 0x104A:  
    case 0x104B:  
    case 0x104C:  
    case 0x104D:  
    case 0x105E:  
    case 0x105F:  
    case 0x1060:  
    case 0x1075:  
    case 0x1076:  
    case 0x1077:  
    case 0x1078:  
    case 0x1079:  
    case 0x107A:  
    case 0x107B:  
    case 0x107C:  
    case 0x107D:  
    case 0x107E:  
    case 0x107F:  
    case 0x108A:  
    case 0x108B:  
    case 0x108C:  
    case 0x1096:  
    case 0x1098:  
    case 0x1099:  
    case 0x109A:  
    case 0x10A4:  
    case 0x10A5:  
    case 0x10A7:  
    case 0x10A9:  
    case 0x10B5:  
    case 0x10B9:  
    case 0x10BA:  
    case 0x10BB:  
    case 0x10BC: 
    case 0x10BD:  
    case 0x10BF:  
    case 0x10C0:  
    case 0x10C2:  
    case 0x10C3:  
    case 0x10C4:  
    case 0x10C5:  
    case 0x10C9:  
    case 0x10CA:  
    case 0x10CB: 
    case 0x10CC: 
    case 0x10CD: 
    case 0x10CE: 
    case 0x10D5: 
    case 0x10D6:  
    case 0x10D9:  
    case 0x10DA:  
    case 0x10DE:  
    case 0x10DF:  
    case 0x10E5:  
    case 0x10E6:  
    case 0x10E7:  
    case 0x10E8:  
    case 0x10EA:  
    case 0x10EB:  
    case 0x10EF:  
    case 0x10F0:  
    case 0x10F5:  
    case 0x10F6:  
    case 0x1501:  
    case 0x1502:  
    case 0x1503:  
    case 0x150A:  
    case 0x150C:  
    case 0x150D: 
    case 0x150E:  
    case 0x150F:  
    case 0x1510:  
    case 0x1511:  
    case 0x1516:  
    case 0x1518: 
    case 0x1520: 
    case 0x1521: 
    case 0x1522: 
    case 0x1523: 
    case 0x1524: 
    case 0x1525: 
    case 0x1526: 
    case 0x1527: 
    case 0x152D: 
    case 0x152F: 
    case 0x1533: 
    case 0x1534: 
    case 0x1535: 
    case 0x1536: 
    case 0x1537: 
    case 0x1538: 
    case 0x1539: 
    case 0x153A: 
    case 0x153B:
    case 0x1546: 
    case 0x1559: 
    case 0x155A: 
    case 0x156F: 
    case 0x1570: 
    case 0x157B:  
    case 0x157C:  
    case 0x15A0:  
    case 0x15A1:
    case 0x15A2: 
    case 0x15A3: 
    case 0x15B7: 
    case 0x15B8: 
    case 0x15B9: 
    case 0x15BB:  
    case 0x15BC:  
    case 0x15BD:  
    case 0x15BE:  
    case 0x15D6:  
    case 0x15D7:  
    case 0x15D8:  
    case 0x15DF:  
    case 0x15E0:  
    case 0x15E1:  
    case 0x15E2:  
    case 0x15E3:  
    case 0x1F40:  
    case 0x1F41:  
    case 0x1F45:  
    case 0x294C:  
        return false;
    default:
        return false;
    }
}

UNMAP_AFTER_INIT RefPtr<E1000ENetworkAdapter> E1000ENetworkAdapter::try_to_initialize(PCI::Address address)
{
    auto id = PCI::get_id(address);
    if (id.vendor_id != PCI::VendorID::Intel)
        return {};
    if (!is_valid_device_id(id.device_id))
        return {};
    u8 irq = PCI::get_interrupt_line(address);
    auto adapter = adopt_ref_if_nonnull(new (nothrow) E1000ENetworkAdapter(address, irq));
    if (!adapter)
        return {};
    if (adapter->initialize())
        return adapter;
    return {};
}

UNMAP_AFTER_INIT bool E1000ENetworkAdapter::initialize()
{
    dmesgln("E1000e: Found @ {}", pci_address());

    m_io_base = IOAddress(PCI::get_BAR2(pci_address()) & ~1);

    enable_bus_mastering(pci_address());

    size_t mmio_base_size = PCI::get_BAR_space_size(pci_address(), 0);
    m_mmio_region = MM.allocate_kernel_region(PhysicalAddress(page_base_of(PCI::get_BAR0(pci_address()))), page_round_up(mmio_base_size), "E1000e MMIO", Region::Access::Read | Region::Access::Write, Region::Cacheable::No);
    if (!m_mmio_region)
        return false;
    m_mmio_base = m_mmio_region->vaddr();
    m_use_mmio = true;
    m_interrupt_line = PCI::get_interrupt_line(pci_address());
    dmesgln("E1000e: port base: {}", m_io_base);
    dmesgln("E1000e: MMIO base: {}", PhysicalAddress(PCI::get_BAR0(pci_address()) & 0xfffffffc));
    dmesgln("E1000e: MMIO base size: {} bytes", mmio_base_size);
    dmesgln("E1000e: Interrupt line: {}", m_interrupt_line);
    detect_eeprom();
    dmesgln("E1000e: Has EEPROM? {}", m_has_eeprom);
    read_mac_address();
    const auto& mac = mac_address();
    dmesgln("E1000e: MAC address: {}", mac.to_string());

    initialize_rx_descriptors();
    initialize_tx_descriptors();

    setup_link();
    setup_interrupts();
    return true;
}

UNMAP_AFTER_INIT E1000ENetworkAdapter::E1000ENetworkAdapter(PCI::Address address, u8 irq)
    : E1000NetworkAdapter(address, irq)
{
}

UNMAP_AFTER_INIT E1000ENetworkAdapter::~E1000ENetworkAdapter()
{
}

UNMAP_AFTER_INIT void E1000ENetworkAdapter::detect_eeprom()
{
    m_has_eeprom = true;
}

UNMAP_AFTER_INIT u32 E1000ENetworkAdapter::read_eeprom(u8 address)
{
    VERIFY(m_has_eeprom);
    u16 data = 0;
    u32 tmp = 0;
    if (m_has_eeprom) {
        out32(REG_EEPROM, ((u32)address << 2) | 1);
        while (!((tmp = in32(REG_EEPROM)) & (1 << 1)))
            ;
    } else {
        out32(REG_EEPROM, ((u32)address << 2) | 1);
        while (!((tmp = in32(REG_EEPROM)) & (1 << 1)))
            ;
    }
    data = (tmp >> 16) & 0xffff;
    return data;
}

}