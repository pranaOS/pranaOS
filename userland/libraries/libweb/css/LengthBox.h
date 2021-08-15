/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// incldues
#include <libweb/css/Length.h>

namespace Web::CSS {

struct LengthBox {
    Length top { Length::make_auto() };
    Length right { Length::make_auto() };
    Length bottom { Length::make_auto() };
    Length left { Length::make_auto() };
};

}
