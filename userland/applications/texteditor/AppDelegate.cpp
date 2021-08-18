#include "TextEditorView.h"
#include "TextEditorViewController.h"
#include <csignal>
#include <libui/AppDelegate.h>

static int shell_pid = 0;

int setup_shell()
{
    int ptmx = posix_openpt(O_RDONLY);
    printf("hello");
    return ptmx;
}

class AppDelegate : public UI::AppDelegate {
public:
    AppDelegate() = default;
    virtual ~AppDelegate() = default;

    LG::Size preferred_desktop_window_size() const override { return LG::Size(400, 300); }
    const char* icon_path() const override { return "/res/"; }

    bool application() override
    {
        int ptmx = setup_shell();
        auto& window = std::pranaos::construct<UI::Window>(window_size(), icon_path());
        auto& superview = window.create_superview<TextEditorView, TextEditorViewController>(ptmx);
        window.set_focused_view(superview);
        window.set_frame_style(LG::Color(0x181818));
        window.set_title("TextEditor");
        return true;
    }

private:
};

SET_APP_DELEGATE(AppDelegate);
