// includes
#include "TextEditorView.h"
#include "TextEditorViewController.h"
#include <csignal>
#include <libui/AppDelegate.h>
#include "TextEditorView.h"
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int shell_pid = 0;

int setup_texteditor()
{
    int setup = posix_openpt(O_RDONLY);
    printf("hello");
    return setup;
}

class AppDelegate : public UI::AppDelegate {
public:
    AppDelegate() = default;
    virtual ~AppDelegate() = default;

    LG::Size preferred_desktop_window_size() const override { return LG::Size(400, 300); }
    const char* icon_path() const override { return "/res/icons/apps/terminal.icon"; }

    bool application() override
    {
        int setup = setup_texteditor();
        auto& window = std::pranaos::construct<UI::Window>(window_size(), icon_path());
        auto& superview = window.create_superview<TextEditorView, TextEditorViewController>(setup);
        window.set_focused_view(superview);
        window.set_frame_style(LG::Color(0x181818));
        window.set_title("TextEditor");
        return true;

    }

private:
};

SET_APP_DELEGATE(AppDelegate);
