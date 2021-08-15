/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/TypeCasts.h>
#include <base/Weakable.h>
#include <libjs/runtime/GlobalObject.h>
#include <libweb/Forward.h>

namespace Web {
namespace Bindings {

class WindowObject final
    : public JS::GlobalObject
    , public Weakable<WindowObject> {
    JS_OBJECT(WindowObject, JS::GlobalObject);

public:
    explicit WindowObject(DOM::Window&);
    virtual void initialize_global_object() override;
    virtual ~WindowObject() override;

    DOM::Window& impl() { return *m_impl; }
    const DOM::Window& impl() const { return *m_impl; }

    Origin origin() const;

    JS::Object* web_prototype(const String& class_name) { return m_prototypes.get(class_name).value_or(nullptr); }
    JS::NativeFunction* web_constructor(const String& class_name) { return m_constructors.get(class_name).value_or(nullptr); }

    template<typename T>
    JS::Object& ensure_web_prototype(const String& class_name)
    {
        auto it = m_prototypes.find(class_name);
        if (it != m_prototypes.end())
            return *it->value;
        auto* prototype = heap().allocate<T>(*this, *this);
        m_prototypes.set(class_name, prototype);
        return *prototype;
    }

    template<typename T>
    JS::NativeFunction& ensure_web_constructor(const String& class_name)
    {
        auto it = m_constructors.find(class_name);
        if (it != m_constructors.end())
            return *it->value;
        auto* constructor = heap().allocate<T>(*this, *this);
        m_constructors.set(class_name, constructor);
        define_direct_property(class_name, JS::Value(constructor), JS::Attribute::Writable | JS::Attribute::Configurable);
        return *constructor;
    }

private:
    virtual void visit_edges(Visitor&) override;

    JS_DECLARE_NATIVE_FUNCTION(top_getter);

    JS_DECLARE_NATIVE_FUNCTION(document_getter);

    JS_DECLARE_NATIVE_FUNCTION(performance_getter);
    JS_DECLARE_NATIVE_FUNCTION(screen_getter);

    JS_DECLARE_NATIVE_FUNCTION(event_getter);

    JS_DECLARE_NATIVE_FUNCTION(inner_width_getter);
    JS_DECLARE_NATIVE_FUNCTION(inner_height_getter);

    JS_DECLARE_NATIVE_FUNCTION(parent_getter);

    JS_DECLARE_NATIVE_FUNCTION(alert);
    JS_DECLARE_NATIVE_FUNCTION(confirm);
    JS_DECLARE_NATIVE_FUNCTION(prompt);
    JS_DECLARE_NATIVE_FUNCTION(set_interval);
    JS_DECLARE_NATIVE_FUNCTION(set_timeout);
    JS_DECLARE_NATIVE_FUNCTION(clear_interval);
    JS_DECLARE_NATIVE_FUNCTION(clear_timeout);
    JS_DECLARE_NATIVE_FUNCTION(request_animation_frame);
    JS_DECLARE_NATIVE_FUNCTION(cancel_animation_frame);
    JS_DECLARE_NATIVE_FUNCTION(atob);
    JS_DECLARE_NATIVE_FUNCTION(btoa);

    NonnullRefPtr<DOM::Window> m_impl;

    HashMap<String, JS::Object*> m_prototypes;
    HashMap<String, JS::NativeFunction*> m_constructors;
};

}
}
