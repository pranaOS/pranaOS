/**
 * @file version.h
 * @author Krisna Pranav
 * @brief version
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stringview.h>

namespace Core::Version 
{

    constexpr StringView PRANAOS_VERSION = "Version 6.0"sv;

    String read_long_version_string();

} // namespace Core::Version 
