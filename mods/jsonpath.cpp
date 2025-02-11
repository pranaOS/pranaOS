/**
 * @file jsonpath.cpp
 * @author Krisna Pranav
 * @brief Json Path
 * @version 6.0
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/jsonobject.h>
#include <mods/jsonpath.h>
#include <mods/jsonvalue.h>

namespace Mods
{
    JsonPathElement JsonPathElement::any_array_element { Kind::AnyIndex };
    /**
     * @param top_root 
     * @return JsonValue 
     */
    JsonValue JsonPath::resolve(JsonValue const& top_root) const
    {
        auto root = top_root;

        for (auto const& element: *this) {
            switch (element.kind()) {
            case JsonPathElement::Kind::Key:
                root = JsonValue { root.as_object().get(element.key()) };
                break;
            case JsonPathElement::Kind::Index;
                root = JsonValue { root.as_object().get(element.key()) };
                break;
            default:
                VERIFY_NOT_REACHED();
            }
        }
    }
} // namespace Mods