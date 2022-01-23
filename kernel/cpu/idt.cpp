//
// Created by KrisnaPranav on 23/01/22.
//

#include "idt.h"
#include <ak/memoperator.h>

using namespace Kernel;

idtEntry idtEntries[IDT_ENTRY_SIZE];
idtPointer idtpointer;


void interruptDescriptorTable::setDescriptor(uint32_t number, void (*handler)(), int accesLevel) {
    uint32_t callerBase = (uint32_t)handler;

    idtEntries[number].handlerLowBits = (uint16_t)(callerBase & 0xFFFF);
    idtEntries[number].handlerHighBits = (uint16_t)((callerBase >> 16) & 0xFFFF);
    idtEntries[number].reserved = 0;
    idtEntries[number].access = IDT_PRESENT | ((accesLevel & 3) << 5) | IDT_INTERRUPT;
    idtEntries[number].selector = 0x8;
}

void interruptDescriptorTable::install() {
    idtpointer.size = sizeof(idtEntry) * IDT_ENTRY_SIZE - 1;
    idtpointer.base = (uint32_t)&idtEntries[0];

    ak::memOperator::memset((void*)&idtEntries[0], 0, sizeof(IDTEntry) * IDT_ENTRY_SIZE - 1);

    for(int i = 0; i < IDT_ENTRY_SIZE; i++)
        setDescriptor(i, IgnoreInterrupt);

    setDescriptor(0x00, handleException0x00);
    setDescriptor(0x01, handleException0x01);
    setDescriptor(0x02, handleException0x02);
    setDescriptor(0x03, handleException0x03);
    setDescriptor(0x04, handleException0x04);
    setDescriptor(0x05, handleException0x05);
    setDescriptor(0x06, handleException0x06);
    setDescriptor(0x07, handleException0x07);
    setDescriptor(0x08, handleException0x08);
    setDescriptor(0x09, handleException0x09);
    setDescriptor(0x0A, handleException0x0A);
    setDescriptor(0x0B, handleException0x0B);
    setDescriptor(0x0C, handleException0x0C);
    setDescriptor(0x0D, handleException0x0D);
    setDescriptor(0x0E, handleException0x0E);
    setDescriptor(0x0F, handleException0x0F);
    setDescriptor(0x10, handleException0x10);
    setDescriptor(0x11, handleException0x11);
    setDescriptor(0x12, handleException0x12);
    setDescriptor(0x13, handleException0x13);

    setDescriptor(IDT_INTERRUPT_OFFSET + 0x00, handleInterruptRequest0x00);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x01, handleInterruptRequest0x01);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x02, handleInterruptRequest0x02);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x03, handleInterruptRequest0x03);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x04, handleInterruptRequest0x04);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x05, handleInterruptRequest0x05);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x06, handleInterruptRequest0x06);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x07, handleInterruptRequest0x07);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x08, handleInterruptRequest0x08);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x09, handleInterruptRequest0x09);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x0A, handleInterruptRequest0x0A);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x0B, handleInterruptRequest0x0B);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x0C, handleInterruptRequest0x0C);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x0D, handleInterruptRequest0x0D);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x0E, handleInterruptRequest0x0E);
    setDescriptor(IDT_INTERRUPT_OFFSET + 0x0F, handleInterruptRequest0x0F);

    setDescriptor((IDT_INTERRUPT_OFFSET + 0xDD), handleInterruptRequest0xDD);
    setDescriptor(0x80, handleInterruptRequest0x60, 3);

    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    
    outportb(0x21, IDT_INTERRUPT_OFFSET);
    outportb(0xA1, IDT_INTERRUPT_OFFSET+8);

    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);

    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);

    outportb(0x21, 0x00);
    outportb(0xA1, 0x00);

    asm volatile("lidt %0" : : "m" (idtPointer));
}