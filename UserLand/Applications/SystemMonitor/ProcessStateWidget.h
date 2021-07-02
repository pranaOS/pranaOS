#pragma once

// includes
#include <LibGUI/Widget.h>

class ProcessStateWidget final : public GUI::Widget {
    C_OBJECT(ProcessStateWidget);

public:
    virtual ~ProcessStateWidget() override;

private:
    explicit ProcessStateWidget(pid_t);
    RefPtr<GUI::TableView> m_table_view;
};
