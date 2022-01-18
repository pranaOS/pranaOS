#pragma once

#include <ak/types.h>

using namespace ak;

namespace Kernel {
    struct gdtEntry {
        ak::uint16_t limitLow;           
        ak::uint16_t baseLow;            
        ak::uint8_t  baseMiddle;         
        ak::uint8_t  access;              
        ak::uint8_t  granularity;
        ak::uint8_t  baseHigh;           
    } __attribute__((packed));

    struct gdtPointer {
        ak::uint16_t limit;               
        ak::uint32_t base;                
    } __attribute__((packed));

    class globalDescriptorTable {        
    public:
        static void setDescriptor(int number, ak::uint32_t base, ak::uint32_t limit, ak::uint8_t access, ak::uint8_t gran);
        static gdtEntry* getDescriptor(int number);
        static void init();
    };
}
