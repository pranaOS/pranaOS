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

    class math {
    public:
        static void enableFPU();

        static long abs(long v);
        static double fAbs(double x);
        static long sign(long v);
        static double sin(double x);
        static double cos(double x);

        static long max(long a, long b);
        static long min(long a, long b);
        static long constrain(long x, long a, long b);
        static long map(long x, long in_min, long in_max, long out_min, long out_max);
        
    }
}
