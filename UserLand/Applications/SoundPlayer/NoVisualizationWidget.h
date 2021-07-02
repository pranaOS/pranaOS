#pragma once

// includes
#include "VisualizationBase.h"
#include <LibAudio/Buffer.h>
#include <LibGUI/Frame.h>

class NoVisualizationWidget final : public GUI::Frame
    , public Visualization {
    C_OBJECT(NoVisualizationWidget)

public:
    ~NoVisualizationWidget() override;
    void set_buffer(RefPtr<Audio::Buffer>) override;

private:
    void paint_event(GUI::PaintEvent&) override;
    NoVisualizationWidget();

    RefPtr<Gfx::Bitmap> m_pranaos_bg;
};
