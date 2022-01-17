#pragma once

#include "exceptions.h"
#include "port.h"
#include "register.h"
#include <ak/convert.h>
#include <ak/types.h>
#include <ak/memoperator.h>
#include <system/interrupthandler.h>

namespace Kernel {

    #define IDT_ENTRY_SIZE 256
    #define IDT_PRESENT 0x80
    #define IDT_INTERRUPT 0xE
    #define IDT_INTERRUPT_OFFSET 0x20 

    struct idtEntry {
        ak::uint16_t handlerLowBits;
        ak::uint16_t selector;
        ak::uint16_t handlerHighBits;
        ak::uint8_t reserved;
        ak::uint8_t access;
    };

    struct idtPointer {
        ak::uint16_t size;
        ak::uint32_t base;
    } __attribute__((packed));

    class interruptDescriptorTable {
    public:
        static void install();
        static void enableInterrupts();
        static void disableInterrupts();
        static bool areEnabled();
    
    private:
        static void setDescriptor(ak::uint32_t number, void(*handler)(), int accessLevel = 0);
        static ak::uint32_t handleInterrupt(CPUState* state);
    };
}