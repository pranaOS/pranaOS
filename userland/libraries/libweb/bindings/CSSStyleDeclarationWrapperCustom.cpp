/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/ScopeGuard.h>
#include <libweb/bindings/CSSStyleDeclarationWrapper.h>
#include <libweb/css/parser/Parser.h>
#include <libweb/dom/Element.h>

namespace Web::Bindings {

bool CSSStyleDeclarationWrapper::internal_has_property(JS::PropertyName const& name) const
{
    if (!name.is_string())
        return Base::internal_has_property(name);

    auto property_id = CSS::property_id_from_string(name.to_string());
    return property_id != CSS::PropertyID::Invalid;
}

JS::Value CSSStyleDeclarationWrapper::internal_get(JS::PropertyName const& name, JS::Value receiver) const
{
    if (!name.is_string())
        return Base::internal_get(name, receiver);
 
    auto property_id = CSS::property_id_from_string(name.to_string());
    if (property_id == CSS::PropertyID::Invalid)
        return Base::internal_get(name, receiver);
    for (auto& property : impl().properties()) {
        if (property.property_id == property_id)
            return js_string(vm(), property.value->to_string());
    }
    return js_string(vm(), String::empty());
}

bool CSSStyleDeclarationWrapper::internal_set(JS::PropertyName const& name, JS::Value value, JS::Value receiver)
{
    if (!name.is_string())
        return Base::internal_set(name, value, receiver);

    auto property_id = CSS::property_id_from_string(name.to_string());
    if (property_id == CSS::PropertyID::Invalid)
        return Base::internal_set(name, value, receiver);

    auto css_text = value.to_string(global_object());
    if (vm().exception())
        return false;

    auto new_value = parse_css_value(CSS::ParsingContext {}, css_text, property_id);

    if (!new_value)
        return false;

    ScopeGuard style_invalidation_guard = [&] {
        auto& declaration = verify_cast<CSS::ElementInlineCSSStyleDeclaration>(impl());
        if (auto* element = declaration.element())
            element->invalidate_style();
    };


    for (auto& property : impl().m_properties) {
        if (property.property_id == property_id) {
            property.value = new_value.release_nonnull();
            return true;
        }
    }

    impl().m_properties.append(CSS::StyleProperty {
        .property_id = property_id,
        .value = new_value.release_nonnull(),
        .important = false,
    });
    return true;
}

}
