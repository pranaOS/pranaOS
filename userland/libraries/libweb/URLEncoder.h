/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/String.h>
#include <base/Vector.h>

namespace Web {

struct URLQueryParam {
    String name;
    String value;
};

String urlencode(const Vector<URLQueryParam>&, URL::PercentEncodeSet);

}
