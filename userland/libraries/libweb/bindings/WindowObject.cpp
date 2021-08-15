/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Base64.h>
#include <base/String.h>
#include <base/Utf8View.h>
#include <libjs/runtime/Error.h>
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/Shape.h>
#include <libtextcodec/Decoder.h>
#include <libweb/bindings/DocumentWrapper.h>
#include <libweb/bindings/EventTargetConstructor.h>
#include <libweb/bindings/EventTargetPrototype.h>
#include <libweb/bindings/EventWrapper.h>
#include <libweb/bindings/EventWrapperFactory.h>
#include <libweb/bindings/LocationObject.h>
#include <libweb/bindings/NavigatorObject.h>
#include <libweb/bindings/NodeWrapperFactory.h>
#include <libweb/bindings/PerformanceWrapper.h>
#include <libweb/bindings/ScreenWrapper.h>
#include <libweb/bindings/WindowObject.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Event.h>
#include <libweb/dom/Window.h>
#include <libweb/Origin.h>
#include <libweb/page/BrowsingContext.h>
#include <libweb/webassembly/WebAssemblyObject.h>
#include <libweb/bindings/WindowObjectHelper.h>

namespace Web::Bindings {

WindowObject::WindowObject(DOM::Window& impl)
    : m_impl(impl)
{
    impl.set_wrapper({}, *this);
}

void WindowObject::initialize_global_object()
{
    Base::initialize_global_object();

    auto success = Object::internal_set_prototype_of(&ensure_web_prototype<EventTargetPrototype>("EventTarget"));
    VERIFY(success);

    define_direct_property("window", this, JS::Attribute::Enumerable);
    define_direct_property("frames", this, JS::Attribute::Enumerable);
    define_direct_property("self", this, JS::Attribute::Enumerable);
    define_native_accessor("top", top_getter, nullptr, JS::Attribute::Enumerable);
    define_native_accessor("parent", parent_getter, {}, JS::Attribute::Enumerable);
    define_native_accessor("document", document_getter, {}, JS::Attribute::Enumerable);
    define_native_accessor("performance", performance_getter, {}, JS::Attribute::Enumerable);
    define_native_accessor("screen", screen_getter, {}, JS::Attribute::Enumerable);
    define_native_accessor("innerWidth", inner_width_getter, {}, JS::Attribute::Enumerable);
    define_native_accessor("innerHeight", inner_height_getter, {}, JS::Attribute::Enumerable);
    u8 attr = JS::Attribute::Writable | JS::Attribute::Enumerable | JS::Attribute::Configurable;
    define_native_function("alert", alert, 0, attr);
    define_native_function("confirm", confirm, 0, attr);
    define_native_function("prompt", prompt, 0, attr);
    define_native_function("setInterval", set_interval, 1, attr);
    define_native_function("setTimeout", set_timeout, 1, attr);
    define_native_function("clearInterval", clear_interval, 1, attr);
    define_native_function("clearTimeout", clear_timeout, 1, attr);
    define_native_function("requestAnimationFrame", request_animation_frame, 1, attr);
    define_native_function("cancelAnimationFrame", cancel_animation_frame, 1, attr);
    define_native_function("atob", atob, 1, attr);
    define_native_function("btoa", btoa, 1, attr);

    define_native_accessor("event", event_getter, {}, JS::Attribute::Enumerable);

    define_direct_property("navigator", heap().allocate<NavigatorObject>(*this, *this), JS::Attribute::Enumerable | JS::Attribute::Configurable);
    define_direct_property("location", heap().allocate<LocationObject>(*this, *this), JS::Attribute::Enumerable | JS::Attribute::Configurable);

    define_direct_property("WebAssembly", heap().allocate<WebAssemblyObject>(*this, *this), JS::Attribute::Enumerable | JS::Attribute::Configurable);

    ADD_WINDOW_OBJECT_INTERFACES;
}

WindowObject::~WindowObject()
{
}

void WindowObject::visit_edges(Visitor& visitor)
{
    GlobalObject::visit_edges(visitor);
    for (auto& it : m_prototypes)
        visitor.visit(it.value);
    for (auto& it : m_constructors)
        visitor.visit(it.value);
}

Origin WindowObject::origin() const
{
    return impl().document().origin();
}

static DOM::Window* impl_from(JS::VM& vm, JS::GlobalObject& global_object)
{
    auto this_value = vm.this_value(global_object);
    if (this_value.is_nullish()) {
        this_value = global_object.value_of();
    }

    auto* this_object = this_value.to_object(global_object);
    VERIFY(this_object);

    if (StringView("WindowObject") != this_object->class_name()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::NotA, "WindowObject");
        return nullptr;
    }
    return &static_cast<WindowObject*>(this_object)->impl();
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::alert)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    String message = "";
    if (vm.argument_count()) {
        message = vm.argument(0).to_string(global_object);
        if (vm.exception())
            return {};
    }
    impl->alert(message);
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::confirm)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    String message = "";
    if (!vm.argument(0).is_undefined()) {
        message = vm.argument(0).to_string(global_object);
        if (vm.exception())
            return {};
    }
    return JS::Value(impl->confirm(message));
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::prompt)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    String message = "";
    String default_ = "";
    if (!vm.argument(0).is_undefined()) {
        message = vm.argument(0).to_string(global_object);
        if (vm.exception())
            return {};
    }
    if (!vm.argument(1).is_undefined()) {
        default_ = vm.argument(1).to_string(global_object);
        if (vm.exception())
            return {};
    }
    auto response = impl->prompt(message, default_);
    if (response.is_null())
        return JS::js_null();
    return JS::js_string(vm, response);
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::set_interval)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    if (!vm.argument_count()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::BadArgCountAtLeastOne, "setInterval");
        return {};
    }
    auto* callback_object = vm.argument(0).to_object(global_object);
    if (!callback_object)
        return {};
    if (!callback_object->is_function()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::NotAFunctionNoParam);
        return {};
    }
    i32 interval = 0;
    if (vm.argument_count() >= 2) {
        interval = vm.argument(1).to_i32(global_object);
        if (vm.exception())
            return {};
        if (interval < 0)
            interval = 0;
    }

    auto timer_id = impl->set_interval(*static_cast<JS::FunctionObject*>(callback_object), interval);
    return JS::Value(timer_id);
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::set_timeout)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    if (!vm.argument_count()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::BadArgCountAtLeastOne, "setTimeout");
        return {};
    }
    auto* callback_object = vm.argument(0).to_object(global_object);
    if (!callback_object)
        return {};
    if (!callback_object->is_function()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::NotAFunctionNoParam);
        return {};
    }
    i32 interval = 0;
    if (vm.argument_count() >= 2) {
        interval = vm.argument(1).to_i32(global_object);
        if (vm.exception())
            return {};
        if (interval < 0)
            interval = 0;
    }

    auto timer_id = impl->set_timeout(*static_cast<JS::FunctionObject*>(callback_object), interval);
    return JS::Value(timer_id);
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::clear_timeout)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    if (!vm.argument_count()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::BadArgCountAtLeastOne, "clearTimeout");
        return {};
    }
    i32 timer_id = vm.argument(0).to_i32(global_object);
    if (vm.exception())
        return {};
    impl->clear_timeout(timer_id);
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::clear_interval)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    if (!vm.argument_count()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::BadArgCountAtLeastOne, "clearInterval");
        return {};
    }
    i32 timer_id = vm.argument(0).to_i32(global_object);
    if (vm.exception())
        return {};
    impl->clear_timeout(timer_id);
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::request_animation_frame)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    if (!vm.argument_count()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::BadArgCountOne, "requestAnimationFrame");
        return {};
    }
    auto* callback_object = vm.argument(0).to_object(global_object);
    if (!callback_object)
        return {};
    if (!callback_object->is_function()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::NotAFunctionNoParam);
        return {};
    }
    return JS::Value(impl->request_animation_frame(*static_cast<JS::FunctionObject*>(callback_object)));
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::cancel_animation_frame)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    if (!vm.argument_count()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::BadArgCountOne, "cancelAnimationFrame");
        return {};
    }
    auto id = vm.argument(0).to_i32(global_object);
    if (vm.exception())
        return {};
    impl->cancel_animation_frame(id);
    return JS::js_undefined();
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::atob)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    if (!vm.argument_count()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::BadArgCountOne, "atob");
        return {};
    }
    auto string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};
    auto decoded = decode_base64(StringView(string));

    auto decoder = TextCodec::decoder_for("windows-1252");
    VERIFY(decoder);
    return JS::js_string(vm, decoder->to_utf8(decoded));
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::btoa)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    if (!vm.argument_count()) {
        vm.throw_exception<JS::TypeError>(global_object, JS::ErrorType::BadArgCountOne, "btoa");
        return {};
    }
    auto string = vm.argument(0).to_string(global_object);
    if (vm.exception())
        return {};

    Vector<u8> byte_string;
    byte_string.ensure_capacity(string.length());
    for (u32 code_point : Utf8View(string)) {
        if (code_point > 0xff) {
            vm.throw_exception<JS::InvalidCharacterError>(global_object, JS::ErrorType::NotAByteString, "btoa");
            return {};
        }
        byte_string.append(code_point);
    }

    auto encoded = encode_base64(byte_string.span());
    return JS::js_string(vm, move(encoded));
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::top_getter)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};

    auto* this_browsing_context = impl->document().browsing_context();
    if (!this_browsing_context)
        return JS::js_null();

    VERIFY(this_browsing_context->top_level_browsing_context().document());
    auto& top_window = this_browsing_context->top_level_browsing_context().document()->window();
    return top_window.wrapper();
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::parent_getter)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};

    auto* this_browsing_context = impl->document().browsing_context();
    if (!this_browsing_context)
        return JS::js_null();

    if (this_browsing_context->parent()) {
        VERIFY(this_browsing_context->parent()->document());
        auto& parent_window = this_browsing_context->parent()->document()->window();
        return parent_window.wrapper();
    }
    VERIFY(this_browsing_context == &this_browsing_context->top_level_browsing_context());
    return impl->wrapper();
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::document_getter)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    return wrap(global_object, impl->document());
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::performance_getter)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    return wrap(global_object, impl->performance());
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::screen_getter)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    return wrap(global_object, impl->screen());
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::event_getter)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    if (!impl->current_event())
        return JS::js_undefined();
    return wrap(global_object, const_cast<DOM::Event&>(*impl->current_event()));
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::inner_width_getter)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    return JS::Value(impl->inner_width());
}

JS_DEFINE_NATIVE_FUNCTION(WindowObject::inner_height_getter)
{
    auto* impl = impl_from(vm, global_object);
    if (!impl)
        return {};
    return JS::Value(impl->inner_height());
}

}
