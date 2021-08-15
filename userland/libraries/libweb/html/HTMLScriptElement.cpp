/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Debug.h>
#include <base/StringBuilder.h>
#include <libjs/Parser.h>
#include <libtextcodec/Decoder.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Event.h>
#include <libweb/dom/ShadowRoot.h>
#include <libweb/dom/Text.h>
#include <libweb/html/EventNames.h>
#include <libweb/html/HTMLScriptElement.h>
#include <libweb/loader/ResourceLoader.h>

namespace Web::HTML {

HTMLScriptElement::HTMLScriptElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
    , m_script_filename("(document)")
{
}

HTMLScriptElement::~HTMLScriptElement()
{
}

void HTMLScriptElement::set_parser_document(Badge<HTMLDocumentParser>, DOM::Document& document)
{
    m_parser_document = document;
}

void HTMLScriptElement::set_non_blocking(Badge<HTMLDocumentParser>, bool non_blocking)
{
    m_non_blocking = non_blocking;
}

void HTMLScriptElement::execute_script()
{
    if (m_preparation_time_document.ptr() != &document()) {
        dbgln("HTMLScriptElement: Refusing to run script because the preparation time document is not the same as the node document.");
        return;
    }

    if (m_script_source.is_null()) {
        dbgln("HTMLScriptElement: Refusing to run script because the script source is null.");
        dispatch_event(DOM::Event::create(HTML::EventNames::error));
        return;
    }

    bool incremented_destructive_writes_counter = false;

    if (m_from_an_external_file || m_script_type == ScriptType::Module) {
        document().increment_ignore_destructive_writes_counter();
        incremented_destructive_writes_counter = true;
    }

    if (m_script_type == ScriptType::Classic) {
        auto old_current_script = document().current_script();
        if (!is<DOM::ShadowRoot>(root()))
            document().set_current_script({}, this);
        else
            document().set_current_script({}, nullptr);

        if (m_from_an_external_file)
            dbgln_if(HTML_SCRIPT_DEBUG, "HTMLScriptElement: Running script {}", attribute(HTML::AttributeNames::src));
        else
            dbgln_if(HTML_SCRIPT_DEBUG, "HTMLScriptElement: Running inline script");

        document().run_javascript(m_script_source, m_script_filename);

        document().set_current_script({}, old_current_script);
    } else {
        VERIFY(!document().current_script());
        TODO();
    }

    if (incremented_destructive_writes_counter)
        document().decrement_ignore_destructive_writes_counter();

    if (m_from_an_external_file)
        dispatch_event(DOM::Event::create(HTML::EventNames::load));
}

static bool is_javascript_mime_type_essence_match(const String& string)
{
    auto lowercase_string = string.to_lowercase();
    return lowercase_string.is_one_of("application/ecmascript", "application/javascript", "application/x-ecmascript", "application/x-javascript", "text/ecmascript", "text/javascript", "text/javascript1.0", "text/javascript1.1", "text/javascript1.2", "text/javascript1.3", "text/javascript1.4", "text/javascript1.5", "text/jscript", "text/livescript", "text/x-ecmascript", "text/x-javascript");
}

void HTMLScriptElement::prepare_script()
{
    if (m_already_started) {
        dbgln("HTMLScriptElement: Refusing to run script because it has already started.");
        return;
    }

    RefPtr<DOM::Document> parser_document = m_parser_document.ptr();
    m_parser_document = nullptr;

    if (parser_document && !has_attribute(HTML::AttributeNames::async)) {
        m_non_blocking = true;
    }

    auto source_text = child_text_content();
    if (!has_attribute(HTML::AttributeNames::src) && source_text.is_empty()) {
        dbgln("HTMLScriptElement: Refusing to run empty script.");
        return;
    }

    if (!is_connected()) {
        dbgln("HTMLScriptElement: Refusing to run script because the element is not connected.");
        return;
    }

    String script_block_type;
    bool has_type = has_attribute(HTML::AttributeNames::type);
    bool has_language = has_attribute(HTML::AttributeNames::language);
    if ((has_type && attribute(HTML::AttributeNames::type).is_empty())
        || (!has_type && has_language && attribute(HTML::AttributeNames::language).is_empty())
        || (!has_type && !has_language)) {
        script_block_type = "text/javascript";
    } else if (has_type) {
        script_block_type = attribute(HTML::AttributeNames::type).trim_whitespace();
    } else if (!attribute(HTML::AttributeNames::language).is_empty()) {
        script_block_type = String::formatted("text/{}", attribute(HTML::AttributeNames::language));
    }

    if (is_javascript_mime_type_essence_match(script_block_type)) {
        m_script_type = ScriptType::Classic;
    } else if (script_block_type.equals_ignoring_case("module")) {
        m_script_type = ScriptType::Module;
    } else {
        dbgln("HTMLScriptElement: Refusing to run script because the type '{}' is not recognized.", script_block_type);
        return;
    }

    if (parser_document) {
        m_parser_document = *parser_document;
        m_non_blocking = false;
    }

    m_already_started = true;
    m_preparation_time_document = document();

    if (parser_document && parser_document.ptr() != m_preparation_time_document.ptr()) {
        dbgln("HTMLScriptElement: Refusing to run script because the parser document is not the same as the preparation time document.");
        return;
    }

    if (is_scripting_disabled()) {
        dbgln("HTMLScriptElement: Refusing to run script because scripting is disabled.");
        return;
    }

    if (m_script_type == ScriptType::Classic && has_attribute(HTML::AttributeNames::nomodule)) {
        dbgln("HTMLScriptElement: Refusing to run classic script because it has the nomodule attribute.");
        return;
    }


    if (m_script_type == ScriptType::Classic && has_attribute(HTML::AttributeNames::event) && has_attribute(HTML::AttributeNames::for_)) {
        auto for_ = attribute(HTML::AttributeNames::for_).trim_whitespace();
        auto event = attribute(HTML::AttributeNames::event).trim_whitespace();

        if (!for_.equals_ignoring_case("window")) {
            dbgln("HTMLScriptElement: Refusing to run classic script because the provided 'for' attribute is not equal to 'window'");
            return;
        }

        if (!event.equals_ignoring_case("onload") && !event.equals_ignoring_case("onload()")) {
            dbgln("HTMLScriptElement: Refusing to run classic script because the provided 'event' attribute is not equal to 'onload' or 'onload()'");
            return;
        }
    }


    if (has_attribute(HTML::AttributeNames::src)) {
        auto src = attribute(HTML::AttributeNames::src);
        if (src.is_empty()) {
            dbgln("HTMLScriptElement: Refusing to run script because the src attribute is empty.");

            dispatch_event(DOM::Event::create(HTML::EventNames::error));
            return;
        }
        m_from_an_external_file = true;

        auto url = document().complete_url(src);
        if (!url.is_valid()) {
            dbgln("HTMLScriptElement: Refusing to run script because the src URL '{}' is invalid.", url);

            dispatch_event(DOM::Event::create(HTML::EventNames::error));
            return;
        }

        if (m_script_type == ScriptType::Classic) {
            auto request = LoadRequest::create_for_url_on_page(url, document().page());

            m_script_filename = url.to_string();
            ResourceLoader::the().load_sync(
                request,
                [this, url](auto data, auto&, auto) {
                    if (data.is_null()) {
                        dbgln("HTMLScriptElement: Failed to load {}", url);
                        return;
                    }
                    m_script_source = String::copy(data);
                    script_became_ready();
                },
                [this](auto&, auto) {
                    m_failed_to_load = true;
                });
        } else {
            TODO();
        }
    } else {
        if (m_script_type == ScriptType::Classic) {
            m_script_source = source_text;
            script_became_ready();
        } else {
            TODO();
        }
    }

    if ((m_script_type == ScriptType::Classic && has_attribute(HTML::AttributeNames::src) && has_attribute(HTML::AttributeNames::defer) && is_parser_inserted() && !has_attribute(HTML::AttributeNames::async))
        || (m_script_type == ScriptType::Module && is_parser_inserted() && !has_attribute(HTML::AttributeNames::async))) {
        document().add_script_to_execute_when_parsing_has_finished({}, *this);
        when_the_script_is_ready([this] {
            m_ready_to_be_parser_executed = true;
        });
    }

    else if (m_script_type == ScriptType::Classic && has_attribute(HTML::AttributeNames::src) && is_parser_inserted() && !has_attribute(HTML::AttributeNames::async)) {
        document().set_pending_parsing_blocking_script({}, this);
        when_the_script_is_ready([this] {
            m_ready_to_be_parser_executed = true;
        });
    }

    else if ((m_script_type == ScriptType::Classic && has_attribute(HTML::AttributeNames::src) && !has_attribute(HTML::AttributeNames::async) && !m_non_blocking)
        || (m_script_type == ScriptType::Module && !has_attribute(HTML::AttributeNames::async) && !m_non_blocking)) {
        m_preparation_time_document->add_script_to_execute_as_soon_as_possible({}, *this);

    }

    else if ((m_script_type == ScriptType::Classic && has_attribute(HTML::AttributeNames::src)) || m_script_type == ScriptType::Module) {

        m_preparation_time_document->add_script_to_execute_as_soon_as_possible({}, *this);
    }

    else {
        
        execute_script();
    }
}

void HTMLScriptElement::script_became_ready()
{
    m_script_ready = true;
    if (!m_script_ready_callback)
        return;
    m_script_ready_callback();
    m_script_ready_callback = nullptr;
}

void HTMLScriptElement::when_the_script_is_ready(Function<void()> callback)
{
    if (m_script_ready) {
        callback();
        return;
    }
    m_script_ready_callback = move(callback);
}

void HTMLScriptElement::inserted()
{
    if (!is_parser_inserted()) {

        if (is_connected()) {
            prepare_script();
        }
    }
    HTMLElement::inserted();
}

}
