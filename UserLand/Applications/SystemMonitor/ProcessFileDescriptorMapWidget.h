#pragma once

// includes
#include <LibGUI/Widget.h>

class ProcessFileDescriptorMapWidget final : public GUI::Widget {
    C_OBJECT(ProcessFileDescriptorMapWidget);

public:
    virtual ~ProcessFileDescriptorMapWidget() override;

    void set_pid(pid_t);

private:
    ProcessFileDescriptorMapWidget();

    RefPtr<GUI::TableView> m_table_view;
    RefPtr<GUI::JsonArrayModel> m_model;
    pid_t m_pid { -1 };
};
