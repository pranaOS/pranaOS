// includes
#include "NoVisualizationWidget.h"
#include <LibGUI/Painter.h>

NoVisualizationWidget::NoVisualizationWidget()
{
}

NoVisualizationWidget::~NoVisualizationWidget()
{
}

void NoVisualizationWidget::paint_event(GUI::PaintEvent& event)
{
    Frame::paint_event(event);
    GUI::Painter painter(*this);

    if (m_pranaos_bg.is_null()) {
        m_pranaos_bg = Gfx::Bitmap::load_from_file("/res/wallpapers/sunset-retro.png");
    }
    painter.draw_scaled_bitmap(frame_inner_rect(), *m_pranaos_bg, m_pranaos_bg->rect(), 1.0f);
    return;
}

void NoVisualizationWidget::set_buffer(RefPtr<Audio::Buffer>)
{
}
