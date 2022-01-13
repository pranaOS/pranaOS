//
//  fpu.h
//  pranaOS
//
//  Created by Apple on 13/01/22.
//

#pragma once

#include <ak/types.h>

namespace Kernel {
    struct Fpu {
        ak::uint8_t invalidOperand : 1;
        ak::uint8_t denormalOperand : 1 ;
        ak::uint8_t zeroDevide : 1;
        ak::uint8_t overflow : 1;
    }

    class fpu {
    public:
        static void enable();
    };
}
