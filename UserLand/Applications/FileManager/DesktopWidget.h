#pragma once

// includes
#include <LibGUI/Widget.h>

namespace FileManager {

class DesktopWidget final : public GUI::Widget {
    C_OBJECT(DesktopWidget);

public:
    virtual ~DesktopWidget() override;

private:
    virtual void paint_event(GUI::PaintEvent&) override;

    DesktopWidget();
};

}