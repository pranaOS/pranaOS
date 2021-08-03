/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefCounted.h>
#include <base/Types.h>
#include <kernel/graphics/Console/VGAConsole.h>
#include <kernel/SpinLock.h>

namespace Kernel::Graphics {
class TextModeConsole final : public VGAConsole {
public:
    static NonnullRefPtr<TextModeConsole> initialize(const VGACompatibleAdapter& adapter);
    virtual size_t chars_per_line() const override { return width(); };

    virtual bool has_hardware_cursor() const override { return true; }
    virtual bool is_hardware_paged_capable() const override { return true; }

    virtual size_t bytes_per_base_glyph() const override { return 2; }
    virtual void set_cursor(size_t x, size_t y) override;
    virtual void hide_cursor() override;
    virtual void show_cursor() override;
    virtual void clear(size_t x, size_t y, size_t length) override;
    virtual void write(size_t x, size_t y, char ch, bool critical = false) override;
    virtual void write(size_t x, size_t y, char ch, Color background, Color foreground, bool critical = false) override;
    virtual void write(char ch, bool critical = false) override;
    virtual void flush(size_t, size_t, size_t, size_t) override { }

    virtual void enable() override { }
    virtual void disable() override { VERIFY_NOT_REACHED(); }

}