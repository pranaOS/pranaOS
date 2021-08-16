set(SOURCES
    bindings/CSSStyleDeclarationWrapperCustom.cpp
    bindings/EventListenerWrapper.cpp
    bindings/EventTargetWrapperFactory.cpp
    bindings/EventWrapperFactory.cpp
    bindings/HTMLCollectionWrapperCustom.cpp
    bindings/ImageConstructor.cpp
    bindings/LocationObject.cpp
    bindings/MainThreadVM.cpp
    bindings/NavigatorObject.cpp
    bindings/NodeWrapperFactory.cpp
    bindings/ScriptExecutionContext.cpp
    bindings/WindowObject.cpp
    bindings/Wrappable.cpp
    css/CSSImportRule.cpp
    css/CSSRule.cpp
    css/CSSStyleDeclaration.cpp
    css/CSSStyleRule.cpp
    css/CSSStyleSheet.cpp
    css/DefaultStyleSheetSource.cpp
    css/Length.cpp
    css/parser/Parser.cpp
    css/parser/StyleRules.cpp
    css/parser/Token.cpp
    css/parser/Tokenizer.cpp
    css/PropertyID.cpp
    css/PropertyID.h
    css/QuirksModeStyleSheetSource.cpp
    css/Screen.cpp
    css/Selector.cpp
    css/SelectorEngine.cpp
    css/StyleInvalidator.cpp
    css/StyleProperties.cpp
    css/StyleResolver.cpp
    css/StyleSheet.cpp
    css/StyleSheetList.cpp
    css/StyleValue.cpp
    css/ValueID.cpp
    css/ValueID.h
    cookie/ParsedCookie.cpp
    dom/CharacterData.cpp
    dom/CharacterData.idl
    dom/Comment.cpp
    dom/DOMImplementation.cpp
    dom/Document.cpp
    dom/DocumentFragment.cpp
    dom/DocumentType.cpp
    dom/Element.cpp
    dom/ElementFactory.cpp
    dom/Event.cpp
    dom/EventDispatcher.cpp
    dom/EventListener.cpp
    dom/EventTarget.cpp
    dom/HTMLCollection.cpp
    dom/Node.cpp
    dom/ParentNode.cpp
    dom/Position.cpp
    dom/ProcessingInstruction.cpp
    dom/Range.cpp
    dom/ShadowRoot.cpp
    dom/Text.cpp
    dom/Text.idl
    dom/Timer.cpp
    dom/Window.cpp
    DOMTreeModel.cpp
    DOMTreeJSONModel.cpp
    Dump.cpp
    FontCache.cpp
    html/AttributeNames.cpp
    html/BrowsingContextContainer.cpp
    html/CanvasRenderingContext2D.cpp
    html/DOMParser.cpp
    html/EventNames.cpp
    html/FormAssociatedElement.cpp
    html/GlobalEventHandlers.cpp
    html/HTMLAnchorElement.cpp
    html/HTMLAreaElement.cpp
    html/HTMLAudioElement.cpp
    html/HTMLBRElement.cpp
    html/HTMLBaseElement.cpp
    html/HTMLBlinkElement.cpp
    html/HTMLBodyElement.cpp
    html/HTMLButtonElement.cpp
    html/HTMLCanvasElement.cpp
    html/HTMLDListElement.cpp
    html/HTMLDataElement.cpp
    html/HTMLDataListElement.cpp
    html/HTMLDetailsElement.cpp
    html/HTMLDialogElement.cpp
    html/HTMLDirectoryElement.cpp
    html/HTMLDivElement.cpp
    html/HTMLElement.cpp
    html/HTMLEmbedElement.cpp
    html/HTMLFieldSetElement.cpp
    html/HTMLFontElement.cpp
    html/HTMLFormElement.cpp
    html/HTMLFrameElement.cpp
    html/HTMLFrameSetElement.cpp
    html/HTMLHRElement.cpp
    html/HTMLHeadElement.cpp
    html/HTMLHeadingElement.cpp
    html/HTMLHtmlElement.cpp
    html/HTMLIFrameElement.cpp
    html/HTMLImageElement.cpp
    html/HTMLInputElement.cpp
    html/HTMLLIElement.cpp
    html/HTMLLabelElement.cpp
    html/HTMLLegendElement.cpp
    html/HTMLLinkElement.cpp
    html/HTMLMapElement.cpp
    html/HTMLMarqueeElement.cpp
    html/HTMLMediaElement.cpp
    html/HTMLMenuElement.cpp
    html/HTMLMetaElement.cpp
    html/HTMLMeterElement.cpp
    html/HTMLModElement.cpp
    html/HTMLOListElement.cpp
    html/HTMLObjectElement.cpp
    html/HTMLOptGroupElement.cpp
    html/HTMLOptionElement.cpp
    html/HTMLOutputElement.cpp
    html/HTMLParagraphElement.cpp
    html/HTMLParamElement.cpp
    html/HTMLPictureElement.cpp
    html/HTMLPreElement.cpp
    html/HTMLProgressElement.cpp
    html/HTMLQuoteElement.cpp
    html/HTMLScriptElement.cpp
    html/HTMLSelectElement.cpp
    html/HTMLSlotElement.cpp
    html/HTMLSourceElement.cpp
    html/HTMLSpanElement.cpp
    html/HTMLStyleElement.cpp
    html/HTMLTableCaptionElement.cpp
    html/HTMLTableCellElement.cpp
    html/HTMLTableColElement.cpp
    html/HTMLTableElement.cpp
    html/HTMLTableRowElement.cpp
    html/HTMLTableSectionElement.cpp
    html/HTMLTemplateElement.cpp
    html/HTMLTextAreaElement.cpp
    html/HTMLTimeElement.cpp
    html/HTMLTitleElement.cpp
    html/HTMLTrackElement.cpp
    html/HTMLUListElement.cpp
    html/HTMLUnknownElement.cpp
    html/HTMLVideoElement.cpp
    html/ImageData.cpp
    html/parser/Entities.cpp
    html/parser/HTMLDocumentParser.cpp
    html/parser/HTMLEncodingDetection.cpp
    html/parser/HTMLToken.cpp
    html/parser/HTMLTokenizer.cpp
    html/parser/ListOfActiveFormattingElements.cpp
    html/parser/StackOfOpenElements.cpp
    html/SubmitEvent.cpp
    html/SyntaxHighlighter/SyntaxHighlighter.cpp
    html/TagNames.cpp
    html/WebSocket.cpp
    highresolutiontime/Performance.cpp
    InProcessWebView.cpp
    layout/BlockBox.cpp
    layout/BlockFormattingContext.cpp
    layout/Box.cpp
    layout/BoxModelMetrics.cpp
    layout/BreakNode.cpp
    layout/ButtonBox.cpp
    layout/CanvasBox.cpp
    layout/CheckBox.cpp
    layout/FlexFormattingContext.cpp
    layout/FormattingContext.cpp
    layout/FrameBox.cpp
    layout/ImageBox.cpp
    layout/InitialContainingBlockBox.cpp
    layout/InlineFormattingContext.cpp
    layout/InlineNode.cpp
    layout/Label.cpp
    layout/LayoutPosition.cpp
    layout/LineBox.cpp
    layout/LineBoxFragment.cpp
    layout/ListItemBox.cpp
    layout/ListItemMarkerBox.cpp
    layout/Node.cpp
    Layout/RadioButton.cpp
    layout/ReplacedBox.cpp
    layout/SVGBox.cpp
    layout/SVGGraphicsBox.cpp
    layout/SVGPathBox.cpp
    layout/SVGSVGBox.cpp
    layout/TableBox.cpp
    layout/TableCellBox.cpp
    layout/TableFormattingContext.cpp
    layout/TableRowBox.cpp
    layout/TableRowGroupBox.cpp
    layout/TextNode.cpp
    layout/TreeBuilder.cpp
    LayoutTreeModel.cpp
    loader/CSSLoader.cpp
    loader/ContentFilter.cpp
    loader/FrameLoader.cpp
    loader/ImageLoader.cpp
    loader/ImageResource.cpp
    loader/LoadRequest.cpp
    loader/Resource.cpp
    loader/ResourceLoader.cpp
    Namespace.cpp
    navigationtiming/PerformanceTiming.cpp
    OutOfProcessWebView.cpp
    page/BrowsingContext.cpp
    page/EditEventHandler.cpp
    page/EventHandler.cpp
    page/Page.cpp
    painting/BorderPainting.cpp
    painting/StackingContext.cpp
    svg/SVGElement.cpp
    svg/SVGGeometryElement.cpp
    svg/SVGGraphicsElement.cpp
    svg/SVGPathElement.cpp
    svg/SVGSVGElement.cpp
    svg/TagNames.cpp
    StylePropertiesModel.cpp
    uievents/EventNames.cpp
    uievents/MouseEvent.cpp
    URLEncoder.cpp
    webassembly/WebAssemblyInstanceConstructor.cpp
    webassembly/WebAssemblyInstanceObject.cpp
    webassembly/WebAssemblyInstanceObjectPrototype.cpp
    webassembly/WebAssemblyMemoryConstructor.cpp
    webassembly/WebAssemblyMemoryPrototype.cpp
    webassembly/WebAssemblyModuleConstructor.cpp
    webassembly/WebAssemblyModuleObject.cpp
    webassembly/WebAssemblyObject.cpp
    WebContentClient.cpp
    xhrEventNames.cpp
    xhrXMLHttpRequest.cpp
)

set(GENERATED_SOURCES
    ../../services/requestserver/RequestClientEndpoint.h
    ../../services/requestserver/RequestServerEndpoint.h
    ../../services/webcontent/WebContentClientEndpoint.h
    ../../services/webcontent/WebContentServerEndpoint.h
)

set_property(GLOBAL PROPERTY wrapper_sources)
function(add_wrapper_sources)
    get_property(tmp GLOBAL PROPERTY wrapper_sources)
    foreach(arg ${ARGV})
        set(tmp ${tmp}
            ${arg}
        )
    endforeach()
    set_property(GLOBAL PROPERTY wrapper_sources "${tmp}")
endfunction(add_wrapper_sources)

function(libweb_js_wrapper class)
    get_filename_component(basename ${class} NAME)
    add_wrapper_sources(bindings/${basename}Wrapper.cpp Bindings/${basename}Wrapper.h)
    add_wrapper_sources(bindings/${basename}Constructor.cpp Bindings/${basename}Constructor.h)
    add_wrapper_sources(bindings/${basename}Prototype.cpp Bindings/${basename}Prototype.h)
    add_custom_command(
        OUTPUT bindings/${basename}Wrapper.h
        COMMAND ${write_if_different} bindings/${basename}Wrapper.h codegenerators/WrapperGenerator --header ${CMAKE_CURRENT_SOURCE_DIR}/${class}.idl
        VERBATIM
        DEPENDS WrapperGenerator
        MAIN_DEPENDENCY ${class}.idl
    )
    add_custom_command(
        OUTPUT bindings/${basename}Wrapper.cpp
        COMMAND ${write_if_different} bindings/${basename}Wrapper.cpp codegenerators/WrapperGenerator --implementation ${CMAKE_CURRENT_SOURCE_DIR}/${class}.idl
        VERBATIM
        DEPENDS WrapperGenerator
        MAIN_DEPENDENCY ${class}.idl
    )
    add_custom_command(
        OUTPUT Bindings/${basename}Constructor.h
        COMMAND ${write_if_different} bindings/${basename}Constructor.h codegenerators/WrapperGenerator --constructor-header ${CMAKE_CURRENT_SOURCE_DIR}/${class}.idl
        VERBATIM
        DEPENDS WrapperGenerator
        MAIN_DEPENDENCY ${class}.idl
    )
    add_custom_command(
        OUTPUT bindings/${basename}Constructor.cpp
        COMMAND ${write_if_different} bindings/${basename}Constructor.cpp CodeGenerators/WrapperGenerator --constructor-implementation ${CMAKE_CURRENT_SOURCE_DIR}/${class}.idl
        VERBATIM
        DEPENDS WrapperGenerator
        MAIN_DEPENDENCY ${class}.idl
    )
    add_custom_command(
        OUTPUT bindings/${basename}Prototype.h
        COMMAND ${write_if_different} bindings/${basename}Prototype.h CodeGenerators/WrapperGenerator --prototype-header ${CMAKE_CURRENT_SOURCE_DIR}/${class}.idl
        VERBATIM
        DEPENDS WrapperGenerator
        MAIN_DEPENDENCY ${class}.idl
    )
    add_custom_command(
        OUTPUT bindings/${basename}Prototype.cpp
        COMMAND ${write_if_different} bindings/${basename}Prototype.cpp CodeGenerators/WrapperGenerator --prototype-implementation ${CMAKE_CURRENT_SOURCE_DIR}/${class}.idl
        VERBATIM
        DEPENDS WrapperGenerator
        MAIN_DEPENDENCY ${class}.idl
    )
    add_custom_target(generate_${basename}Wrapper.h DEPENDS Bindings/${class}Wrapper.h)
    add_custom_target(generate_${basename}Wrapper.cpp DEPENDS Bindings/${class}Wrapper.cpp)
    add_custom_target(generate_${basename}Constructor.h DEPENDS Bindings/${class}Constructor.h)
    add_custom_target(generate_${basename}Constructor.cpp DEPENDS Bindings/${class}Constructor.cpp)
    add_custom_target(generate_${basename}Prototype.h DEPENDS Bindings/${class}Prototype.h)
    add_custom_target(generate_${basename}Prototype.cpp DEPENDS Bindings/${class}Prototype.cpp)
endfunction()

libweb_js_wrapper(css/CSSStyleDeclaration)
libweb_js_wrapper(css/CSSStyleSheet)
libweb_js_wrapper(css/Screen)
libweb_js_wrapper(css/StyleSheet)
libweb_js_wrapper(css/StyleSheetList)
libweb_js_wrapper(dom/CharacterData)
libweb_js_wrapper(dom/Comment)
libweb_js_wrapper(dom/Document)
libweb_js_wrapper(dom/DocumentFragment)
libweb_js_wrapper(dom/DocumentType)
libweb_js_wrapper(dom/DOMException)
libweb_js_wrapper(dom/DOMImplementation)
libweb_js_wrapper(dom/Element)
libweb_js_wrapper(dom/Event)
libweb_js_wrapper(dom/EventTarget)
libweb_js_wrapper(dom/HTMLCollection)
libweb_js_wrapper(dom/ProcessingInstruction)
libweb_js_wrapper(dom/ShadowRoot)
libweb_js_wrapper(dom/Node)
libweb_js_wrapper(dom/Range)
libweb_js_wrapper(dom/Text)
libweb_js_wrapper(html/CanvasRenderingContext2D)
libweb_js_wrapper(html/CloseEvent)
libweb_js_wrapper(html/DOMParser)
libweb_js_wrapper(html/HTMLAnchorElement)
libweb_js_wrapper(html/HTMLAreaElement)
libweb_js_wrapper(html/HTMLAudioElement)
libweb_js_wrapper(html/HTMLBaseElement)
libweb_js_wrapper(html/HTMLBodyElement)
libweb_js_wrapper(html/HTMLBRElement)
libweb_js_wrapper(html/HTMLButtonElement)
libweb_js_wrapper(html/HTMLCanvasElement)
libweb_js_wrapper(html/HTMLDataElement)
libweb_js_wrapper(html/HTMLDataListElement)
libweb_js_wrapper(html/HTMLDetailsElement)
libweb_js_wrapper(html/HTMLDialogElement)
libweb_js_wrapper(html/HTMLDirectoryElement)
libweb_js_wrapper(html/HTMLDivElement)
libweb_js_wrapper(html/HTMLDListElement)
libweb_js_wrapper(html/HTMLElement)
libweb_js_wrapper(html/HTMLEmbedElement)
libweb_js_wrapper(html/HTMLFieldSetElement)
libweb_js_wrapper(html/HTMLFontElement)
libweb_js_wrapper(html/HTMLFormElement)
libweb_js_wrapper(html/HTMLFrameElement)
libweb_js_wrapper(html/HTMLFrameSetElement)
libweb_js_wrapper(html/HTMLHeadElement)
libweb_js_wrapper(html/HTMLHeadingElement)
libweb_js_wrapper(html/HTMLHRElement)
libweb_js_wrapper(html/HTMLHtmlElement)
libweb_js_wrapper(html/HTMLIFrameElement)
libweb_js_wrapper(html/HTMLImageElement)
libweb_js_wrapper(html/HTMLInputElement)
libweb_js_wrapper(html/HTMLLabelElement)
libweb_js_wrapper(html/HTMLLegendElement)
libweb_js_wrapper(html/HTMLLIElement)
libweb_js_wrapper(html/HTMLLinkElement)
libweb_js_wrapper(html/HTMLMapElement)
libweb_js_wrapper(html/HTMLMarqueeElement)
libweb_js_wrapper(html/HTMLMediaElement)
libweb_js_wrapper(html/HTMLMenuElement)
libweb_js_wrapper(html/HTMLMetaElement)
libweb_js_wrapper(html/HTMLMeterElement)
libweb_js_wrapper(html/HTMLModElement)
libweb_js_wrapper(html/HTMLObjectElement)
libweb_js_wrapper(html/HTMLOListElement)
libweb_js_wrapper(html/HTMLOptGroupElement)
libweb_js_wrapper(html/HTMLOptionElement)
libweb_js_wrapper(html/HTMLOutputElement)
libweb_js_wrapper(html/HTMLParagraphElement)
libweb_js_wrapper(html/HTMLParamElement)
libweb_js_wrapper(html/HTMLPictureElement)
libweb_js_wrapper(html/HTMLPreElement)
libweb_js_wrapper(html/HTMLProgressElement)
libweb_js_wrapper(html/HTMLQuoteElement)
libweb_js_wrapper(html/HTMLScriptElement)
libweb_js_wrapper(html/HTMLSelectElement)
libweb_js_wrapper(html/HTMLSlotElement)
libweb_js_wrapper(html/HTMLSourceElement)
libweb_js_wrapper(html/HTMLSpanElement)
libweb_js_wrapper(html/HTMLStyleElement)
libweb_js_wrapper(html/HTMLTableCaptionElement)
libweb_js_wrapper(html/HTMLTableCellElement)
libweb_js_wrapper(html/HTMLTableColElement)
libweb_js_wrapper(html/HTMLTableElement)
libweb_js_wrapper(html/HTMLTableRowElement)
libweb_js_wrapper(html/HTMLTableSectionElement)
libweb_js_wrapper(html/HTMLTemplateElement)
libweb_js_wrapper(html/HTMLTextAreaElement)
libweb_js_wrapper(html/HTMLTimeElement)
libweb_js_wrapper(html/HTMLTitleElement)
libweb_js_wrapper(html/HTMLTrackElement)
libweb_js_wrapper(html/HTMLUListElement)
libweb_js_wrapper(html/HTMLUnknownElement)
libweb_js_wrapper(html/HTMLVideoElement)
libweb_js_wrapper(html/ImageData)
libweb_js_wrapper(html/MessageEvent)
libweb_js_wrapper(html/SubmitEvent)
libweb_js_wrapper(html/WebSocket)
libweb_js_wrapper(highresolutiontime/Performance)
libweb_js_wrapper(navigationtiming/PerformanceTiming)
libweb_js_wrapper(svg/SVGElement)
libweb_js_wrapper(svg/SVGGeometryElement)
libweb_js_wrapper(svg/SVGGraphicsElement)
libweb_js_wrapper(svg/SVGPathElement)
libweb_js_wrapper(svg/SVGSVGElement)
libweb_js_wrapper(uievents/MouseEvent)
libweb_js_wrapper(uievents/UIEvent)
libweb_js_wrapper(xhr/ProgressEvent)
libweb_js_wrapper(xhr/XMLHttpRequest)
libweb_js_wrapper(xhr/XMLHttpRequestEventTarget)

get_property(WRAPPER_SOURCES GLOBAL PROPERTY wrapper_sources)
set(SOURCES ${SOURCES} ${WRAPPER_SOURCES})

add_custom_command(
    OUTPUT css/PropertyID.h
    COMMAND ${write_if_different} css/PropertyID.h CodeGenerators/Generate_CSS_PropertyID_h ${CMAKE_CURRENT_SOURCE_DIR}/css/Properties.json
    VERBATIM
    DEPENDS Generate_CSS_PropertyID_h
    MAIN_DEPENDENCY css/Properties.json
)
add_custom_target(generate_PropertyID.h DEPENDS css/PropertyID.h)

add_custom_command(
    OUTPUT css/PropertyID.cpp
    COMMAND ${CMAKE_COMMAND} -E make_directory CSS
    COMMAND ${write_if_different} css/PropertyID.cpp CodeGenerators/Generate_CSS_PropertyID_cpp ${CMAKE_CURRENT_SOURCE_DIR}/css/Properties.json
    VERBATIM
    DEPENDS Generate_CSS_PropertyID_cpp
    MAIN_DEPENDENCY css/Properties.json
)

add_custom_command(
    OUTPUT css/ValueID.h
    COMMAND ${write_if_different} css/ValueID.h CodeGenerators/Generate_CSS_ValueID_h ${CMAKE_CURRENT_SOURCE_DIR}/css/Identifiers.json
    VERBATIM
    DEPENDS Generate_CSS_ValueID_h
    MAIN_DEPENDENCY css/Identifiers.json
)
add_custom_target(generate_ValueID.h DEPENDS css/ValueID.h)

add_custom_command(
    OUTPUT css/ValueID.cpp
    COMMAND ${CMAKE_COMMAND} -E make_directory CSS
    COMMAND ${write_if_different} css/ValueID.cpp CodeGenerators/Generate_CSS_ValueID_cpp ${CMAKE_CURRENT_SOURCE_DIR}/css/Identifiers.json
    VERBATIM
    DEPENDS Generate_CSS_ValueID_cpp
    MAIN_DEPENDENCY css/Identifiers.json
)

add_custom_command(
    OUTPUT css/DefaultStyleSheetSource.cpp
    COMMAND ${write_if_different} css/DefaultStyleSheetSource.cpp ${CMAKE_CURRENT_SOURCE_DIR}/scripts/GenerateStyleSheetSource.sh default_stylesheet_source ${CMAKE_CURRENT_SOURCE_DIR}/css/Default.css
    VERBATIM
    DEPENDS scripts/GenerateStyleSheetSource.sh
    MAIN_DEPENDENCY css/Default.css
)

add_custom_command(
    OUTPUT css/QuirksModeStyleSheetSource.cpp
    COMMAND ${write_if_different} css/QuirksModeStyleSheetSource.cpp ${CMAKE_CURRENT_SOURCE_DIR}/scripts/GenerateStyleSheetSource.sh quirks_mode_stylesheet_source ${CMAKE_CURRENT_SOURCE_DIR}/css/QuirksMode.css
    VERBATIM
    DEPENDS scripts/GenerateStyleSheetSource.sh
    MAIN_DEPENDENCY css/Default.css
)

pranaos_lib(libweb web)
target_link_libraries(libweb libcore libjs libnarkdown libgemini libgui libgfx libtextcodec libprotocol libimagedecoderclient libwasm)

add_subdirectory(DumpLayoutTree)
