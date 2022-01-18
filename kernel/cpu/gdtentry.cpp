#include <cpu/gdtentry.h>

using namespace Kernel;
using namespace Kernel::ak;
using namespace Kernel::core;

gdtEntry gdtEntries[6];
gdtPointer _gdtPointer;

extern "C" void gdt_flush(uint32_t);

void globalDescriptorTable::setDescriptor(int number, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdtEntries[number].base_low    = (base & 0xFFFF);
    gdtEntries[number].base_middle = (base >> 16) & 0xFF;
    gdtEntries[number].base_high   = (base >> 24) & 0xFF;

    gdtEntries[number].limit_low   = (limit & 0xFFFF);
    gdtEntries[number].granularity = (limit >> 16) & 0x0F;

    gdtEntries[number].granularity |= gran & 0xF0;
    gdtEntries[number].access      = access;
}

gdtEntry* globalDescriptorTable::getDescriptor(int number) {
    return &gdtEntries[number];
}

void globalDescriptorTable::init() {
    _gdtPointer.limit = (sizeof(gdtEntry) * 6) - 1;
    _gdtPointer.base = (uint32_t)&gdtEntries;

    setDescriptor(0, 0, 0, 0, 0);                
    setDescriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); 
    setDescriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF); 
    setDescriptor(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); 
    setDescriptor(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); 

    gdt_flush((uint32_t)&_gdtPointer);
}