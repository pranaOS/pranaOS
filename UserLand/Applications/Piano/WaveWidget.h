#pragma once

#include <LibGUI/Frame.h>

class TrackManager;

class WaveWidget final : public GUI::Frame {
    C_OBJECT(WaveWidget)
public:
    virtual ~WaveWidget() override;

private:
    explicit WaveWidget(TrackManager&);

    virtual void paint_event(GUI::PaintEvent&) override;

    int sample_to_y(int sample) const;

    TrackManager& m_track_manager;
};
