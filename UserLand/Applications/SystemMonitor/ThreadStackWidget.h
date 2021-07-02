#pragma once

// includes
#include <LibGUI/TextEditor.h>
#include <LibGUI/Widget.h>

class ThreadStackWidget final : public GUI::Widget {
    C_OBJECT(ThreadStackWidget)
public:
    virtual ~ThreadStackWidget() override;

    void set_ids(pid_t pid, pid_t tid);
    void refresh();

private:
    ThreadStackWidget();

    virtual void show_event(GUI::ShowEvent&) override;
    virtual void hide_event(GUI::HideEvent&) override;

    pid_t m_pid { -1 };
    pid_t m_tid { -1 };
    RefPtr<GUI::TextEditor> m_stack_editor;
    RefPtr<Core::Timer> m_timer;
};
