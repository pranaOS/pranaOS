/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/dom/Element.h>

namespace Web::DOM {

NonnullRefPtr<Element> create_element(Document&, const FlyString& tag_name, const FlyString& namespace_);

}
