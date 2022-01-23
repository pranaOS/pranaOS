//
// Created by KrisnaPranav on 23/01/22.
//

#include "idt.h"

using namespace Kernel;

idtEntry idtEntries[IDT_ENTRY_SIZE];
idtPointer idtpointer;


void InterruptDescriptorTable::SetDescriptor(uint32_t number, void (*handler)(), int accesLevel) {
    uint32_t callerBase = (uint32_t)handler;

    idtEntries[number].handlerLowBits = (uint16_t)(callerBase & 0xFFFF);
    idtEntries[number].handlerHighBits = (uint16_t)((callerBase >> 16) & 0xFFFF);
    idtEntries[number].reserved = 0;
    idtEntries[number].access = IDT_PRESENT | ((accesLevel & 3) << 5) | IDT_INTERRUPT;
    idtEntries[number].selector = 0x8;
}