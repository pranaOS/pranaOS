/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefCounted.h>
#include <base/Types.h>
#include <base/Vector.h>
#include <kernel/PhysicalAddress.h>


namespace Kernel::ACPI {

namespace FADTFlags {

enum class FeatureFlags : u32 {
    WBINVD = 1 << 0,
    WBINVD_FLUSH = 1 << 1,
    PROC_C1 = 1 << 2,
    P_LVL2_UP = 1 << 3,
    PWR_BUTTON = 1 << 4,
    SLP_BUTTON = 1 << 5,
    FIX_RTC = 1 << 6,
    RTC_s4 = 1 << 7,
    TMR_VAL_EXT = 1 << 8,
    DCK_CAP = 1 << 9,
    RESET_REG_SUPPORTED = 1 << 10,
    SEALED_CASE = 1 << 11,
    HEADLESS = 1 << 12,
    CPU_SW_SLP = 1 << 13,
    PCI_EXP_WAK = 1 << 14,
    USE_PLATFORM_CLOCK = 1 << 15,
    S4_RTC_STS_VALID = 1 << 16,
    REMOTE_POWER_ON_CAPABLE = 1 << 17,
    FORCE_APIC_CLUSTER_MODEL = 1 << 18,
    FORCE_APIC_PHYSICAL_DESTINATION_MODE = 1 << 19,
    HW_REDUCED_ACPI = 1 << 20,
    LOW_POWER_S0_IDLE_CAPABLE = 1 << 21
};

enum class IA_PC_Flags : u8 {
    Legacy_Devices = 1 << 0,
    PS2_8042 = 1 << 1,
    VGA_Not_Present = 1 << 2,
    MSI_Not_Supported = 1 << 3,
    PCIe_ASPM_Controls = 1 << 4,
    CMOS_RTC_Not_Present = 1 << 5
};

struct [[gnu::packed]] HardwareFeatures {
    bool wbinvd : 1;
    bool wbinvd_flush : 1;
    bool processor_c1 : 1;
    bool multiprocessor_c2 : 1;
    bool power_button : 1;
    bool sleep_button : 1;
    bool fix_rtc : 1;
    bool rtc_s4 : 1;
    bool timer_value_extension : 1;
    bool docking_capability : 1;
    bool reset_register_supported : 1;
    bool sealed_case : 1;
    bool headless : 1;
    bool cpu_software_sleep : 1;
    bool pci_express_wake : 1;
    bool use_platform_clock : 1;
    bool s4_rtc_status_valid : 1;
    bool remote_power_on_capable : 1;
    bool force_apic_cluster_model : 1;
    bool force_apic_physical_destination_mode : 1;
    bool hardware_reduced_acpi : 1;
    bool low_power_s0_idle_capable : 1;
};
struct [[gnu::packed]] x86_Specific_Flags {
    bool legacy_devices : 1;
    bool keyboard_8042 : 1;
    bool vga_not_present : 1;
    bool msi_not_supported : 1;
    bool cmos_rtc_not_present : 1;
};
};

namespace GenericAddressStructure {
enum class AddressSpace {
    SystemMemory = 0,
    SystemIO = 1,
    PCIConfigurationSpace = 2,
    EmbeddedController = 3,
    SMBus = 4,
    PCC = 0xA,
    FunctionalFixedHardware = 0x7F
};
enum class AccessSize {
    Undefined = 0,
    Byte = 1,
    Word = 2,
    DWord = 3,
    QWord = 4
};
enum class BitWidth {
    Undefined = 0,
    Byte = 8,
    Word = 16,
    DWord = 32,
    QWord = 64
};
}


}
}