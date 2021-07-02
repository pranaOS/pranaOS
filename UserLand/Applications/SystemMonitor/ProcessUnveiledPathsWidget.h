#pragma once

// includes
#include <LibGUI/Widget.h>

class ProcessUnveiledPathsWidget final : public GUI::Widget {
    C_OBJECT(ProcessUnveiledPathsWidget);

public:
    virtual ~ProcessUnveiledPathsWidget() override;

    void set_pid(pid_t);

private:
    ProcessUnveiledPathsWidget();

    RefPtr<GUI::TableView> m_table_view;
    RefPtr<GUI::JsonArrayModel> m_model;
    pid_t m_pid { -1 };
};
