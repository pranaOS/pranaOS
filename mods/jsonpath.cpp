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
    JsonPathElement JsonPathElement::any_array_element{Kind::AnyIndex};
    JsonPathElement JsonPathElement::any_object_element{Kind::AnyKey};

    /**
     * @param top_root 
     * @return JsonValue 
     */
    JsonValue JsonPath::resolve(JsonValue const& top_root) const
    {
        auto root = top_root;

        for(auto const& element : *this)
        {
            switch(element.kind())
            {
            case JsonPathElement::Kind::Key:
                root = JsonValue{root.as_object().get(element.key())};
                break;
            case JsonPathElement::Kind::Index:
                root = JsonValue{root.as_array().at(element.index())};
                break;
            default:
                VERIFY_NOT_REACHED();
            }
        }

        return root;
    }

    /**
     * @return String 
     */
    String JsonPath::to_string() const
    {
        StringBuilder builder;
        builder.append("{ .");

        for(auto const& el : *this)
        {
            builder.append(" > ");
            builder.append(el.to_string());
        }

        builder.append(" }");
        return builder.to_string();
    }
} // namespace Mods
