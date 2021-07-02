#include <LibGUI/Application.h>
#include <LibGUI/Painter.h>
#include <LibGUI/Widget.h>
#include <LibGUI/Window.h>
#include <LibGfx/FontDatabase.h>
#include <LibGfx/Palette.h>
#include <stdio.h>
#include <unistd.h>

class UserNameWidget final : public GUI::Widget {
    C_OBJECT(UserNameWidget)
public:
    UserNameWidget()
    {
        m_username = getlogin();
        m_username_width = Gfx::FontDatabase::default_bold_font().width(m_username);
    }

    virtual ~UserNameWidget() override { }

    int get_width()
    {
        return m_username_width + menubar_menu_margin();
    }

private:
    static int menubar_menu_margin() { return 4; }

    virtual void paint_event(GUI::PaintEvent& event) override
    {
        GUI::Painter painter(*this);
        painter.fill_rect(event.rect(), palette().window());
        painter.draw_text(event.rect(), m_username, Gfx::FontDatabase::default_bold_font(), Gfx::TextAlignment::Center, palette().window_text());
    }

    String m_username;
    int m_username_width;
};

int main(int argc, char** argv)
{
    if (pledge("stdio recvfd sendfd rpath cpath unix fattr", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    auto app = GUI::Application::construct(argc, argv);

    if (pledge("stdio recvfd sendfd rpath", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    if (unveil("/res", "r") < 0) {
        perror("unveil");
        return 1;
    }

    if (unveil("/etc/passwd", "r") < 0) {
        perror("unveil");
        return 1;
    }

    unveil(nullptr, nullptr);

    auto window = GUI::Window::construct();
    window->set_title("UserName");
    window->set_window_type(GUI::WindowType::Applet);

    auto& widget = window->set_main_widget<UserNameWidget>();
    window->resize(widget.get_width(), 16);
    window->show();

    return app->exec();
}
