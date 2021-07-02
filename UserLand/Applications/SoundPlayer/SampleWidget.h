#pragma once

// includes
#include "VisualizationBase.h"
#include <LibGUI/Frame.h>

namespace Audio {
class Buffer;
}

class SampleWidget final : public GUI::Frame
    , public Visualization {
    C_OBJECT(SampleWidget)
public:
    virtual ~SampleWidget() override;

    void set_buffer(RefPtr<Audio::Buffer>) override;

private:
    SampleWidget();
    virtual void paint_event(GUI::PaintEvent&) override;

    RefPtr<Audio::Buffer> m_buffer;
};
