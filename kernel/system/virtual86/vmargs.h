//
//  vmargs.h
//  pranaOS
//
//  Created by Krisna Pranav on 13/01/22.
//

#pragma once

#include <ak/types.h>

namespace pranaOS {
    namespace system {
        struct vmArgs {
            ak::uint16_t AX;
            ak::uint16_t BX;
            ak::uint16_t CX;
            ak::uint16_t DX;
            ak::uint16_t DI;
            
        } __attribute__((packed));
    }
}
