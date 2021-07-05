/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/RingBuffer.h>
#include <libutils/Vector.h>

#include "pci/PCIDevice.h"
#include "system/memory/MMIO.h"


// knob 
#define SND_KNOB_MASTER 0
#define SND_KNOB_VENDOR 1024

// bars
#define AC97_NAMBAR 0x10   
#define AC97_NABMBAR 0x14  

// bus master io ports
#define AC97_PO_BDBAR 0x10  
#define AC97_PO_CIV 0x14    
#define AC97_PO_LVI 0x15    
#define AC97_PO_SR 0x16    
#define AC97_PO_PICB 0x18   
#define AC97_PO_CR 0x1B     


#define AC97_RINGBUFFER_LEN 0x10000
#define AC97_BDL_LEN 32

// buffer 16kb len
#define AC97_BDL_BUFFER_LEN 0x4000                    
#define AC97_CL_GET_LENGTH(cl) ((cl)&0xFFFE)          
#define AC97_CL_SET_LENGTH(cl, v) ((cl) = (v)&0xFFFE) 
#define AC97_CL_BUP ((uint32_t)1 << 30)               
#define AC97_CL_IOC ((uint32_t)1 << 31)              

// pcm out control register flags
#define AC97_X_CR_RPBM (1 << 0) 
#define AC97_X_CR_RR (1 << 1)   
#define AC97_X_CR_LVBIE (1 << 2)
#define AC97_X_CR_FEIE (1 << 3) 
#define AC97_X_CR_IOCE (1 << 4) 

// status register flags defined
#define AC97_X_SR_DCH (1 << 0)  
#define AC97_X_SR_CELV (1 << 1) 
#define AC97_X_SR_LVBCI (1 << 2)
#define AC97_X_SR_BCIS (1 << 3) 
#define AC97_X_SR_FIFOE (1 << 4)

// mixer io ports
#define AC97_RESET 0x0000
#define AC97_MASTER_VOLUME 0x0002
#define AC97_AUX_OUT_VOLUME 0x0004
#define AC97_MONO_VOLUME 0x0006
#define AC97_PC_BEEP 0x000A
#define AC97_PCM_OUT_VOLUME 0x0018
#define AC97_EXT_AUDIO_ID 0x0028
#define AC97_EXT_AUDIO_STC 0x002A
#define AC97_FRONT_SPLRATE 0x002C
#define AC97_LR_SPLRATE 0x0032
#define AC97_PLAYBACK_SPEED 48000
#define AC97_GLB_CTRL_STAT 0x0060

struct PACKED AC97BufferDescriptor
{
    uintptr_t pointer;
    uint32_t cl;
};