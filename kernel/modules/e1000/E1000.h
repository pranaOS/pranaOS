/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Vector.h>
#include "pci/PCIDevice.h"
#include "system/memory/MMIO.h"

#define E1000_REG_CONTROL 0x0000
#define E1000_REG_STATUS 0x0008

#define E1000_REG_EEPROM 0x0014
#define E1000_REG_IMASK 0x00D0
#define E1000_REG_MAC_LOW 0x5400
#define E1000_REG_MAC_HIGHT 0x5404

#define E1000_REG_RX_CONTROL 0x0100
#define E1000_REG_RX_LOW 0x2800
#define E1000_REG_RX_HIGH 0x2804
#define E1000_REG_RX_LENGTH 0x2808
#define E1000_REG_RX_HEAD 0x2810
#define E1000_REG_RX_TAIL 0x2818

#define RCTL_EN (1 << 1)             
#define RCTL_SBP (1 << 2)            
#define RCTL_UPE (1 << 3)            
#define RCTL_MPE (1 << 4)            
#define RCTL_LPE (1 << 5)            
#define RCTL_LBM_NONE (0 << 6)       
#define RCTL_LBM_PHY (3 << 6)        
#define RTCL_RDMTS_HALF (0 << 8)    
#define RTCL_RDMTS_QUARTER (1 << 8)  
#define RTCL_RDMTS_EIGHTH (2 << 8)   
#define RCTL_MO_36 (0 << 12)         
#define RCTL_MO_35 (1 << 12)         
#define RCTL_MO_34 (2 << 12)        
#define RCTL_MO_32 (3 << 12)         
#define RCTL_BAM (1 << 15)          
#define RCTL_VFE (1 << 18)           
#define RCTL_CFIEN (1 << 19)         
#define RCTL_CFI (1 << 20)          
#define RCTL_DPF (1 << 22)         
#define RCTL_PMCF (1 << 23)         
#define RCTL_SECRC (1 << 26)        
#define RCTL_BSIZE_256 (3 << 16)
#define RCTL_BSIZE_512 (2 << 16)
#define RCTL_BSIZE_1024 (1 << 16)
#define RCTL_BSIZE_2048 (0 << 16)
#define RCTL_BSIZE_4096 ((3 << 16) | (1 << 25))
#define RCTL_BSIZE_8192 ((2 << 16) | (1 << 25))
#define RCTL_BSIZE_16384 ((1 << 16) | (1 << 25))

#define E1000_REG_TX_CONTROL 0x0400
#define E1000_REG_TX_LOW 0x3800
#define E1000_REG_TX_HIGH 0x3804
#define E1000_REG_TX_LENGTH 0x3808
#define E1000_REG_TX_HEAD 0x3810
#define E1000_REG_TX_TAIL 0x3818

#define TCTL_EN (1 << 1)      
#define TCTL_PSP (1 << 3)     
#define TCTL_CT_SHIFT 4       
#define TCTL_COLD_SHIFT 12    
#define TCTL_SWXOFF (1 << 22) 
#define TCTL_RTLC (1 << 24)   

#define TSTA_DD (1 << 0) 
#define TSTA_EC (1 << 1) 
#define TSTA_LC (1 << 2) 
#define LSTA_TU (1 << 3) 

#define CMD_EOP (1 << 0) 
#define CMD_IFCS (1 << 1)
#define CMD_IC (1 << 2)  
#define CMD_RS (1 << 3)  
#define CMD_RPS (1 << 4) 
#define CMD_VLE (1 << 6) 
#define CMD_IDE (1 << 7) 

#define E1000_NUM_RX_DESC 32
#define E1000_NUM_TX_DESC 8

#define E1000_CTL_START_LINK 0x40  

struct PACKED E1000RXDescriptor
{
    uint64_t address;
    uint16_t length;
    uint16_t checksum;
    uint8_t status;
    uint8_t errors;
    uint16_t special;
};

struct PACKED E1000TXDescriptor
{
    uint64_t address;
    uint16_t length;
    uint8_t cso;
    uint8_t command;
    uint8_t status;
    uint8_t css;
    uint16_t special;
};

struct E1000 : public PCIDevice
{
private:
    RefPtr<MMIORange> _mmio_range = {};
    uint16_t _pio_base = 0;

    bool _has_eeprom = false;
    MacAddress _mac_address = {};

    int _current_rx_descriptors = 0;
    RefPtr<MMIORange> _rx_descriptors_range{};
    E1000RXDescriptor *_rx_descriptors{};
    Vector<RefPtr<MMIORange>> _rx_buffers;

    int _current_tx_descriptors = 0;
    RefPtr<MMIORange> _tx_descriptors_range{};
    E1000TXDescriptor *_tx_descriptors{};
    Vector<RefPtr<MMIORange>> _tx_buffers;

    void write_register(uint16_t offset, uint32_t value);

    uint32_t read_register(uint16_t offset);

    bool detect_eeprom();

    uint16_t read_eeprom(uint32_t address);

    MacAddress read_mac_address();

    void initialize_rx();

    void initialize_tx();

    void enable_interrupt();

    size_t receive_packet(void *buffer, size_t size);

    size_t send_packet(const void *buffer, size_t size);

public:
    E1000(DeviceAddress address);

    void acknowledge_interrupt() override;

    void handle_interrupt() override;

    bool can_write() override;

    bool can_read() override;

    ResultOr<size_t> read(size64_t offset, void *buffer, size_t size) override;

    ResultOr<size_t> write(size64_t offset, const void *buffer, size_t size) override;

    HjResult call(IOCall request, void *args) override;
};