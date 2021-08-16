/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/ElementFactory.h>
#include <libweb/html/HTMLAnchorElement.h>
#include <libweb/html/HTMLAreaElement.h>
#include <libweb/html/HTMLAudioElement.h>
#include <libweb/html/HTMLBRElement.h>
#include <libweb/html/HTMLBaseElement.h>
#include <libweb/html/HTMLBlinkElement.h>
#include <libweb/html/HTMLBodyElement.h>
#include <libweb/html/HTMLButtonElement.h>
#include <libweb/html/HTMLCanvasElement.h>
#include <libweb/html/HTMLDListElement.h>
#include <libweb/html/HTMLDataElement.h>
#include <libweb/html/HTMLDataListElement.h>
#include <libweb/html/HTMLDetailsElement.h>
#include <libweb/html/HTMLDialogElement.h>
#include <libweb/html/HTMLDirectoryElement.h>
#include <libweb/html/HTMLDivElement.h>
#include <libweb/html/HTMLEmbedElement.h>
#include <libweb/html/HTMLFieldSetElement.h>
#include <libweb/html/HTMLFontElement.h>
#include <libweb/html/HTMLFormElement.h>
#include <libweb/html/HTMLFrameElement.h>
#include <libweb/html/HTMLFrameSetElement.h>
#include <libweb/html/HTMLHRElement.h>
#include <libweb/html/HTMLHeadElement.h>
#include <libweb/html/HTMLHeadingElement.h>
#include <libweb/html/HTMLHtmlElement.h>
#include <libweb/html/HTMLIFrameElement.h>
#include <libweb/html/HTMLImageElement.h>
#include <libweb/html/HTMLInputElement.h>
#include <libweb/html/HTMLLIElement.h>
#include <libweb/html/HTMLLabelElement.h>
#include <libweb/html/HTMLLegendElement.h>
#include <libweb/html/HTMLLinkElement.h>
#include <libweb/html/HTMLMapElement.h>
#include <libweb/html/HTMLMarqueeElement.h>
#include <libweb/html/HTMLMenuElement.h>
#include <libweb/html/HTMLMetaElement.h>
#include <libweb/html/HTMLMeterElement.h>
#include <libweb/html/HTMLModElement.h>
#include <libweb/html/HTMLOListElement.h>
#include <libweb/html/HTMLObjectElement.h>
#include <libweb/html/HTMLOptGroupElement.h>
#include <libweb/html/HTMLOptionElement.h>
#include <libweb/html/HTMLOutputElement.h>
#include <libweb/html/HTMLParagraphElement.h>
#include <libweb/html/HTMLParamElement.h>
#include <libweb/html/HTMLPictureElement.h>
#include <libweb/html/HTMLPreElement.h>
#include <libweb/html/HTMLProgressElement.h>
#include <libweb/html/HTMLQuoteElement.h>
#include <libweb/html/HTMLScriptElement.h>
#include <libweb/html/HTMLSelectElement.h>
#include <libweb/html/HTMLSlotElement.h>
#include <libweb/html/HTMLSourceElement.h>
#include <libweb/html/HTMLSpanElement.h>
#include <libweb/html/HTMLStyleElement.h>
#include <libweb/html/HTMLTableCaptionElement.h>
#include <libweb/html/HTMLTableCellElement.h>
#include <libweb/html/HTMLTableColElement.h>
#include <libweb/html/HTMLTableElement.h>
#include <libweb/html/HTMLTableRowElement.h>
#include <libweb/html/HTMLTableSectionElement.h>
#include <libweb/html/HTMLTemplateElement.h>
#include <libweb/html/HTMLTextAreaElement.h>
#include <libweb/html/HTMLTimeElement.h>
#include <libweb/html/HTMLTitleElement.h>
#include <libweb/html/HTMLTrackElement.h>
#include <libweb/html/HTMLUListElement.h>
#include <libweb/html/HTMLUnknownElement.h>
#include <libweb/html/HTMLVideoElement.h>
#include <libweb/svg/SVGPathElement.h>
#include <libweb/svg/SVGSVGElement.h>
#include <libweb/svg/TagNames.h>

namespace Web::DOM {

NonnullRefPtr<Element> create_element(Document& document, const FlyString& tag_name, const FlyString& namespace_)
{
    auto lowercase_tag_name = tag_name.to_lowercase();

    auto qualified_name = QualifiedName(tag_name, {}, namespace_);
    if (lowercase_tag_name == HTML::TagNames::a)
        return adopt_ref(*new HTML::HTMLAnchorElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::area)
        return adopt_ref(*new HTML::HTMLAreaElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::audio)
        return adopt_ref(*new HTML::HTMLAudioElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::base)
        return adopt_ref(*new HTML::HTMLBaseElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::blink)
        return adopt_ref(*new HTML::HTMLBlinkElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::body)
        return adopt_ref(*new HTML::HTMLBodyElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::br)
        return adopt_ref(*new HTML::HTMLBRElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::button)
        return adopt_ref(*new HTML::HTMLButtonElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::canvas)
        return adopt_ref(*new HTML::HTMLCanvasElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::data)
        return adopt_ref(*new HTML::HTMLDataElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::datalist)
        return adopt_ref(*new HTML::HTMLDataListElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::details)
        return adopt_ref(*new HTML::HTMLDetailsElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::dialog)
        return adopt_ref(*new HTML::HTMLDialogElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::dir)
        return adopt_ref(*new HTML::HTMLDirectoryElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::div)
        return adopt_ref(*new HTML::HTMLDivElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::dl)
        return adopt_ref(*new HTML::HTMLDListElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::embed)
        return adopt_ref(*new HTML::HTMLEmbedElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::fieldset)
        return adopt_ref(*new HTML::HTMLFieldSetElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::font)
        return adopt_ref(*new HTML::HTMLFontElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::form)
        return adopt_ref(*new HTML::HTMLFormElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::frame)
        return adopt_ref(*new HTML::HTMLFrameElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::frameset)
        return adopt_ref(*new HTML::HTMLFrameSetElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::head)
        return adopt_ref(*new HTML::HTMLHeadElement(document, move(qualified_name)));
    if (lowercase_tag_name.is_one_of(HTML::TagNames::h1, HTML::TagNames::h2, HTML::TagNames::h3, HTML::TagNames::h4, HTML::TagNames::h5, HTML::TagNames::h6))
        return adopt_ref(*new HTML::HTMLHeadingElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::hr)
        return adopt_ref(*new HTML::HTMLHRElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::html)
        return adopt_ref(*new HTML::HTMLHtmlElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::iframe)
        return adopt_ref(*new HTML::HTMLIFrameElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::img)
        return adopt_ref(*new HTML::HTMLImageElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::input)
        return adopt_ref(*new HTML::HTMLInputElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::label)
        return adopt_ref(*new HTML::HTMLLabelElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::legend)
        return adopt_ref(*new HTML::HTMLLegendElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::li)
        return adopt_ref(*new HTML::HTMLLIElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::link)
        return adopt_ref(*new HTML::HTMLLinkElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::map)
        return adopt_ref(*new HTML::HTMLMapElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::marquee)
        return adopt_ref(*new HTML::HTMLMarqueeElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::menu)
        return adopt_ref(*new HTML::HTMLMenuElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::meta)
        return adopt_ref(*new HTML::HTMLMetaElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::meter)
        return adopt_ref(*new HTML::HTMLMeterElement(document, move(qualified_name)));
    if (lowercase_tag_name.is_one_of(HTML::TagNames::ins, HTML::TagNames::del))
        return adopt_ref(*new HTML::HTMLModElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::object)
        return adopt_ref(*new HTML::HTMLObjectElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::ol)
        return adopt_ref(*new HTML::HTMLOListElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::optgroup)
        return adopt_ref(*new HTML::HTMLOptGroupElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::option)
        return adopt_ref(*new HTML::HTMLOptionElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::output)
        return adopt_ref(*new HTML::HTMLOutputElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::p)
        return adopt_ref(*new HTML::HTMLParagraphElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::param)
        return adopt_ref(*new HTML::HTMLParamElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::picture)
        return adopt_ref(*new HTML::HTMLPictureElement(document, move(qualified_name)));

    if (lowercase_tag_name.is_one_of(HTML::TagNames::pre, HTML::TagNames::listing, HTML::TagNames::xmp))
        return adopt_ref(*new HTML::HTMLPreElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::progress)
        return adopt_ref(*new HTML::HTMLProgressElement(document, move(qualified_name)));
    if (lowercase_tag_name.is_one_of(HTML::TagNames::blockquote, HTML::TagNames::q))
        return adopt_ref(*new HTML::HTMLQuoteElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::script)
        return adopt_ref(*new HTML::HTMLScriptElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::select)
        return adopt_ref(*new HTML::HTMLSelectElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::slot)
        return adopt_ref(*new HTML::HTMLSlotElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::source)
        return adopt_ref(*new HTML::HTMLSourceElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::span)
        return adopt_ref(*new HTML::HTMLSpanElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::style)
        return adopt_ref(*new HTML::HTMLStyleElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::caption)
        return adopt_ref(*new HTML::HTMLTableCaptionElement(document, move(qualified_name)));
    if (lowercase_tag_name.is_one_of(Web::HTML::TagNames::td, Web::HTML::TagNames::th))
        return adopt_ref(*new HTML::HTMLTableCellElement(document, move(qualified_name)));
    if (lowercase_tag_name.is_one_of(HTML::TagNames::colgroup, HTML::TagNames::col))
        return adopt_ref(*new HTML::HTMLTableColElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::table)
        return adopt_ref(*new HTML::HTMLTableElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::tr)
        return adopt_ref(*new HTML::HTMLTableRowElement(document, move(qualified_name)));
    if (lowercase_tag_name.is_one_of(HTML::TagNames::tbody, HTML::TagNames::thead, HTML::TagNames::tfoot))
        return adopt_ref(*new HTML::HTMLTableSectionElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::template_)
        return adopt_ref(*new HTML::HTMLTemplateElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::textarea)
        return adopt_ref(*new HTML::HTMLTextAreaElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::time)
        return adopt_ref(*new HTML::HTMLTimeElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::title)
        return adopt_ref(*new HTML::HTMLTitleElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::track)
        return adopt_ref(*new HTML::HTMLTrackElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::ul)
        return adopt_ref(*new HTML::HTMLUListElement(document, move(qualified_name)));
    if (lowercase_tag_name == HTML::TagNames::video)
        return adopt_ref(*new HTML::HTMLVideoElement(document, move(qualified_name)));
    if (lowercase_tag_name.is_one_of(
            HTML::TagNames::article, HTML::TagNames::section, HTML::TagNames::nav, HTML::TagNames::aside, HTML::TagNames::hgroup, HTML::TagNames::header, HTML::TagNames::footer, HTML::TagNames::address, HTML::TagNames::dt, HTML::TagNames::dd, HTML::TagNames::figure, HTML::TagNames::figcaption, HTML::TagNames::main, HTML::TagNames::em, HTML::TagNames::strong, HTML::TagNames::small, HTML::TagNames::s, HTML::TagNames::cite, HTML::TagNames::dfn, HTML::TagNames::abbr, HTML::TagNames::ruby, HTML::TagNames::rt, HTML::TagNames::rp, HTML::TagNames::code, HTML::TagNames::var, HTML::TagNames::samp, HTML::TagNames::kbd, HTML::TagNames::sub, HTML::TagNames::sup, HTML::TagNames::i, HTML::TagNames::b, HTML::TagNames::u, HTML::TagNames::mark, HTML::TagNames::bdi, HTML::TagNames::bdo, HTML::TagNames::wbr, HTML::TagNames::summary, HTML::TagNames::noscript,
        
            HTML::TagNames::acronym, HTML::TagNames::basefont, HTML::TagNames::big, HTML::TagNames::center, HTML::TagNames::nobr, HTML::TagNames::noembed, HTML::TagNames::noframes, HTML::TagNames::plaintext, HTML::TagNames::rb, HTML::TagNames::rtc, HTML::TagNames::strike, HTML::TagNames::tt))
        return adopt_ref(*new HTML::HTMLElement(document, move(qualified_name)));
    if (lowercase_tag_name == SVG::TagNames::svg)
        return adopt_ref(*new SVG::SVGSVGElement(document, move(qualified_name)));
    if (lowercase_tag_name == SVG::TagNames::path)
        return adopt_ref(*new SVG::SVGPathElement(document, move(qualified_name)));


    return adopt_ref(*new HTML::HTMLUnknownElement(document, move(qualified_name)));
}

}
