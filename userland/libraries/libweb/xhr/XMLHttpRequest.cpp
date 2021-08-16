/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/FunctionObject.h>
#include <libweb/bindings/EventWrapper.h>
#include <libweb/bindings/XMLHttpRequestWrapper.h>
#include <libweb/dom/DOMException.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Event.h>
#include <libweb/dom/EventDispatcher.h>
#include <libweb/dom/EventListener.h>
#include <libweb/dom/ExceptionOr.h>
#include <libweb/dom/Window.h>
#include <libweb/html/EventNames.h>
#include <libweb/loader/ResourceLoader.h>
#include <libweb/Origin.h>
#include <libweb/xhr/EventNames.h>
#include <libweb/xhr/ProgressEvent.h>
#include <libweb/xhr/XMLHttpRequest.h>

namespace Web::XHR {

XMLHttpRequest::XMLHttpRequest(DOM::Window& window)
    : XMLHttpRequestEventTarget(static_cast<Bindings::ScriptExecutionContext&>(window.document()))
    , m_window(window)
{
}

XMLHttpRequest::~XMLHttpRequest()
{
}

void XMLHttpRequest::set_ready_state(ReadyState ready_state)
{
    m_ready_state = ready_state;
    dispatch_event(DOM::Event::create(EventNames::readystatechange));
}

void XMLHttpRequest::fire_progress_event(const String& event_name, u64 transmitted, u64 length)
{
    dispatch_event(ProgressEvent::create(event_name, transmitted, length));
}

String XMLHttpRequest::response_text() const
{
    if (m_response_object.is_empty())
        return {};
    return String::copy(m_response_object);
}

static bool is_forbidden_header_name(const String& header_name)
{
    if (header_name.starts_with("Proxy-", CaseSensitivity::CaseInsensitive) || header_name.starts_with("Sec-", CaseSensitivity::CaseInsensitive))
        return true;

    auto lowercase_header_name = header_name.to_lowercase();
    return lowercase_header_name.is_one_of("accept-charset", "accept-encoding", "access-control-request-headers", "access-control-request-method", "connection", "content-length", "cookie", "cookie2", "date", "dnt", "expect", "host", "keep-alive", "origin", "referer", "te", "trailer", "transfer-encoding", "upgrade", "via");
}

static bool is_forbidden_method(const String& method)
{
    auto lowercase_method = method.to_lowercase();
    return lowercase_method.is_one_of("connect", "trace", "track");
}

static String normalize_method(const String& method)
{
    auto lowercase_method = method.to_lowercase();
    if (lowercase_method.is_one_of("delete", "get", "head", "options", "post", "put"))
        return method.to_uppercase();
    return method;
}

static String normalize_header_value(const String& header_value)
{
    return header_value.trim_whitespace();
}

DOM::ExceptionOr<void> XMLHttpRequest::set_request_header(const String& header, const String& value)
{
    if (m_ready_state != ReadyState::Opened)
        return DOM::InvalidStateError::create("XHR readyState is not OPENED");

    if (m_send)
        return DOM::InvalidStateError::create("XHR send() flag is already set");

    if (is_forbidden_header_name(header))
        return {};

    m_request_headers.set(header, normalize_header_value(value));
    return {};
}

DOM::ExceptionOr<void> XMLHttpRequest::open(const String& method, const String& url)
{

    if (is_forbidden_method(method))
        return DOM::SecurityError::create("Forbidden method, must not be 'CONNECT', 'TRACE', or 'TRACK'");

    auto normalized_method = normalize_method(method);

    auto parsed_url = m_window->document().complete_url(url);
    if (!parsed_url.is_valid())
        return DOM::SyntaxError::create("Invalid URL");

    if (!parsed_url.host().is_null()) {
    }

    m_send = false;
    m_upload_listener = false;
    m_method = normalized_method;
    m_url = parsed_url;

    m_synchronous = false;
    m_request_headers.clear();

    m_response_object = {};

    if (m_ready_state != ReadyState::Opened)
        set_ready_state(ReadyState::Opened);
    return {};
}

DOM::ExceptionOr<void> XMLHttpRequest::send()
{
    if (m_ready_state != ReadyState::Opened)
        return DOM::InvalidStateError::create("XHR readyState is not OPENED");

    if (m_send)
        return DOM::InvalidStateError::create("XHR send() flag is already set");

    URL request_url = m_window->document().complete_url(m_url.to_string());
    dbgln("XHR send from {} to {}", m_window->document().url(), request_url);

    Origin request_url_origin = Origin(request_url.protocol(), request_url.host(), request_url.port());

    if (!m_window->document().origin().is_same(request_url_origin)) {
        dbgln("XHR failed to load: Same-Origin Policy violation: {} may not load {}", m_window->document().url(), request_url);
        auto weak_this = make_weak_ptr();
        if (!weak_this)
            return {};
        const_cast<XMLHttpRequest&>(*weak_this).set_ready_state(ReadyState::Done);
        const_cast<XMLHttpRequest&>(*weak_this).dispatch_event(DOM::Event::create(HTML::EventNames::error));
        return {};
    }

    auto request = LoadRequest::create_for_url_on_page(request_url, m_window->document().page());
    request.set_method(m_method);
    for (auto& it : m_request_headers)
        request.set_header(it.key, it.value);

    m_upload_complete = false;
    m_timed_out = false;

    m_upload_complete = true;

    m_send = true;

    if (!m_synchronous) {
        fire_progress_event(EventNames::loadstart, 0, 0);

        if (m_ready_state != ReadyState::Opened || !m_send)
            return {};

        ResourceLoader::the().load(
            request,
            [weak_this = make_weak_ptr()](auto data, auto& response_headers, auto status_code) {
                if (!weak_this)
                    return;
                auto& xhr = const_cast<XMLHttpRequest&>(*weak_this);
                auto response_data = ByteBuffer::copy(data);
             
                u64 transmitted = response_data.size();
                u64 length = response_data.size();

                if (!xhr.m_synchronous) {
                    xhr.m_response_object = response_data;
                    xhr.fire_progress_event(EventNames::progress, transmitted, length);
                }

                xhr.m_ready_state = ReadyState::Done;
                xhr.m_status = status_code.value_or(0);
                xhr.m_response_headers = move(response_headers);
                xhr.m_send = false;
                xhr.dispatch_event(DOM::Event::create(EventNames::readystatechange));
                xhr.fire_progress_event(EventNames::load, transmitted, length);
                xhr.fire_progress_event(EventNames::loadend, transmitted, length);
            },
            [weak_this = make_weak_ptr()](auto& error, auto status_code) {
                if (!weak_this)
                    return;
                dbgln("XHR failed to load: {}", error);
                const_cast<XMLHttpRequest&>(*weak_this).set_ready_state(ReadyState::Done);
                const_cast<XMLHttpRequest&>(*weak_this).set_status(status_code.value_or(0));
                const_cast<XMLHttpRequest&>(*weak_this).dispatch_event(DOM::Event::create(HTML::EventNames::error));
            });
    } else {
        TODO();
    }
    return {};
}

bool XMLHttpRequest::dispatch_event(NonnullRefPtr<DOM::Event> event)
{
    return DOM::EventDispatcher::dispatch(*this, move(event));
}

JS::Object* XMLHttpRequest::create_wrapper(JS::GlobalObject& global_object)
{
    return wrap(global_object, *this);
}

}
