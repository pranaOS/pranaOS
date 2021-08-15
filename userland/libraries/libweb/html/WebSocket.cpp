/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/Interpreter.h>
#include <libjs/Parser.h>
#include <libjs/Runtime/FunctionObject.h>
#include <libjs/Runtime/OrdinaryFunctionObject.h>
#include <libprotocol/WebSocket.h>
#include <libprotocol/WebSocketClient.h>
#include <libweb/bindings/EventWrapper.h>
#include <libweb/bindings/WebSocketWrapper.h>
#include <libweb/dom/DOMException.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Event.h>
#include <libweb/dom/EventDispatcher.h>
#include <libweb/dom/EventListener.h>
#include <libweb/dom/ExceptionOr.h>
#include <libweb/dom/Window.h>
#include <libweb/html/CloseEvent.h>
#include <libweb/html/EventHandler.h>
#include <libweb/html/EventNames.h>
#include <libweb/html/MessageEvent.h>
#include <libweb/html/WebSocket.h>
#include <libweb/Origin.h>

namespace Web::HTML {

WebSocketClientManager& WebSocketClientManager::the()
{
    static WebSocketClientManager* s_the;
    if (!s_the)
        s_the = &WebSocketClientManager::construct().leak_ref();
    return *s_the;
}

WebSocketClientManager::WebSocketClientManager()
    : m_websocket_client(Protocol::WebSocketClient::construct())
{
}

RefPtr<Protocol::WebSocket> WebSocketClientManager::connect(const URL& url)
{
    return m_websocket_client->connect(url);
}

DOM::ExceptionOr<NonnullRefPtr<WebSocket>> WebSocket::create_with_global_object(Bindings::WindowObject& window, const String& url)
{
    URL url_record(url);
    if (!url_record.is_valid())
        return DOM::SyntaxError::create("Invalid URL");
    if (!url_record.protocol().is_one_of("ws", "wss"))
        return DOM::SyntaxError::create("Invalid protocol");
    if (!url_record.fragment().is_empty())
        return DOM::SyntaxError::create("Presence of URL fragment is invalid");
    
    return WebSocket::create(window.impl(), url_record);
}

WebSocket::WebSocket(DOM::Window& window, URL& url)
    : EventTarget(static_cast<Bindings::ScriptExecutionContext&>(window.document()))
    , m_window(window)
{
    m_websocket = WebSocketClientManager::the().connect(url);
    m_websocket->on_open = [weak_this = make_weak_ptr()] {
        if (!weak_this)
            return;
        auto& websocket = const_cast<WebSocket&>(*weak_this);
        websocket.on_open();
    };
    m_websocket->on_message = [weak_this = make_weak_ptr()](auto message) {
        if (!weak_this)
            return;
        auto& websocket = const_cast<WebSocket&>(*weak_this);
        websocket.on_message(move(message.data), message.is_text);
    };
    m_websocket->on_close = [weak_this = make_weak_ptr()](auto code, auto reason, bool was_clean) {
        if (!weak_this)
            return;
        auto& websocket = const_cast<WebSocket&>(*weak_this);
        websocket.on_close(code, reason, was_clean);
    };
    m_websocket->on_error = [weak_this = make_weak_ptr()](auto) {
        if (!weak_this)
            return;
        auto& websocket = const_cast<WebSocket&>(*weak_this);
        websocket.on_error();
    };
}

WebSocket::~WebSocket()
{
}

WebSocket::ReadyState WebSocket::ready_state() const
{
    if (!m_websocket)
        return WebSocket::ReadyState::Closed;
    auto ready_state = const_cast<Protocol::WebSocket&>(*m_websocket).ready_state();
    switch (ready_state) {
    case Protocol::WebSocket::ReadyState::Connecting:
        return WebSocket::ReadyState::Connecting;
    case Protocol::WebSocket::ReadyState::Open:
        return WebSocket::ReadyState::Open;
    case Protocol::WebSocket::ReadyState::Closing:
        return WebSocket::ReadyState::Closing;
    case Protocol::WebSocket::ReadyState::Closed:
        return WebSocket::ReadyState::Closed;
    }
    return WebSocket::ReadyState::Closed;
}

String WebSocket::extensions() const
{
    if (!m_websocket)
        return String::empty();
    return String::empty();
}

String WebSocket::protocol() const
{
    if (!m_websocket)
        return String::empty();

    return String::empty();
}

DOM::ExceptionOr<void> WebSocket::close(u16 code, const String& reason)
{
    if (code == 0)
        code = 1000;
    if (code != 1000 && (code < 3000 || code > 4099))
        return DOM::InvalidAccessError::create("The close error code is invalid");
    if (!reason.is_empty() && reason.bytes().size() > 123)
        return DOM::SyntaxError::create("The close reason is longer than 123 bytes");
    auto state = ready_state();
    if (state == WebSocket::ReadyState::Closing || state == WebSocket::ReadyState::Closed)
        return {};
    m_websocket->close(code, reason);
    return {};
}

DOM::ExceptionOr<void> WebSocket::send(const String& data)
{
    auto state = ready_state();
    if (state == WebSocket::ReadyState::Connecting)
        return DOM::InvalidStateError::create("Websocket is still CONNECTING");
    if (state == WebSocket::ReadyState::Open) {
        m_websocket->send(data);

    }
    return {};
}

void WebSocket::on_open()
{
    dispatch_event(DOM::Event::create(EventNames::open));
}

void WebSocket::on_error()
{
    dispatch_event(DOM::Event::create(EventNames::error));
}

void WebSocket::on_close(u16 code, String reason, bool was_clean)
{
    dispatch_event(CloseEvent::create(EventNames::close, was_clean, code, reason));
}

void WebSocket::on_message(ByteBuffer message, bool is_text)
{
    if (m_websocket->ready_state() != Protocol::WebSocket::ReadyState::Open)
        return;
    if (is_text) {
        auto text_message = String(ReadonlyBytes(message));
        dispatch_event(MessageEvent::create(EventNames::message, text_message, url()));
        return;
    }
    TODO();
}

bool WebSocket::dispatch_event(NonnullRefPtr<DOM::Event> event)
{
    return DOM::EventDispatcher::dispatch(*this, move(event));
}

JS::Object* WebSocket::create_wrapper(JS::GlobalObject& global_object)
{
    return wrap(global_object, *this);
}

#undef __ENUMERATE
#define __ENUMERATE(attribute_name, event_name)                    \
    void WebSocket::set_##attribute_name(HTML::EventHandler value) \
    {                                                              \
        set_event_handler_attribute(event_name, move(value));      \
    }                                                              \
    HTML::EventHandler WebSocket::attribute_name()                 \
    {                                                              \
        return get_event_handler_attribute(event_name);            \
    }
ENUMERATE_WEBSOCKET_EVENT_HANDLERS(__ENUMERATE)
#undef __ENUMERATE

void WebSocket::set_event_handler_attribute(const FlyString& name, HTML::EventHandler value)
{
    RefPtr<DOM::EventListener> listener;
    if (!value.callback.is_null()) {
        listener = adopt_ref(*new DOM::EventListener(move(value.callback)));
    } else {
        StringBuilder builder;
        builder.appendff("function {}(event) {{\n{}\n}}", name, value.string);
        auto parser = JS::Parser(JS::Lexer(builder.string_view()));
        auto program = parser.parse_function_node<JS::FunctionExpression>();
        if (parser.has_errors()) {
            dbgln("Failed to parse script in event handler attribute '{}'", name);
            return;
        }
        auto* function = JS::OrdinaryFunctionObject::create(script_execution_context()->interpreter().global_object(), name, program->body(), program->parameters(), program->function_length(), nullptr, JS::FunctionKind::Regular, false, false);
        VERIFY(function);
        listener = adopt_ref(*new DOM::EventListener(JS::make_handle(static_cast<JS::FunctionObject*>(function))));
    }
    if (listener) {
        for (auto& registered_listener : listeners()) {
            if (registered_listener.event_name == name && registered_listener.listener->is_attribute()) {
                remove_event_listener(name, registered_listener.listener);
                break;
            }
        }
        add_event_listener(name, listener.release_nonnull());
    }
}

HTML::EventHandler WebSocket::get_event_handler_attribute(const FlyString& name)
{
    for (auto& listener : listeners()) {
        if (listener.event_name == name && listener.listener->is_attribute())
            return HTML::EventHandler { JS::make_handle(&listener.listener->function()) };
    }

    return {};
}

}
