/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/css/StyleSheet.h>
#include <libweb/dom/Element.h>

namespace Web::CSS {

void StyleSheet::set_owner_node(DOM::Element* element)
{
    if (element)
        m_owner_node = element->make_weak_ptr<DOM::Element>();
    else
        m_owner_node = nullptr;
}

}
