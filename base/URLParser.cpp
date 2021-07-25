/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/CharacterTypes.h>
#include <base/Debug.h>
#include <base/Optional.h>
#include <base/SourceLocation.h>
#include <base/String.h>
#include <base/StringBuilder.h>
#include <base/StringUtils.h>
#include <base/URLParser.h>
#include <base/Utf8View.h>

namespace Base {

constexpr u32 end_of_file = 0xFFFFFFFF;

constexpr bool is_url_code_point(u32 code_point)
{
    return is_ascii_alphanumeric(code_point) || code_point >= 0xA0 || "!$&'()*+,-./:;=?@_~"sv.contains(code_point);
}

static void report_validation_error(SourceLocation const& location = SourceLocation::current())
{
    dbgln_if(URL_PARSER_DEBUG, "URLParser::parse: Validation error! {}", location);
}

static Optional<String> parse_opaque_host(StringView const& input)
{
    auto forbidden_host_code_points_excluding_percent = "\0\t\n\r #/:<>?@[\\]^|"sv;
    for (auto code_point : forbidden_host_code_points_excluding_percent) {
        if (input.contains(code_point)) {
            report_validation_error();
            return {};
        }
    }
    return URL::percent_encode(input, URL::PercentEncodeSet::C0Control);
}

static Optional<String> parse_ipv4_address(StringView const& input)
{
    return input;
}

static Optional<String> parse_host(StringView const& input, bool is_not_special = false)
{
    if (input.starts_with('[')) {
        if (!input.ends_with(']')) {
            report_validation_error();
            return {};
        }

        TODO();
    }

    if (is_not_special)
        return parse_opaque_host(input);
    VERIFY(!input.is_empty());

    auto domain = URL::percent_decode(input);

    auto& ascii_domain = domain;

    auto forbidden_host_code_points = "\0\t\n\r #%/:<>?@[\\]^|"sv;
    for (auto code_point : forbidden_host_code_points) {
        if (ascii_domain.view().contains(code_point)) {
            report_validation_error();
            return {};
        }
    }

    auto ipv4_host = parse_ipv4_address(ascii_domain);
    return ipv4_host;
}

constexpr bool starts_with_windows_drive_letter(StringView const& input)
{
    if (input.length() < 2)
        return false;
    if (!is_ascii_alpha(input[0]) && !(input[1] == ':' || input[1] == '|'))
        return false;
    if (input.length() == 2)
        return true;
    return "/\\?#"sv.contains(input[2]);
}

constexpr bool is_windows_drive_letter(StringView const& input)
{
    return input.length() == 2 && is_ascii_alpha(input[0]) && (input[1] == ':' || input[1] == '|');
}

constexpr bool is_normalized_windows_drive_letter(StringView const& input)
{
    return input.length() == 2 && is_ascii_alpha(input[0]) && input[1] == ':';
}

constexpr bool is_single_dot_path_segment(StringView const& input)
{
    return input == "."sv || input.equals_ignoring_case("%2e"sv);
}

constexpr bool is_double_dot_path_segment(StringView const& input)
{
    return input == ".."sv || input.equals_ignoring_case(".%2e"sv) || input.equals_ignoring_case("%2e."sv) || input.equals_ignoring_case("%2e%2e"sv);
}

Optional<URL> URLParser::parse_data_url(StringView const& raw_input)
{
    dbgln_if(URL_PARSER_DEBUG, "URLParser::parse_data_url: Parsing '{}'.", raw_input);
    VERIFY(raw_input.starts_with("data:"));
    auto input = raw_input.substring_view(5);
    auto comma_offset = input.find(',');
    if (!comma_offset.has_value())
        return {};
    auto mime_type = input.substring_view(0, comma_offset.value());
    auto encoded_body = input.substring_view(comma_offset.value() + 1);
    auto body = URL::percent_decode(encoded_body);
    bool is_base_64_encoded = false;
    if (mime_type.ends_with(";base64", CaseSensitivity::CaseInsensitive)) {
        is_base_64_encoded = true;
        mime_type = mime_type.substring_view(0, mime_type.length() - 7);
    }

    if (mime_type.starts_with(";")) {
        StringBuilder builder;
        builder.append("text/plain");
        builder.append(mime_type);
        mime_type = builder.to_string();
    }

    URL url { mime_type, move(body), is_base_64_encoded };
    dbgln_if(URL_PARSER_DEBUG, "URLParser::parse_data_url: Parsed data URL to be '{}'.", url.serialize());
    return url;
}

}