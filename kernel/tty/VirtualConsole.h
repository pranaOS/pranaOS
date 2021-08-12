/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Noncopyable.h>
#include <base/NonnullOwnPtrVector.h>
#include <base/String.h>
#include <base/Vector.h>
#include <kernel/api/KeyCode.h>
#include <kernel/ConsoleDevice.h>
#include <kernel/devices/hid/HIDManagement.h>
#include <kernel/graphics/console/Console.h>
#include <kernel/tty/TTY.h>
#include <libvt/Attribute.h>
#include <libvt/Color.h>
#include <libvt/Position.h>
#include <libvt/Terminal.h>

namespace Kernel {

class ConsoleManagement;
class VirtualConsole;

class ConsoleImpl final : public VT::Terminal {
public:
    explicit ConsoleImpl(VirtualConsole&);

    virtual void set_size(u16 columns, u16 rows) override;

private:
    virtual void invalidate_cursor() override;
    virtual void clear() override;
    virtual void clear_history() override;

    virtual void scroll_up(u16 region_top, u16 region_bottom, size_t count) override;
    virtual void scroll_down(u16 region_top, u16 region_bottom, size_t count) override;
    virtual void scroll_left(u16 row, u16 column, size_t count) override;
    virtual void scroll_right(u16 row, u16 column, size_t count) override;
    virtual void put_character_at(unsigned row, unsigned column, u32 ch) override;
    virtual void clear_in_line(u16 row, u16 first_column, u16 last_column) override;
};

class VirtualConsole final : public TTY
    , public KeyboardClient
    , public VT::TerminalClient {
    BASE_MAKE_ETERNAL
    friend class ConsoleManagement;
    friend class ConsoleImpl;
    friend class VT::Terminal;

public:
    struct Line {
        bool dirty;
        size_t length;
    };

    struct Cell {
        void clear()
        {
            ch = ' ';
            attribute.reset();
        }
        char ch;
        VT::Attribute attribute;
    };

public:
    static NonnullRefPtr<VirtualConsole> create(size_t index);
    static NonnullRefPtr<VirtualConsole> create_with_preset_log(size_t index, const CircularQueue<char, 16384>&);

    virtual ~VirtualConsole() override;

    size_t index() const { return m_index; }

    void refresh_after_resolution_change();

    bool is_graphical() { return m_graphical; }
    void set_graphical(bool graphical);

    void emit_char(char);

private:
    explicit VirtualConsole(const unsigned index);
    VirtualConsole(const unsigned index, const CircularQueue<char, 16384>&);

    virtual void on_key_pressed(KeyEvent) override;

    virtual KResultOr<size_t> on_tty_write(const UserOrKernelBuffer&, size_t) override;
    virtual String const& tty_name() const override { return m_tty_name; }
    virtual void echo(u8) override;

    virtual void beep() override;
    virtual void set_window_title(const StringView&) override;
    virtual void set_window_progress(int, int) override;
    virtual void terminal_did_resize(u16 columns, u16 rows) override;
    virtual void terminal_history_changed(int) override;
    virtual void emit(const u8*, size_t) override;
    virtual void set_cursor_style(VT::CursorStyle) override;

    virtual StringView class_name() const override { return "VirtualConsole"; }

    virtual String device_name() const override;

    void set_active(bool);
    void flush_dirty_lines();

    unsigned m_index;
    bool m_active { false };
    bool m_graphical { false };

    String m_tty_name;

private:
    void initialize();

    void invalidate_cursor(size_t row);

    void clear();

    void inject_string(const StringView&);

    Cell& cell_at(size_t column, size_t row);

    typedef Vector<unsigned, 4> ParamVector;

    void on_code_point(u32);

    void scroll_down(u16 region_top, u16 region_bottom, size_t count);
    void scroll_up(u16 region_top, u16 region_bottom, size_t count);
    void scroll_left(u16 row, u16 column, size_t count);
    void scroll_right(u16 row, u16 column, size_t count);
    void clear_line(size_t index)
    {
        clear_in_line(index, 0, m_console_impl.columns() - 1);
    }
    void clear_in_line(u16 row, u16 first_column, u16 last_column);
    void put_character_at(unsigned row, unsigned column, u32 ch, const VT::Attribute&);

    OwnPtr<Memory::Region> m_cells;
    Vector<VirtualConsole::Line> m_lines;
    ConsoleImpl m_console_impl;
};

}
