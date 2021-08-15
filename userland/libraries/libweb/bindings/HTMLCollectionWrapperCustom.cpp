/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/bindings/HTMLCollectionWrapper.h>
#include <libweb/bindings/NodeWrapper.h>
#include <libweb/bindings/NodeWrapperFactory.h>
#include <libweb/dom/Element.h>

namespace Web::Bindings {

JS::Value HTMLCollectionWrapper::internal_get(JS::PropertyName const& property_name, JS::Value receiver) const
{
    if (property_name.is_symbol())
        return Base::internal_get(property_name, receiver);
    DOM::Element* item = nullptr;
    if (property_name.is_string())
        item = const_cast<DOM::HTMLCollection&>(impl()).named_item(property_name.to_string());
    else if (property_name.is_number())
        item = const_cast<DOM::HTMLCollection&>(impl()).item(property_name.as_number());
    if (!item)
        return Base::internal_get(property_name, receiver);
    return wrap(global_object(), *item);
}

}
