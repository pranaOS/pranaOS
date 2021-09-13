#include "ViewController.h"
#include <libui/AppDelegate.h>
#include <libui/MenuBar.h>

class AppDelegate : public UI::AppDelegate {
public:
    AppDelegate() = default;
    virtual ~AppDelegate() = default;

    LG::Size preferred_desktop_window_size() const override { return LG::Size(200, 140); }
    const char* icon_path() const override { return "/res/icons/apps/about.icon"; }

    virtual bool application() override
    {
        auto& window = std::pranaos::construct<UI::Window>(window_size(), icon_path());
        auto& superview = window.create_superview<UI::View, ViewController>();

        window.set_title("About");

        // FIXME: Popup when click on the perticular MenuItem
        window.menubar().add_menu("Edit").add_item(UI::MenuItem("Edit", [] { Logger::debug << "Edit" << std::endl; }));
        window.menubar().add_menu("View").add_item(UI::MenuItem("View", [] { Logger::debug << "View" << std::endl; }));
        window.menubar().add_menu("Help").add_item(UI::MenuItem("Help", [] { Logger::debug << "Help" << std::endl; }));

        return true;
    }

private:
};

SET_APP_DELEGATE(AppDelegate);
