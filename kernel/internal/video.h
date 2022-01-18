//
// Created by Apple on 18/01/22.
//

#pragma once

#include <ak/types.h>

namespace Kernel {
    typedef struct {
        uint16_t A;
        uint16_t B;
    } __attribute__((packed)) pointer;
}