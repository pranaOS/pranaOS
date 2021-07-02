// includes
#include "DesktopWidget.h"
#include <LibGUI/Painter.h>

namespace FileManager {

DesktopWidget::DesktopWidget()
{
}

DesktopWidget::~DesktopWidget()
{
}

void DesktopWidget::paint_event(GUI::PaintEvent& event)
{
    GUI::Painter painter(*this);
    painter.add_clip_rect(event.rect());
    painter.clear_rect(event.rect(), Color(0, 0, 0, 0));
}

}
