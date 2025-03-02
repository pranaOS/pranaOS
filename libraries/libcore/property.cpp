/**
 * @file property.cpp
 * @author Krisna Pranav
 * @brief property
 * @version 6.0
 * @date 2025-03-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libcore/property.h>

namespace Core 
{

    /**
     * @brief Construct a new Property::Property object
     * 
     * @param name 
     * @param getter 
     * @param setter 
     */
    Property::Property(String name, Function<JsonValue()> getter, Function<bool(JsonValue const&)> setter)
        : m_name(move(name))
        , m_getter(move(getter))
        , m_setter(move(setter))
    {
    }

} // namespace Core
