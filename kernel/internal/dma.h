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

    /**
     * @breif: dmaController[set channel address, counter]
     */
    class dmaController : public systemComponent {
      public:
        dmaController();

        void setChannelAddress(ak::uint8_t channel, ak::uint8_t low, ak::uint8_t high);
        void setChannelCounter(ak::uint8_t channel, ak::uint8_t low, ak::uint8_t high);
    };
}