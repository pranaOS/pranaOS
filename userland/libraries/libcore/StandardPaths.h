/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Forward.h>

namespace Core {

class StandardPaths {
public:
    static String home_directory();
    static String desktop_directory();
    static String downloads_directory();
    static String tempfile_directory();
    static String config_directory();
};

}
