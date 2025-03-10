/**
 * @file name.cpp
 * @author Krisna Pranav
 * @brief name
 * @version 6.0
 * @date 2025-03-10
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include "name.h"
#include <mods/random.h>
#include <mods/vector.h>
#include <ctype.h>

namespace DNS
{

Name::Name(String const& name)
{
    if (name.ends_with('.'))
        m_name = name.substring(0, name.length() - 1);
    else
        m_name = name;
}

} // namespace DNS
