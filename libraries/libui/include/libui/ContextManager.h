/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include <libg/Context.h>

namespace UI {

static inline void graphics_push_context(LG::Context&& context)
{
    extern std::vector<LG::Context> s_ui_graphics_contexts;
    s_ui_graphics_contexts.push_back(std::move(context));
}

static inline void graphics_push_context(const LG::Context& context)
{
    extern std::vector<LG::Context> s_ui_graphics_contexts;
    s_ui_graphics_contexts.push_back(context);
}

static inline void graphics_pop_context()
{
    extern std::vector<LG::Context> s_ui_graphics_contexts;
    s_ui_graphics_contexts.pop_back();
}

static inline LG::Context& graphics_current_context()
{
    extern std::vector<LG::Context> s_ui_graphics_contexts;
    return s_ui_graphics_contexts.back();
}

} // namespace UI