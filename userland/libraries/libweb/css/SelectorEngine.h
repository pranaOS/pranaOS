/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/css/Selector.h>
#include <libweb/dom/Element.h>

namespace Web::SelectorEngine {

bool matches(CSS::Selector const&, DOM::Element const&);

}
