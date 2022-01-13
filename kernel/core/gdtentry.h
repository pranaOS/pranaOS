#pragma once

#include <ak/types.h>

using namespace pranaOS::ak;

namespace Kernel {
    namespace core {
        struct gdtEntry {
            ak::uint16_t limit_low;           
            ak::uint16_t base_low;            
            ak::uint8_t  base_middle;         
            ak::uint8_t  access;              
            ak::uint8_t  granularity;
            ak::uint8_t  base_high;           
        } __attribute__((packed));

        struct gdtPointer {
            ak::uint16_t limit;               
            ak::uint32_t base;                
        } __attribute__((packed));

        class globalDescriptorTable {        
        public:
            static void SetDescriptor(int number, ak::uint32_t base, ak::uint32_t limit, ak::uint8_t access, ak::uint8_t gran);
            static gdtEntry* GetDescriptor(int number);
            static void Init();
        };
    }
}