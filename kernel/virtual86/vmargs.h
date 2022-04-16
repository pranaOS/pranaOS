//
//  vmargs.h
//  pranaOS
//
//  Created by Krisna Pranav on 13/01/22.
//

#pragma once

#include <ak/types.h>

namespace pranaOS {
    struct VM86Args {
        uint16_t AX;
        uint16_t BX;
        uint16_t CX;
        uint16_t DX;
        uint16_t DI;
            
    } __attribute__((packed));
}
