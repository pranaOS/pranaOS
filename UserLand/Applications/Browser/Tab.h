#pragma once

// includes

#include "History.h"
#include <AK/URL.h>
#include <LibGUI/Widget.h>
#include <LibGfx/ShareableBitmap.h>
#include <LibHTTP/HttpJob.h>
#include <LibWeb/Forward.h>

namespace Web {
class OutOfProcessWebView;
class WebViewHooks;
}

namespace Browser {

class Tab final : public GUI::Widget {
    C_OBJECT(Tab);

public:
    enum class Type {
        InProcessWebView,
        OutOfProcessWebView,
    };

    virtual ~Tab() override;

    URL url() const;

    enum class LoadType {
        Normal,
        HistoryNavigation,
    };

    void load(const URL&, LoadType = LoadType::Normal);
    void reload();
    void go_back();
    void go_forward();

    void did_become_active();
    void context_menu_requested(const Gfx::IntPoint& screen_position);

    Function<void(String)> on_title_change;
    Function<void(const URL&)> on_tab_open_request;
    Function<void(Tab&)> on_tab_close_request;
    Function<void(const Gfx::Bitmap&)> on_favicon_change;
    Function<String(const URL& url)> on_get_cookie;
    Function<void(const URL& url, const String& cookie)> on_set_cookie;

    const String& title() const { return m_title; }
    const Gfx::Bitmap* icon() const { return m_icon; }

    GUI::ScrollableWidget& view();

private:
    explicit Tab(Type);

    Web::WebViewHooks& hooks();
    void update_actions();
    void update_bookmark_button(const String& url);
    void start_download(const URL& url);
    void view_source(const URL& url, const String& source);

    Type m_type;

    History m_history;

    RefPtr<Web::InProcessWebView> m_page_view;
    RefPtr<Web::OutOfProcessWebView> m_web_content_view;

    RefPtr<GUI::Action> m_go_back_action;
    RefPtr<GUI::Action> m_go_forward_action;
    RefPtr<GUI::Action> m_go_home_action;
    RefPtr<GUI::Action> m_reload_action;
    RefPtr<GUI::TextBox> m_location_box;
    RefPtr<GUI::Button> m_bookmark_button;
    RefPtr<GUI::Window> m_dom_inspector_window;
    RefPtr<GUI::Window> m_console_window;
    RefPtr<GUI::StatusBar> m_statusbar;
    RefPtr<GUI::MenuBar> m_menubar;
    RefPtr<GUI::ToolBarContainer> m_toolbar_container;

    RefPtr<GUI::Menu> m_link_context_menu;
    RefPtr<GUI::Action> m_link_context_menu_default_action;
    URL m_link_context_menu_url;

    RefPtr<GUI::Menu> m_image_context_menu;
    Gfx::ShareableBitmap m_image_context_menu_bitmap;
    URL m_image_context_menu_url;

    RefPtr<GUI::Menu> m_tab_context_menu;
    RefPtr<GUI::Menu> m_page_context_menu;

    String m_title;
    RefPtr<const Gfx::Bitmap> m_icon;

    bool m_is_history_navigation { false };
};

URL url_from_user_input(const String& input);

}