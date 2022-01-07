#pragma once

#include <types.h>

namespace pranaOSMath {
    // reference: https://www.geeksforgeeks.org/pi-in-c-with-examples/
    #define MATH_PI 3.14159265358979323846

    struct statusRegister {
        uint8_t invalidOperationFlag : 1;
        uint8_t denormalFlag : 1;
        uint8_t devideByZeroFlag : 1;
        uint8_t overflowFlag : 1;
        uint8_t underflowFlag : 1;
        uint8_t precisionFlag : 1;
        uint8_t demormalsAreZeros : 1;
        uint8_t invalidOperationMask : 1;
        uint8_t denormalOperationMask : 1;
        uint8_t devideByZeroMask : 1;
        uint8_t overflowMask : 1;
        uint8_t underflowMask : 1;
        uint8_t precisionMask : 1;
        uint8_t roundingControl : 2;
        uint8_t flushToZero : 1;
        uint16_t reserved;
    } __attribute__((packed));
}
