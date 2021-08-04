/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>

namespace Kernel::FIS {

enum class Type : u8 {
    RegisterHostToDevice = 0x27,
    RegisterDeviceToHost = 0x34,
    DMAActivate = 0x39,
    DMASetup = 0x41,
    Data = 0x46,
    BISTActivate = 0x58,
    PIOSetup = 0x5F,
    SetDeviceBits = 0xA1
};

enum class DwordCount : size_t {
    RegisterHostToDevice = 5,
    RegisterDeviceToHost = 5,
    DMAActivate = 1,
    DMASetup = 7,
    PIOSetup = 5,
    SetDeviceBits = 2
};

enum HeaderAttributes : u8 {
    C = (1 << 7), 
};

struct [[gnu::packed]] Header {
    u8 fis_type;
    u8 port_muliplier;
};

}

namespace Kernel::FIS::HostToDevice {

struct [[gnu::packed]] Register {
    Header header;
    u8 command;
    u8 features_low;
    u8 lba_low[3];
    u8 device;
    u8 lba_high[3];
    u8 features_high;
    u16 count;
    u8 icc; 
    u8 control;
    u32 reserved;
};

};

namespace Kernel::FIS::DeviceToHost {

struct [[gnu::packed]] Register {
    Header header;
    u8 status;
    u8 error;
    u8 lba_low[3];
    u8 device;
    u8 lba_high[3];
    u8 reserved;
    u16 count;
    u8 reserved2[6];
};

struct [[gnu::packed]] SetDeviceBits {
    Header header;
    u8 status;
    u8 error;
    u32 protocol_specific;
};

struct [[gnu::packed]] DMAActivate {
    Header header;
    u16 reserved;
};

struct [[gnu::packed]] PIOSetup {
    Header header;
    u8 status;
    u8 error;
    u8 lba_low[3];
    u8 device;
    u8 lba_high[3];
    u8 reserved;
    u16 count;
    u8 reserved2;
    u8 e_status;
    u16 transfer_count;
    u16 reserved3;
};

}

namespace Kernel::FIS::BiDirectional {

struct [[gnu::packed]] Data {
    Header header;
    u16 reserved;
    u32 data[];
};

struct [[gnu::packed]] BISTActivate {
};
struct [[gnu::packed]] DMASetup {
    Header header;
    u16 reserved;
    u32 dma_buffer_identifier_low;
    u32 dma_buffer_identifier_high;
    u32 reserved2;
    u32 dma_buffer_offset;
    u32 dma_transfer_count;
    u32 reserved3;
};

}

namespace Kernel::AHCI {

class MaskedBitField {

public:
    explicit MaskedBitField(volatile u32& bitfield_register)
        : m_bitfield(bitfield_register)
        , m_bit_mask(0xffffffff)
    {
    }

    MaskedBitField(volatile u32& bitfield_register, u32 bit_mask)
        : m_bitfield(bitfield_register)
        , m_bit_mask(bit_mask)
    {
    }

    void set_at(u8 index) const
    {
        VERIFY(((1u << index) & m_bit_mask) != 0);
        m_bitfield = m_bitfield | ((1u << index) & m_bit_mask);
    }

    void set_all() const
    {
        m_bitfield = m_bitfield | (0xffffffff & m_bit_mask);
    }

    bool is_set_at(u8 port_index) const
    {
        return m_bitfield & ((1u << port_index) & m_bit_mask);
    }

    bool is_zeroed() const
    {
        return (m_bitfield & m_bit_mask) == 0;
    }

    Vector<u8> to_vector() const
    {
        Vector<u8> indices;
        u32 bitfield = m_bitfield & m_bit_mask;
        for (size_t index = 0; index < 32; index++) {
            if (bitfield & 1) {
                indices.append(index);
            }
            bitfield >>= 1;
        }
        return indices;
    }

    u32 bit_mask() const { return m_bit_mask; };

    bool operator==(const MaskedBitField&) const = delete;
    bool operator<=(const MaskedBitField&) const = delete;
    bool operator>=(const MaskedBitField&) const = delete;
    bool operator<(const MaskedBitField&) const = delete;
    bool operator>(const MaskedBitField&) const = delete;

private:
    volatile u32& m_bitfield;
    const u32 m_bit_mask;
};

enum Limits : u16 {
    MaxPorts = 32,
    MaxCommands = 32,
    MaxMultiplierConnectedPorts = 16,
};

enum CommandHeaderAttributes : u16 {
    C = (1 << 10),  
    P = (1 << 7),   
    W = (1 << 6),   
    A = (1 << 5),   
    R = (1 << 8)    
};

enum HBACapabilities : u32 {
    S64A = (u32)1 << 31,  
    SNCQ = 1 << 30,      
    SSNTF = 1 << 29,     
    SMPS = 1 << 28,      
    SSS = 1 << 27,       
    SALP = 1 << 26,      
    SAL = 1 << 25,       
    SCLO = 1 << 24,      
    SAM = 1 << 18,       
    SPM = 1 << 17,       
    FBSS = 1 << 16,      
    PMD = 1 << 15,       
    SSC = 1 << 14,       
    PSC = 1 << 13,       
    CCCS = 1 << 7,       
    EMS = 1 << 6,        
    SXS = 1 << 5         
};

enum HBACapabilitiesExtended : u32 {
    DESO = 1 << 5, 
    SADM = 1 << 4, 
    SDS = 1 << 3,  
    APST = 1 << 2, 
    NVMP = 1 << 1, 
    BOH = 1 << 0,  
};

struct [[gnu::packed]] HBADefinedCapabilities {
    size_t ports_count { 1 };
    size_t max_command_list_entries_count { 1 };
    u8 interface_speed_generation { 1 };
    bool external_sata_supported : 1 { false };
    bool enclosure_management_supported : 1 { false };
    bool command_completion_coalescing_supported : 1 { false };
    bool partial_state_capable : 1 { false };
    bool slumber_state_capable : 1 { false };
    bool pio_multiple_drq_block : 1 { false };
    bool fis_based_switching_supported : 1 { false };
    bool port_multiplier_supported : 1 { false };
    bool ahci_mode_only : 1 { true };
    bool command_list_override_supported : 1 { false };
    bool activity_led_supported : 1 { false };
    bool aggressive_link_power_management_supported : 1 { false };
    bool staggered_spin_up_supported : 1 { false };
    bool mechanical_presence_switch_supported : 1 { false };
    bool snotification_register_supported : 1 { false };
    bool native_command_queuing_supported : 1 { false };
    bool addressing_64_bit_supported : 1 { false };
    bool bios_os_handoff : 1 { false };
    bool nvmhci_present : 1 { false };
    bool automatic_partial_to_slumber_transitions : 1 { false };
    bool device_sleep_supported : 1 { false };
    bool aggressive_device_sleep_management_supported : 1 { false };
    bool devsleep_entrance_from_slumber_only : 1 { false };
};

enum DeviceSignature : u32 {
    ATA = 0x00000101,
    ATAPI = 0xEB140101,
    EnclosureManagementBridge = 0xC33C0101,
    PortMultiplier = 0x96690101,
    Unconnected = 0xFFFFFFFF
};

enum class DeviceDetectionInitialization {
    NoActionRequested,
    PerformInterfaceInitializationSequence,
    DisableInterface
};

enum PortInterruptFlag : u32 {
    CPD = (u32)1 << 31, 
    TFE = 1 << 30,      
    HBF = 1 << 29,     
    HBD = 1 << 28,     
    IF = 1 << 27,       
    INF = 1 << 26,      
    OF = 1 << 24,       
    IPM = 1 << 23,      
    PRC = 1 << 22,       
    DMP = 1 << 7,        
    PC = 1 << 6,         
    DP = 1 << 5,         
    UF = 1 << 4,         
    SDB = 1 << 3,        
    DS = 1 << 2,         
    PS = 1 << 1,         
    DHR = 1 << 0        
};

enum SErr : u32 {
    DIAG_X = 1 << 26,  
    DIAG_F = 1 << 25,  
    DIAG_T = 1 << 24,  
    DIAG_S = 1 << 23,  
    DIAG_H = 1 << 22,  
    DIAG_C = 1 << 21,  
    DIAG_D = 1 << 20,  
    DIAG_B = 1 << 19,  
    DIAG_W = 1 << 18, 
    DIAG_I = 1 << 17, 
    DIAG_N = 1 << 16, 
    ERR_E = 1 << 11,  
    ERR_P = 1 << 10,  
    ERR_C = 1 << 9,   
    ERR_T = 1 << 8,    
    ERR_M = 1 << 1,    
    ERR_I = 1 << 0,    
};

class PortInterruptStatusBitField {

public:
    explicit PortInterruptStatusBitField(volatile u32& bitfield_register)
        : m_bitfield(bitfield_register)
    {
    }

    u32 raw_value() const { return m_bitfield; }
    bool is_set(PortInterruptFlag flag) const { return m_bitfield & (u32)flag; }
    void clear() { m_bitfield = 0xffffffff; }

    bool operator==(const MaskedBitField&) const = delete;
    bool operator<=(const MaskedBitField&) const = delete;
    bool operator>=(const MaskedBitField&) const = delete;
    bool operator<(const MaskedBitField&) const = delete;
    bool operator>(const MaskedBitField&) const = delete;

private:
    volatile u32& m_bitfield;
};

class PortInterruptEnableBitField {

public:
    explicit PortInterruptEnableBitField(volatile u32& bitfield_register)
        : m_bitfield(bitfield_register)
    {
    }

    u32 raw_value() const { return m_bitfield; }
    bool is_set(PortInterruptFlag flag) { return m_bitfield & (u32)flag; }
    void set_at(PortInterruptFlag flag) { m_bitfield = m_bitfield | static_cast<u32>(flag); }
    void clear() { m_bitfield = 0; }
    bool is_cleared() const { return m_bitfield == 0; }
    void set_all() { m_bitfield = 0xffffffff; }

    bool operator==(const MaskedBitField&) const = delete;
    bool operator<=(const MaskedBitField&) const = delete;
    bool operator>=(const MaskedBitField&) const = delete;
    bool operator<(const MaskedBitField&) const = delete;
    bool operator>(const MaskedBitField&) const = delete;

private:
    volatile u32& m_bitfield;
};

struct [[gnu::packed]] PortRegisters {
    u32 clb;  
    u32 clbu; 
    u32 fb;   
    u32 fbu;  
    u32 is;   
    u32 ie;   
    u32 cmd;  
    u32 reserved;
    u32 tfd;    
    u32 sig;   
    u32 ssts;   
    u32 sctl;   
    u32 serr;   
    u32 sact;   
    u32 ci;     
    u32 sntf;   
    u32 fbs;    
    u32 devslp; 
    u8 reserved2[0x70 - 0x48];
    u8 vs[16]; 
};

struct [[gnu::packed]] GenericHostControl {
    u32 cap; 
    u32 ghc; 
    u32 is;  
    u32 pi;  
    u32 version;
    u32 ccc_ctl;  
    u32 ccc_ports;
    u32 em_loc;    
    u32 em_ctl;    
    u32 cap2;      
    u32 bohc;      
};

struct [[gnu::packed]] HBA {
    GenericHostControl control_regs;
    u8 reserved[52];
    u8 nvmhci[64];
    u8 vendor_specific[96];
    PortRegisters port_regs[32];
};

struct [[gnu::packed]] CommandHeader {
    u16 attributes;
    u16 prdtl; 
    u32 prdbc; 
    u32 ctba;  
    u32 ctbau; 
    u32 reserved[4];
};

struct [[gnu::packed]] PhysicalRegionDescriptor {
    u32 base_low;
    u32 base_high;
    u32 reserved;
    u32 byte_count; 
};

struct [[gnu::packed]] CommandTable {
    u8 command_fis[64];
    u8 atapi_command[32];
    u8 reserved[32];
    PhysicalRegionDescriptor descriptors[];
};
}
