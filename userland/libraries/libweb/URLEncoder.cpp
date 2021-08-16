/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/StringBuilder.h>
#include <base/URL.h>
#include <libweb/URLEncoder.h>

namespace Web {

String urlencode(const Vector<URLQueryParam>& pairs, URL::PercentEncodeSet percent_encode_set)
{
    StringBuilder builder;
    for (size_t i = 0; i < pairs.size(); ++i) {
        builder.append(URL::percent_encode(pairs[i].name, percent_encode_set));
        builder.append('=');
        builder.append(URL::percent_encode(pairs[i].value, percent_encode_set));
        if (i != pairs.size() - 1)
            builder.append('&');
    }
    return builder.to_string();
}

}
