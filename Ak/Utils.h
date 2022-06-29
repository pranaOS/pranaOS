/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

namespace ak {
    class utils;

    class Utils {
      public:
        bool match(const utils& str);
        bool convert_uint(const utils&, bool& ok);
    };
}