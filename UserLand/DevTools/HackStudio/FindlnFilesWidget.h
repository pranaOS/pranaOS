#pragma once
// includes
#include <LibGUI/Button.h>
#include <LibGUI/TableView.h>
#include <LibGUI/TextBox.h>
#include <LibGUI/Widget.h>

namespace HackStudio {

class FindInFilesWidget final : public GUI::Widget {
    C_OBJECT(FindInFilesWidget)
public:
    virtual ~FindInFilesWidget() override { }

    void focus_textbox_and_select_all();

private:
    explicit FindInFilesWidget();

    RefPtr<GUI::TextBox> m_textbox;
    RefPtr<GUI::Button> m_button;
    RefPtr<GUI::TableView> m_result_view;
};

}
