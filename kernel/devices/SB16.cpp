/*
 * Copyright (c) 2021, AakeshDarsh
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


// includes
#include <base/Memory.h>
#include <base/Singleton.h>
#include <base/StringView.h>
#include <kernel/arch/x86/InterruptDisabler.h>
#include <kernel/Debug.h>
#include <kernel/devices/SB16.h>
#include <kernel/IO.h>
#include <kernel/Sections.h>
#include <kernel/Thread.h>
#include <kernel/vm/AnonymousVMObject.h>
#include <kernel/vm/MemoryManager.h>

namespace Kernel {
#define SB16_DEFAULT_IRQ 5

enum class SampleFormat : u8 {
    Signed = 0x10,
    Stereo = 0x20,
};

const u16 DSP_READ = 0x22A;
const u16 DSP_WRITE = 0x22C;
const u16 DSP_STATUS = 0x22E;
const u16 DSP_R_ACK = 0x22F;

void SB16::dsp_write(u8 value)
{
    while (IO::in8(DSP_WRITE) & 0x80) {
        ;
    }
    IO::out8(DSP_WRITE, value);
}

UNMAP_AFTER_INIT SB16::SB16()
    : IRQHandler(SB16_DEFAULT_IRQ)
    , CharacterDevice(42, 42) // ### ?
{
    initialize();
}

UNMAP_AFTER_INIT SB16::~SB16()
{
}

UNMAP_AFTER_INIT void SB16::detect()
{
    IO::out8(0x226, 1);
    IO::delay(32);
    IO::out8(0x226, 0);

    auto data = dsp_read();
    if (data != 0xaa) {
        return;
    }
    SB16::create();
}

UNMAP_AFTER_INIT void SB16::create()
{
    s_the.ensure_instance();
}

SB16& SB16::the()
{
    return *s_the;
}

UNMAP_AFTER_INIT void SB16::initialize()
{
    disable_irq();

    IO::out8(0x226, 1);
    IO::delay(32);
    IO::out8(0x226, 0);

    auto data = dsp_read();
    if (data != 0xaa) {
        dbgln("SB16: SoundBlaster not ready");
        return;
    }

    dsp_write(0xe1);
    m_major_version = dsp_read();
    auto vmin = dsp_read();

    dmesgln("SB16: Found version {}.{}", m_major_version, vmin);
    set_irq_register(SB16_DEFAULT_IRQ);
    dmesgln("SB16: IRQ {}", get_irq_line());
}

void SB16::set_irq_register(u8 irq_number)
{
    u8 bitmask;
    switch (irq_number) {
    case 2:
        bitmask = 0;
        break;
    case 5:
        bitmask = 0b10;
        break;
    case 7:
        bitmask = 0b100;
        break;
    case 10:
        bitmask = 0b1000;
        break;
    default:
        VERIFY_NOT_REACHED();
    }
    IO::out8(0x224, 0x80);
    IO::out8(0x225, bitmask);
}

u8 SB16::get_irq_line()
{
    IO::out8(0x224, 0x80);
    u8 bitmask = IO::in8(0x225);
    switch (bitmask) {
    case 0:
        return 2;
    case 0b10:
        return 5;
    case 0b100:
        return 7;
    case 0b1000:
        return 10;
    }
    return bitmask;
}
void SB16::set_irq_line(u8 irq_number)
{
    InterruptDisabler disabler;
    if (irq_number == get_irq_line())
        return;
    set_irq_register(irq_number);
    change_irq_number(irq_number);
}

bool SB16::can_read(const FileDescription&, size_t) const
{
    return false;
}

KResultOr<size_t> SB16::read(FileDescription&, u64, UserOrKernelBuffer&, size_t)
{
    return 0;
}

void SB16::dma_start(uint32_t length)
{
    const auto addr = m_dma_region->physical_page(0)->paddr().get();
    const u8 channel = 5;
    const u8 mode = 0x48;

    IO::out8(0xd4, 4 + (channel % 4));

    IO::out8(0xd8, 0);

    IO::out8(0xd6, (channel % 4) | mode);

    u16 offset = (addr / 2) % 65536;
    IO::out8(0xc4, (u8)offset);
    IO::out8(0xc4, (u8)(offset >> 8));

    IO::out8(0xc6, (u8)(length - 1));
    IO::out8(0xc6, (u8)((length - 1) >> 8));

    IO::out8(0x8b, addr >> 16);
    auto page_number = addr >> 16;
    VERIFY(page_number <= NumericLimits<u8>::max());
    IO::out8(0x8b, page_number);

    IO::out8(0xd4, (channel % 4));
}
