/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/ByteBuffer.h>
#include <base/JsonObject.h>
#include <base/SourceGenerator.h>
#include <base/StringBuilder.h>
#include <libcore/File.h>
#include <ctype.h>

static String title_casify(const String& dashy_name)
{
    auto parts = dashy_name.split('-');
    StringBuilder builder;
    for (auto& part : parts) {
        if (part.is_empty())
            continue;
        builder.append(toupper(part[0]));
        if (part.length() == 1)
            continue;
        builder.append(part.substring_view(1, part.length() - 1));
    }
    return builder.to_string();
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        warnln("usage: {} <path/to/CSS/Identifiers.json>", argv[0]);
        return 1;
    }
    auto file = Core::File::construct(argv[1]);
    if (!file->open(Core::OpenMode::ReadOnly))
        return 1;

    auto json = JsonValue::from_string(file->read_all());
    VERIFY(json.has_value());
    VERIFY(json.value().is_array());

    StringBuilder builder;
    SourceGenerator generator { builder };
    generator.append(R"~~~(
#pragma once

#include <base/StringView.h>
#include <base/Traits.h>

namespace Web::CSS {

enum class ValueID {
    Invalid,
)~~~");

    json.value().as_array().for_each([&](auto& name) {
        auto member_generator = generator.fork();
        member_generator.set("name:titlecase", title_casify(name.to_string()));

        member_generator.append(R"~~~(
    @name:titlecase@,
)~~~");
    });

    generator.append(R"~~~(
};

ValueID value_id_from_string(const StringView&);
const char* string_from_value_id(ValueID);

}

)~~~");

    outln("{}", generator.as_string_view());
}
