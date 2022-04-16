//
//  fpu.h
//  pranaOS
//
//  Created by Krisna Pranav on 13/01/22.
//

#pragma once

#include <ak/types.h>

namespace Kernel {
    
    struct FPU {
        uint8_t invalidOperand : 1;
        uint8_t denormalOperand : 1;
        uint8_t zeroDevide : 1;
        uint8_t overflow : 1;
        uint8_t underflow : 1;
        uint8_t precision : 1;
        uint8_t reserved1 : 1;
        uint8_t reserved2 : 1;
        uint8_t precisionControl : 2;
        uint8_t roundingControl : 2;
        uint8_t infinityControl : 1;
        uint8_t reserved3 : 3;
    } __attribute__((packed));

    class Fpu {
    public:
        static void enable();
    };

}
