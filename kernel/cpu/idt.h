#pragma once

#include <ak/convert.h>
#include <ak/memoperator.h>
#include <ak/types.h>
#include <kernel/console.h>
#include <kernel/interrupthandler.h>
#include "port.h"
#include "register.h"
#include <exception.h>


namespace Kernel {
    #define IDT_ENTRY_SIZE 256
    #define IDT_PRESENT 0x80
    #define IDT_INTERRUPT 0xE
    #define IDT_INTERRUPT_OFFSET 0x20

    struct idtEntry {
        ak::uint16_t handlerLowBits;
        ak::uint16_t selector;
        ak::uint8_t reserved;
        ak::uint8_t access;
        ak::uint16_t handlerHighBits;
    } __attribute__((packed));

    struct idtPointer {
        ak::uint16_t size;
        ak::uint32_t base;
    } __attribute__((packed));

    extern "C" void ignoreInterrupt();

    extern "C" void handleInterruptRequest0x00();
    extern "C" void handleInterruptRequest0x01();
    extern "C" void handleInterruptRequest0x02();
    extern "C" void handleInterruptRequest0x03();
    extern "C" void handleInterruptRequest0x04();
    extern "C" void handleInterruptRequest0x05();
    extern "C" void handleInterruptRequest0x06();
    extern "C" void handleInterruptRequest0x07();
    extern "C" void handleInterruptRequest0x08();
    extern "C" void handleInterruptRequest0x09();

    extern "C" void handleInterruptRequest0x0A();
    extern "C" void handleInterruptRequest0x0B();
    extern "C" void handleInterruptRequest0x0C();
    extern "C" void handleInterruptRequest0x0D();
    extern "C" void handleInterruptRequest0x0E();
    extern "C" void handleInterruptRequest0x0F();
    extern "C" void handleInterruptRequest0x31();
    extern "C" void handleInterruptRequest0xDD();
    extern "C" void handleInterruptRequest0x60();

    extern "C" void handleException0x00();
    extern "C" void handleException0x01();
    extern "C" void handleException0x02();
    extern "C" void handleException0x03();
    extern "C" void handleException0x04();
    extern "C" void handleException0x05();
    extern "C" void handleException0x06();
    extern "C" void handleException0x07();
    extern "C" void handleException0x08();
    extern "C" void handleException0x09();
    extern "C" void handleException0x10();
    extern "C" void handleException0x11();
    extern "C" void handleException0x12();
    extern "C" void handleException0x13();        

    extern "C" void handleException0x0A();
    extern "C" void handleException0x0B();
    extern "C" void handleException0x0C();
    extern "C" void handleException0x0D();
    extern "C" void handleException0x0E();
    extern "C" void handleException0x0F();

    class interruptDescriptorTable {
    public:
        static void install();
        static void enableInterrupts();
        static void disableInterrupts();
        static bool areEnabled();

    private:
        static void setDescriptor(ak::uint32_t number,  void (*handler)(), int accesLevel = 0);
        static ak::uint32_t handleInterrupt(CPUState* state);
    };
}