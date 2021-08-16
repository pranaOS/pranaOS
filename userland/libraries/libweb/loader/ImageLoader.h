/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Function.h>
#include <libcore/Timer.h>
#include <libweb/loader/ImageResource.h>

namespace Web {

class ImageLoader : public ImageResourceClient {
public:
    ImageLoader(DOM::Element& owner_element);

    void load(const URL&);

    const Gfx::Bitmap* bitmap(size_t index) const;
    size_t current_frame_index() const { return m_current_frame_index; }

    bool has_image() const;

    bool has_loaded_or_failed() const { return m_loading_state != LoadingState::Loading; }

    void set_visible_in_viewport(bool) const;

    unsigned width() const;
    unsigned height() const;

    Function<void()> on_load;
    Function<void()> on_fail;
    Function<void()> on_animate;

private:

    virtual void resource_did_load() override;
    virtual void resource_did_fail() override;
    virtual bool is_visible_in_viewport() const override { return m_visible_in_viewport; }

    void animate();

    enum class LoadingState {
        None,
        Loading,
        Loaded,
        Failed,
    };

    DOM::Element& m_owner_element;

    mutable bool m_visible_in_viewport { false };

    size_t m_current_frame_index { 0 };
    size_t m_loops_completed { 0 };
    LoadingState m_loading_state { LoadingState::Loading };
    NonnullRefPtr<Core::Timer> m_timer;
};

}
