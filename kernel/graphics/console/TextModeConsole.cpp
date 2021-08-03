/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/graphics/console/TextModeConsole.h>
#include <kernel/graphics/GraphicsManagement.h>
#include <kernel/IO.h>
#include <kernel/Sections.h>

namespace Kernel::Graphics {

UNMAP_AFTER_INIT NonnullRefPtr<TextModeConsole> TextModeConsole::initialize(const VGACompatibleAdapter& adapter)
{
    return adpot_ref(*new TextModeConsole(adapter));
}

UNMAP_AFTER_INIT TextModeConsole::TextModeConsole(const VGACompatibleAdapter& adapter)
    : VGAConsole(adapter, VGAConsole::Mode::TextMode, 80, 25)
    , m_current_vga_window(m_vga_region->vaddr().offset(0x18000).as_ptr())
{
    for (size_t index = 0; index < height(); index++) {
        clear_vga_row(index);
    }
    dbgln("Text mode console initialized!");
}

enum VGAColor : u8 {
    Black = 0,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Brown,
    LightGray,
    DarkGray,
    BrightBlue,
    BrightGreen,
    BrightCyan,
    BrightRed,
    BrightMagenta,
    Yellow,
    White,
};

[[maybe_unused]] static inline VGAColor convert_standard_color_to_vga_color(Console::Color color)
{
    switch (color) {
    case Console::Color::Black:
        return VGAColor::Black;
    case Console::Color::Red:
        return VGAColor::Red;
    case Console::Color::Brown:
        return VGAColor::Brown;
    case Console::Color::Blue:
        return VGAColor::Blue;
    case Console::Color::Magenta:
        return VGAColor::Magenta;
    case Console::Color::Green:
        return VGAColor::Green;
    case Console::Color::Cyan:
        return VGAColor::Cyan;
    case Console::Color::LightGray:
        return VGAColor::LightGray;
    case Console::Color::DarkGray:
        return VGAColor::DarkGray;
    case Console::Color::BrightRed:
        return VGAColor::BrightRed;
    case Console::Color::BrightGreen:
        return VGAColor::BrightGreen;
    case Console::Color::Yellow:
        return VGAColor::Yellow;
    case Console::Color::BrightBlue:
        return VGAColor::BrightBlue;
    case Console::Color::BrightMagenta:
        return VGAColor::BrightMagenta;
    case Console::Color::BrightCyan:
        return VGAColor::BrightCyan;
    case Console::Color::White:
        return VGAColor::White;
    default:
        VERIFY_NOT_REACHED();
    }
}

void TextModeConsole::set_cursor(size_t x, size_t y)
{
    ScopedSpinLock main_lock(GraphicsManagement::the().main_vga_lock());
    ScopedSpinLock lock(m_vga_lock);
    m_cursor_x = x;
    m_cursor_y = y;
    u16 value = m_current_vga_start_address + (y * width() + x);
    IO::out8(0x3d4, 0x0e);
    IO::out8(0x3d5, MSB(value));
    IO::out8(0x3d4, 0x0f);
    IO::out8(0x3d5, LSB(value));
}

void TextModeConsole::clear(size_t x, size_t y, size_t length)
{
    ScopedSpinLock lock(m_vga_lock);
    auto* buf = (u16*)(m_current_vga_window + (x * 2) + (y * width() * 2));
    for (size_t index = 0; index < length; index++) {
        buf[index] = 0x0720;
    }
}

void TextModeConsole::clear_vga_row(u16 row)
{
    clear(row * width(), width(), width());
}

    
}