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

    /**
     * @breif: dmaController[]
     */
    class dmaController : public systemComponent {
      public:
    };
}