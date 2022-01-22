//
// Created by KrisnaPranav on 22/01/22.
//

#pragma once

#include <ak/types.h>
#include "systemcomponent.h"

namespace Kernel {

    enum DMA0IO {
        DMA0_STATUS_REG = 0x08,
        DMA0_COMMAND_REG = 0x08,
        DMA0_REQUEST_REG = 0x09,
        DMA0_CHANMASK_REG = 0x0a,
        DMA0_MODE_REG = 0x0b,
        DMA0_CLEARBYTE_FLIPFLOP_REG = 0x0c,
        DMA0_TEMP_REG = 0x0d,
        DMA0_MASTER_CLEAR_REG = 0x0d,
        DMA0_CLEAR_MASK_REG = 0x0e,
        DMA0_MASK_REG = 0x0f
    };

    enum DMA1IO {
        DMA1_STATUS_REG = 0xd0,
        DMA1_COMMAND_REG = 0xd0,
        DMA1_REQUEST_REG = 0xd2,
        DMA1_CHANMASK_REG = 0xd4,
        DMA1_MODE_REG = 0xd6,
        DMA1_CLEARBYTE_FLIPFLOP_REG = 0xd8,
        DMA1_INTER_REG = 0xda,
        DMA1_UNMASK_ALL_REG = 0xdc,
        DMA1_MASK_REG = 0xde
    };

    enum DMA0CHANNELIO {
        DMA0_CHAN0_ADDR_REG = 0,
        DMA0_CHAN0_COUNT_REG = 1,
        DMA0_CHAN1_ADDR_REG = 2,
        DMA0_CHAN1_COUNT_REG = 3,
        DMA0_CHAN2_ADDR_REG = 4,
        DMA0_CHAN2_COUNT_REG = 5,
        DMA0_CHAN3_ADDR_REG = 6,
        DMA0_CHAN3_COUNT_REG = 7,
    };

    enum DMA1CHANNELIO {
        DMA1_CHAN4_ADDR_REG = 0xc0,
        DMA1_CHAN4_COUNT_REG = 0xc2,
        DMA1_CHAN5_ADDR_REG = 0xc4,
        DMA1_CHAN5_COUNT_REG = 0xc6,
        DMA1_CHAN6_ADDR_REG = 0xc8,
        DMA1_CHAN6_COUNT_REG = 0xca,
        DMA1_CHAN7_ADDR_REG = 0xcc,
        DMA1_CHAN7_COUNT_REG = 0xce,
    };

    enum DMA0PAGEREG {
        DMA_PAGE_EXTRA0 = 0x80,
        DMA_PAGE_CHAN2_ADDRBYTE2 = 0x81,
        DMA_PAGE_CHAN3_ADDRBYTE2 = 0x82,
        DMA_PAGE_CHAN1_ADDRBYTE2 = 0x83,
        DMA_PAGE_EXTRA1 = 0x84,
        DMA_PAGE_EXTRA2 = 0x85,
        DMA_PAGE_EXTRA3 = 0x86,
        DMA_PAGE_CHAN6_ADDRBYTE2 = 0x87,
        DMA_PAGE_CHAN7_ADDRBYTE2 = 0x88,
        DMA_PAGE_CHAN5_ADDRBYTE2 = 0x89,
        DMA_PAGE_EXTRA4 = 0x8c,
        DMA_PAGE_EXTRA5 = 0x8d,
        DMA_PAGE_EXTRA6 = 0x8e,
        DMA_PAGE_DRAM_REFRESH = 0x8f
    };

    enum DMACMDREGMASK {

        DMA_CMD_MASK_MEMTOMEM = 1,
        DMA_CMD_MASK_CHAN0ADDRHOLD = 2,
        DMA_CMD_MASK_ENABLE = 4,
        DMA_CMD_MASK_TIMING = 8,
        DMA_CMD_MASK_PRIORITY = 0x10,
        DMA_CMD_MASK_WRITESEL = 0x20,
        DMA_CMD_MASK_DREQ = 0x40,
        DMA_CMD_MASK_DACK = 0x80
    };

    enum DMAMODEREGMASK {
        DMA_MODE_MASK_SEL = 3,
        DMA_MODE_MASK_TRA = 0xc,
        DMA_MODE_SELF_TEST = 0,
        DMA_MODE_READ_TRANSFER =4,
        DMA_MODE_WRITE_TRANSFER = 8,
        DMA_MODE_MASK_AUTO = 0x10,
        DMA_MODE_MASK_IDEC = 0x20,
        DMA_MODE_MASK = 0xc0,
        DMA_MODE_TRANSFER_ON_DEMAND= 0,
        DMA_MODE_TRANSFER_SINGLE = 0x40,
        DMA_MODE_TRANSFER_BLOCK = 0x80,
        DMA_MODE_TRANSFER_CASCADE = 0xC0
    };

    /**
     * @breif: dmaController[set channel address, counter]
     */
    class dmaController : public systemComponent {
      public:
        dmaController();

        void setChannelAddress(common::uint8_t channel, common::uint8_t low, common::uint8_t high);
        void setChannelCounter(common::uint8_t channel, common::uint8_t low, common::uint8_t high);
        void setExternalPageRegister(common::uint8_t reg, common::uint8_t val);
        void setChannelMode(common::uint8_t channel, common::uint8_t mode);
    };
}