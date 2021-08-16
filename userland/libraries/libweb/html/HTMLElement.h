/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/dom/Element.h>
#include <libweb/html/EventNames.h>
#include <libweb/html/GlobalEventHandlers.h>

namespace Web::HTML {

class HTMLElement
    : public DOM::Element
    , public HTML::GlobalEventHandlers {
public:
    using WrapperType = Bindings::HTMLElementWrapper;

    HTMLElement(DOM::Document&, QualifiedName);
    virtual ~HTMLElement() override;

    String title() const { return attribute(HTML::AttributeNames::title); }

    virtual bool is_editable() const final;
    String content_editable() const;
    DOM::ExceptionOr<void> set_content_editable(const String&);

    String inner_text();
    void set_inner_text(StringView);

    unsigned offset_top() const;
    unsigned offset_left() const;

    bool cannot_navigate() const;

protected:
    virtual void parse_attribute(const FlyString& name, const String& value) override;

private:

    virtual EventTarget& global_event_handlers_to_event_target() override { return *this; }

    enum class ContentEditableState {
        True,
        False,
        Inherit,
    };
    ContentEditableState content_editable_state() const;
};

}
