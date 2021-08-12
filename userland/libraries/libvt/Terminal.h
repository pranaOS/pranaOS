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
#include <libvt/EscapeSequenceParser.h>
#include <libvt/Position.h>

#ifndef KERNEL
#    include <libvt/Attribute.h>
#    include <libvt/Line.h>
#else
namespace Kernel {
class VirtualConsole;
}
#    include <libvt/Attribute.h>
#endif

namespace VT {

enum CursorStyle {
    None,
    BlinkingBlock,
    SteadyBlock,
    BlinkingUnderline,
    SteadyUnderline,
    BlinkingBar,
    SteadyBar
};

class TerminalClient {
public:
    virtual ~TerminalClient() { }

    virtual void beep() = 0;
    virtual void set_window_title(const StringView&) = 0;
    virtual void set_window_progress(int value, int max) = 0;
    virtual void terminal_did_resize(u16 columns, u16 rows) = 0;
    virtual void terminal_history_changed(int delta) = 0;
    virtual void emit(const u8*, size_t) = 0;
    virtual void set_cursor_style(CursorStyle) = 0;
};

class Terminal : public EscapeSequenceExecutor {
public:
#ifndef KERNEL
    explicit Terminal(TerminalClient&);
#else
    explicit Terminal(Kernel::VirtualConsole&);
#endif

    virtual ~Terminal()
    {
    }

    bool m_need_full_flush { false };

#ifndef KERNEL
    void invalidate_cursor();
#else
    virtual void invalidate_cursor() = 0;
#endif

    void on_input(u8);

    void set_cursor(unsigned row, unsigned column, bool skip_debug = false);

    void clear_including_history()
    {
        clear_history();
        clear();
    }

#ifndef KERNEL
    void clear();
    void clear_history();
#else
    virtual void clear() = 0;
    virtual void clear_history() = 0;
#endif

#ifndef KERNEL
    void set_size(u16 columns, u16 rows);
#else
    virtual void set_size(u16 columns, u16 rows) = 0;
#endif

    u16 columns() const
    {
        return m_columns;
    }
    u16 rows() const { return m_rows; }

    u16 cursor_column() const { return m_current_state.cursor.column; }
    u16 cursor_row() const { return m_current_state.cursor.row; }

#ifndef KERNEL
    size_t line_count() const
    {
        if (m_use_alternate_screen_buffer)
            return m_alternate_screen_buffer.size();
        else
            return m_history.size() + m_normal_screen_buffer.size();
    }

    Line& line(size_t index)
    {
        if (m_use_alternate_screen_buffer) {
            return m_alternate_screen_buffer[index];
        } else {
            if (index < m_history.size())
                return m_history[(m_history_start + index) % m_history.size()];
            return m_normal_screen_buffer[index - m_history.size()];
        }
    }
    const Line& line(size_t index) const
    {
        return const_cast<Terminal*>(this)->line(index);
    }

    Line& visible_line(size_t index)
    {
        return active_buffer()[index];
    }

    const Line& visible_line(size_t index) const
    {
        return active_buffer()[index];
    }

    size_t max_history_size() const { return m_max_history_lines; }
    void set_max_history_size(size_t value)
    {
        if (value == 0) {
            auto previous_size = m_history.size();
            m_max_history_lines = 0;
            m_history_start = 0;
            m_history.clear();
            m_client.terminal_history_changed(-previous_size);
            return;
        }

        if (m_max_history_lines > value) {
            NonnullOwnPtrVector<Line> new_history;
            new_history.ensure_capacity(value);
            auto existing_line_count = min(m_history.size(), value);
            for (size_t i = m_history.size() - existing_line_count; i < m_history.size(); ++i) {
                auto j = (m_history_start + i) % m_history.size();
                new_history.unchecked_append(move(static_cast<Vector<NonnullOwnPtr<Line>>&>(m_history).at(j)));
            }
            m_history = move(new_history);
            m_history_start = 0;
            m_client.terminal_history_changed(value - existing_line_count);
        }
        m_max_history_lines = value;
    }
    size_t history_size() const { return m_use_alternate_screen_buffer ? 0 : m_history.size(); }
#endif

    void inject_string(const StringView&);
    void handle_key_press(KeyCode, u32, u8 flags);

#ifndef KERNEL
    Attribute attribute_at(const Position&) const;
#endif

    bool needs_bracketed_paste() const
    {
        return m_needs_bracketed_paste;
    };

    bool is_within_scroll_region(u16 line) const
    {
        return line >= m_scroll_region_top && line <= m_scroll_region_bottom;
    }

protected:

    virtual void emit_code_point(u32) override;
    virtual void execute_control_code(u8) override;
    virtual void execute_escape_sequence(Intermediates intermediates, bool ignore, u8 last_byte) override;
    virtual void execute_csi_sequence(Parameters parameters, Intermediates intermediates, bool ignore, u8 last_byte) override;
    virtual void execute_osc_sequence(OscParameters parameters, u8 last_byte) override;
    virtual void dcs_hook(Parameters parameters, Intermediates intermediates, bool ignore, u8 last_byte) override;
    virtual void receive_dcs_char(u8 byte) override;
    virtual void execute_dcs_sequence() override;

    struct BufferState {
        Attribute attribute;
        CursorPosition cursor;
    };

    void carriage_return();
    inline void scroll_up(size_t count = 1);
    inline void scroll_down(size_t count = 1);
    void linefeed();
#ifndef KERNEL
    void scroll_up(u16 region_top, u16 region_bottom, size_t count);
    void scroll_down(u16 region_top, u16 region_bottom, size_t count);
    void scroll_left(u16 row, u16 column, size_t count);
    void scroll_right(u16 row, u16 column, size_t count);
    void put_character_at(unsigned row, unsigned column, u32 ch);
    void clear_in_line(u16 row, u16 first_column, u16 last_column);
#else
    virtual void scroll_up(u16 region_top, u16 region_bottom, size_t count) = 0;
    virtual void scroll_down(u16 region_top, u16 region_bottom, size_t count) = 0;
    virtual void scroll_left(u16 row, u16 column, size_t count) = 0;
    virtual void scroll_right(u16 row, u16 column, size_t count) = 0;
    virtual void put_character_at(unsigned row, unsigned column, u32 ch) = 0;
    virtual void clear_in_line(u16 row, u16 first_column, u16 last_column) = 0;
#endif

    void unimplemented_control_code(u8);
    void unimplemented_escape_sequence(Intermediates, u8 last_byte);
    void unimplemented_csi_sequence(Parameters, Intermediates, u8 last_byte);
    void unimplemented_osc_sequence(OscParameters, u8 last_byte);

    void emit_string(const StringView&);

    void alter_ansi_mode(bool should_set, Parameters);
    void alter_private_mode(bool should_set, Parameters);

    void CUU(Parameters);

    void CUD(Parameters);

    void CUF(Parameters);

    void CUB(Parameters);

    void CNL(Parameters);

    void CPL(Parameters);

    void CUP(Parameters);

    void ED(Parameters);

    void EL(Parameters);

    void SGR(Parameters);

    void SCOSC();

    void SCORC();

    void DECSC();

    void DECRC();

    void DECSTBM(Parameters);

    void RM(Parameters);

    void DECRST(Parameters);

    void SM(Parameters);

    void DECSET(Parameters);

    void DA(Parameters);

    void HVP(Parameters);

    void NEL();

    void IND();

    void RI();

    void DECBI();

    void DECFI();

    void DSR(Parameters);

    void DECSCUSR(Parameters);

    void ICH(Parameters);

    void SU(Parameters);

    void SD(Parameters);

    void IL(Parameters);

    void DCH(Parameters);

    void DL(Parameters);

    void CHA(Parameters);

    void REP(Parameters);

    void VPA(Parameters);

    void VPR(Parameters);

    void HPA(Parameters);

    void HPR(Parameters);

    void ECH(Parameters);

    void XTERM_WM(Parameters);

    void DECIC(Parameters);

    void DECDC(Parameters);

#ifndef KERNEL
    TerminalClient& m_client;
#else
    Kernel::VirtualConsole& m_client;
#endif

    EscapeSequenceParser m_parser;
#ifndef KERNEL
    size_t m_history_start = 0;
    NonnullOwnPtrVector<Line> m_history;
    void add_line_to_history(NonnullOwnPtr<Line>&& line)
    {
        if (max_history_size() == 0)
            return;

        if (m_history.size() < max_history_size()) {
            VERIFY(m_history_start == 0);
            m_history.append(move(line));
            return;
        }
        m_history.ptr_at(m_history_start) = move(line);
        m_history_start = (m_history_start + 1) % m_history.size();
    }

    NonnullOwnPtrVector<Line>& active_buffer() { return m_use_alternate_screen_buffer ? m_alternate_screen_buffer : m_normal_screen_buffer; };
    const NonnullOwnPtrVector<Line>& active_buffer() const { return m_use_alternate_screen_buffer ? m_alternate_screen_buffer : m_normal_screen_buffer; };
    NonnullOwnPtrVector<Line> m_normal_screen_buffer;
    NonnullOwnPtrVector<Line> m_alternate_screen_buffer;
#endif

    bool m_use_alternate_screen_buffer { false };

    size_t m_scroll_region_top { 0 };
    size_t m_scroll_region_bottom { 0 };

    u16 m_columns { 1 };
    u16 m_rows { 1 };

    BufferState m_current_state;
    BufferState m_normal_saved_state;
    BufferState m_alternate_saved_state;

    CursorPosition m_saved_cursor_position;

    bool m_swallow_current { false };
    bool m_stomp { false };

    CursorStyle m_cursor_style { BlinkingBlock };
    CursorStyle m_saved_cursor_style { BlinkingBlock };

    bool m_needs_bracketed_paste { false };

    Attribute m_current_attribute;
    Attribute m_saved_attribute;

    String m_current_window_title;
    Vector<String> m_title_stack;

#ifndef KERNEL
    u32 m_next_href_id { 0 };
#endif

    Vector<bool> m_horizontal_tabs;
    u32 m_last_code_point { 0 };
    size_t m_max_history_lines { 1024 };

    Optional<u16> m_column_before_carriage_return;
    bool m_controls_are_logically_generated { false };
};

}
