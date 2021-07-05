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


// e100 port
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

// rctl 
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