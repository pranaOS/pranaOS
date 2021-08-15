/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/bindings/CharacterDataWrapper.h>
#include <libweb/bindings/CommentWrapper.h>
#include <libweb/bindings/DocumentFragmentWrapper.h>
#include <libweb/bindings/DocumentTypeWrapper.h>
#include <libweb/bindings/DocumentWrapper.h>
#include <libweb/bindings/HTMLAnchorElementWrapper.h>
#include <libweb/bindings/HTMLAreaElementWrapper.h>
#include <libweb/bindings/HTMLAudioElementWrapper.h>
#include <libweb/bindings/HTMLBRElementWrapper.h>
#include <libweb/bindings/HTMLBaseElementWrapper.h>
#include <libweb/bindings/HTMLBodyElementWrapper.h>
#include <libweb/bindings/HTMLButtonElementWrapper.h>
#include <libweb/bindings/HTMLCanvasElementWrapper.h>
#include <libweb/bindings/HTMLDListElementWrapper.h>
#include <libweb/bindings/HTMLDataElementWrapper.h>
#include <libweb/bindings/HTMLDataListElementWrapper.h>
#include <libweb/bindings/HTMLDetailsElementWrapper.h>
#include <libweb/bindings/HTMLDialogElementWrapper.h>
#include <libweb/bindings/HTMLDirectoryElementWrapper.h>
#include <libweb/bindings/HTMLDivElementWrapper.h>
#include <libweb/bindings/HTMLElementWrapper.h>
#include <libweb/bindings/HTMLEmbedElementWrapper.h>
#include <libweb/bindings/HTMLFieldSetElementWrapper.h>
#include <libweb/bindings/HTMLFontElementWrapper.h>
#include <libweb/bindings/HTMLFormElementWrapper.h>
#include <libweb/bindings/HTMLFrameElementWrapper.h>
#include <libweb/bindings/HTMLFrameSetElementWrapper.h>
#include <libweb/bindings/HTMLHRElementWrapper.h>
#include <libweb/bindings/HTMLHeadElementWrapper.h>
#include <libweb/bindings/HTMLHeadingElementWrapper.h>
#include <libweb/bindings/HTMLHtmlElementWrapper.h>
#include <libweb/bindings/HTMLIFrameElementWrapper.h>
#include <libweb/bindings/HTMLImageElementWrapper.h>
#include <libweb/bindings/HTMLInputElementWrapper.h>
#include <libweb/bindings/HTMLLIElementWrapper.h>
#include <libweb/bindings/HTMLLabelElementWrapper.h>
#include <libweb/bindings/HTMLLegendElementWrapper.h>
#include <libweb/bindings/HTMLLinkElementWrapper.h>
#include <libweb/bindings/HTMLMapElementWrapper.h>
#include <libweb/bindings/HTMLMarqueeElementWrapper.h>
#include <libweb/bindings/HTMLMenuElementWrapper.h>
#include <libweb/bindings/HTMLMetaElementWrapper.h>
#include <libweb/bindings/HTMLMeterElementWrapper.h>
#include <libweb/bindings/HTMLModElementWrapper.h>
#include <libweb/bindings/HTMLOListElementWrapper.h>
#include <libweb/bindings/HTMLObjectElementWrapper.h>
#include <libweb/bindings/HTMLOptGroupElementWrapper.h>
#include <libweb/bindings/HTMLOptionElementWrapper.h>
#include <libweb/bindings/HTMLOutputElementWrapper.h>
#include <libweb/bindings/HTMLParagraphElementWrapper.h>
#include <libweb/bindings/HTMLParamElementWrapper.h>
#include <libweb/bindings/HTMLPictureElementWrapper.h>
#include <libweb/bindings/HTMLPreElementWrapper.h>
#include <libweb/bindings/HTMLProgressElementWrapper.h>
#include <libweb/bindings/HTMLQuoteElementWrapper.h>
#include <libweb/bindings/HTMLScriptElementWrapper.h>
#include <libweb/bindings/HTMLSelectElementWrapper.h>
#include <libweb/bindings/HTMLSlotElementWrapper.h>
#include <libweb/bindings/HTMLSourceElementWrapper.h>
#include <libweb/bindings/HTMLSpanElementWrapper.h>
#include <libweb/bindings/HTMLStyleElementWrapper.h>
#include <libweb/bindings/HTMLTableCaptionElementWrapper.h>
#include <libweb/bindings/HTMLTableCellElementWrapper.h>
#include <libweb/bindings/HTMLTableColElementWrapper.h>
#include <libweb/bindings/HTMLTableElementWrapper.h>
#include <libweb/bindings/HTMLTableRowElementWrapper.h>
#include <libweb/bindings/HTMLTableSectionElementWrapper.h>
#include <libweb/bindings/HTMLTemplateElementWrapper.h>
#include <libweb/bindings/HTMLTextAreaElementWrapper.h>
#include <libweb/bindings/HTMLTimeElementWrapper.h>
#include <libweb/bindings/HTMLTitleElementWrapper.h>
#include <libweb/bindings/HTMLTrackElementWrapper.h>
#include <libweb/bindings/HTMLUListElementWrapper.h>
#include <libweb/bindings/HTMLUnknownElementWrapper.h>
#include <libweb/bindings/HTMLVideoElementWrapper.h>
#include <libweb/bindings/NodeWrapper.h>
#include <libweb/bindings/NodeWrapperFactory.h>
#include <libweb/bindings/SVGPathElementWrapper.h>
#include <libweb/bindings/SVGSVGElementWrapper.h>
#include <libweb/bindings/TextWrapper.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Node.h>
#include <libweb/html/HTMLAnchorElement.h>
#include <libweb/html/HTMLAreaElement.h>
#include <libweb/html/HTMLAudioElement.h>
#include <libweb/html/HTMLBRElement.h>
#include <libweb/html/HTMLBaseElement.h>
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

namespace Web::Bindings {

NodeWrapper* wrap(JS::GlobalObject& global_object, DOM::Node& node)
{
    if (is<DOM::Document>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<DOM::Document>(node)));
    if (is<DOM::DocumentType>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<DOM::DocumentType>(node)));
    if (is<HTML::HTMLAnchorElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLAnchorElement>(node)));
    if (is<HTML::HTMLAreaElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLAreaElement>(node)));
    if (is<HTML::HTMLAudioElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLAudioElement>(node)));
    if (is<HTML::HTMLBaseElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLBaseElement>(node)));
    if (is<HTML::HTMLBodyElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLBodyElement>(node)));
    if (is<HTML::HTMLBRElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLBRElement>(node)));
    if (is<HTML::HTMLButtonElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLButtonElement>(node)));
    if (is<HTML::HTMLCanvasElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLCanvasElement>(node)));
    if (is<HTML::HTMLDataElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLDataElement>(node)));
    if (is<HTML::HTMLDataListElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLDataListElement>(node)));
    if (is<HTML::HTMLDetailsElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLDetailsElement>(node)));
    if (is<HTML::HTMLDialogElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLDialogElement>(node)));
    if (is<HTML::HTMLDirectoryElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLDirectoryElement>(node)));
    if (is<HTML::HTMLDivElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLDivElement>(node)));
    if (is<HTML::HTMLDListElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLDListElement>(node)));
    if (is<HTML::HTMLEmbedElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLEmbedElement>(node)));
    if (is<HTML::HTMLFieldSetElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLFieldSetElement>(node)));
    if (is<HTML::HTMLFontElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLFontElement>(node)));
    if (is<HTML::HTMLFormElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLFormElement>(node)));
    if (is<HTML::HTMLFrameElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLFrameElement>(node)));
    if (is<HTML::HTMLFrameSetElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLFrameSetElement>(node)));
    if (is<HTML::HTMLHeadElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLHeadElement>(node)));
    if (is<HTML::HTMLHeadingElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLHeadingElement>(node)));
    if (is<HTML::HTMLHRElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLHRElement>(node)));
    if (is<HTML::HTMLHtmlElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLHtmlElement>(node)));
    if (is<HTML::HTMLIFrameElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLIFrameElement>(node)));
    if (is<HTML::HTMLImageElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLImageElement>(node)));
    if (is<HTML::HTMLInputElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLInputElement>(node)));
    if (is<HTML::HTMLLabelElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLLabelElement>(node)));
    if (is<HTML::HTMLLegendElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLLegendElement>(node)));
    if (is<HTML::HTMLLIElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLLIElement>(node)));
    if (is<HTML::HTMLLinkElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLLinkElement>(node)));
    if (is<HTML::HTMLMapElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLMapElement>(node)));
    if (is<HTML::HTMLMarqueeElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLMarqueeElement>(node)));
    if (is<HTML::HTMLMenuElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLMenuElement>(node)));
    if (is<HTML::HTMLMetaElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLMetaElement>(node)));
    if (is<HTML::HTMLMeterElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLMeterElement>(node)));
    if (is<HTML::HTMLModElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLModElement>(node)));
    if (is<HTML::HTMLObjectElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLObjectElement>(node)));
    if (is<HTML::HTMLOListElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLOListElement>(node)));
    if (is<HTML::HTMLOptGroupElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLOptGroupElement>(node)));
    if (is<HTML::HTMLOptionElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLOptionElement>(node)));
    if (is<HTML::HTMLOutputElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLOutputElement>(node)));
    if (is<HTML::HTMLParagraphElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLParagraphElement>(node)));
    if (is<HTML::HTMLParamElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLParamElement>(node)));
    if (is<HTML::HTMLPictureElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLPictureElement>(node)));
    if (is<HTML::HTMLPreElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLPreElement>(node)));
    if (is<HTML::HTMLProgressElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLProgressElement>(node)));
    if (is<HTML::HTMLQuoteElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLQuoteElement>(node)));
    if (is<HTML::HTMLScriptElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLScriptElement>(node)));
    if (is<HTML::HTMLSelectElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLSelectElement>(node)));
    if (is<HTML::HTMLSlotElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLSlotElement>(node)));
    if (is<HTML::HTMLSourceElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLSourceElement>(node)));
    if (is<HTML::HTMLSpanElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLSpanElement>(node)));
    if (is<HTML::HTMLStyleElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLStyleElement>(node)));
    if (is<HTML::HTMLTableCaptionElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTableCaptionElement>(node)));
    if (is<HTML::HTMLTableCellElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTableCellElement>(node)));
    if (is<HTML::HTMLTableColElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTableColElement>(node)));
    if (is<HTML::HTMLTableElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTableElement>(node)));
    if (is<HTML::HTMLTableRowElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTableRowElement>(node)));
    if (is<HTML::HTMLTableSectionElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTableSectionElement>(node)));
    if (is<HTML::HTMLTemplateElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTemplateElement>(node)));
    if (is<HTML::HTMLTextAreaElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTextAreaElement>(node)));
    if (is<HTML::HTMLTimeElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTimeElement>(node)));
    if (is<HTML::HTMLTitleElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTitleElement>(node)));
    if (is<HTML::HTMLTrackElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLTrackElement>(node)));
    if (is<HTML::HTMLUListElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLUListElement>(node)));
    if (is<HTML::HTMLUnknownElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLUnknownElement>(node)));
    if (is<HTML::HTMLVideoElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLVideoElement>(node)));
    if (is<HTML::HTMLElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<HTML::HTMLElement>(node)));
    if (is<SVG::SVGSVGElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<SVG::SVGSVGElement>(node)));
    if (is<SVG::SVGPathElement>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<SVG::SVGPathElement>(node)));
    if (is<DOM::Element>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<DOM::Element>(node)));
    if (is<DOM::DocumentFragment>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<DOM::DocumentFragment>(node)));
    if (is<DOM::Comment>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<DOM::Comment>(node)));
    if (is<DOM::Text>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<DOM::Text>(node)));
    if (is<DOM::CharacterData>(node))
        return static_cast<NodeWrapper*>(wrap_impl(global_object, verify_cast<DOM::CharacterData>(node)));
    return static_cast<NodeWrapper*>(wrap_impl(global_object, node));
}

}
