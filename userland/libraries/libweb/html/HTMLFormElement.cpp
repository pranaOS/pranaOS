/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/StringBuilder.h>
#include <libweb/html/EventNames.h>
#include <libweb/html/HTMLFormElement.h>
#include <libweb/html/HTMLInputElement.h>
#include <libweb/html/SubmitEvent.h>
#include <libweb/InProcessWebView.h>
#include <libweb/page/BrowsingContext.h>
#include <libweb/URLEncoder.h>

namespace Web::HTML {

HTMLFormElement::HTMLFormElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLFormElement::~HTMLFormElement()
{
}

void HTMLFormElement::submit_form(RefPtr<HTMLElement> submitter, bool from_submit_binding)
{
    if (cannot_navigate())
        return;

    if (action().is_null()) {
        dbgln("Unsupported form action ''");
        return;
    }

    auto effective_method = method().to_lowercase();

    if (effective_method == "dialog") {
        dbgln("Failed to submit form: Unsupported form method '{}'", method());
        return;
    }

    if (effective_method != "get" && effective_method != "post") {
        effective_method = "get";
    }

    if (!from_submit_binding) {
        if (m_firing_submission_events)
            return;

        m_firing_submission_events = true;

        RefPtr<HTMLElement> submitter_button;

        if (submitter != this)
            submitter_button = submitter;

        auto submit_event = SubmitEvent::create(EventNames::submit, submitter_button);
        submit_event->set_bubbles(true);
        submit_event->set_cancelable(true);
        bool continue_ = dispatch_event(submit_event);

        m_firing_submission_events = false;

        if (!continue_)
            return;

        if (cannot_navigate())
            return;
    }

    URL url(document().complete_url(action()));

    if (!url.is_valid()) {
        dbgln("Failed to submit form: Invalid URL: {}", action());
        return;
    }

    if (url.protocol() == "file") {
        if (document().url().protocol() != "file") {
            dbgln("Failed to submit form: Security violation: {} may not submit to {}", document().url(), url);
            return;
        }
        if (effective_method != "get") {
            dbgln("Failed to submit form: Unsupported form method '{}' for URL: {}", method(), url);
            return;
        }
    } else if (url.protocol() != "http" && url.protocol() != "https") {
        dbgln("Failed to submit form: Unsupported protocol for URL: {}", url);
        return;
    }

    Vector<URLQueryParam> parameters;

    for_each_in_inclusive_subtree_of_type<HTMLInputElement>([&](auto& input) {
        if (!input.name().is_null() && (input.type() != "submit" || &input == submitter))
            parameters.append({ input.name(), input.value() });
        return IterationDecision::Continue;
    });

    if (effective_method == "get") {
        url.set_query(urlencode(parameters, URL::PercentEncodeSet::ApplicationXWWWFormUrlencoded));
    }

    LoadRequest request;
    request.set_url(url);

    if (effective_method == "post") {
        auto body = urlencode(parameters, URL::PercentEncodeSet::ApplicationXWWWFormUrlencoded).to_byte_buffer();
        request.set_method("POST");
        request.set_header("Content-Type", "application/x-www-form-urlencoded");
        request.set_header("Content-Length", String::number(body.size()));
        request.set_body(body);
    }

    if (auto* page = document().page())
        page->load(request);
}

void HTMLFormElement::submit()
{
    submit_form(this, true);
}

void HTMLFormElement::add_associated_element(Badge<FormAssociatedElement>, HTMLElement& element)
{
    m_associated_elements.append(element);
}

void HTMLFormElement::remove_associated_element(Badge<FormAssociatedElement>, HTMLElement& element)
{
    m_associated_elements.remove_first_matching([&](auto& entry) { return entry.ptr() == &element; });
}

}
