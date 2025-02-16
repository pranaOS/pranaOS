/**
 * @file urlparser.cpp
 * @author Krisna Pranav
 * @brief URLParser
 * @version 6.0
 * @date 2023-07-27
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/charactertypes.h>
#include <mods/debug.h>
#include <mods/optional.h>
#include <mods/sourcelocation.h>
#include <mods/string.h>
#include <mods/stringbuilder.h>
#include <mods/stringutils.h>
#include <mods/urlparser.h>
#include <mods/utf8view.h>

namespace Mods
{
    constexpr u32 end_of_file = 0xFFFFFFFF;

    /**
     * @param code_point 
     * @return true 
     * @return false 
     */
    constexpr bool is_url_code_point(u32 code_point)
    {
        return is_ascii_alphanumeric(code_point) || code_point >= 0xA0 || "!$&'()*+,-./:;=?@_~"sv.contains(code_point);
    }

    /**
     * @param location 
     */
    static void report_validation_error(SourceLocation const& location = SourceLocation::current())
    {
        dbgln_if(URL_PARSER_DEBUG, "URLParser::parse: Validation error! {}", location);
    }

    /**
     * @param input 
     * @return Optional<String> 
     */
    static Optional<String> parse_opaque_host(StringView input)
    {
        auto forbidden_host_code_points_excluding_percent = "\0\t\n\r #/:<>?@[\\]^|"sv;
        for(auto code_point : forbidden_host_code_points_excluding_percent)
        {
            if(input.contains(code_point))
            {
                report_validation_error();
                return {};
            }
        }
        return URL::percent_encode(input, URL::PercentEncodeSet::C0Control);
    }

    /**
     * @param input 
     * @return Optional<String> 
     */
    static Optional<String> parse_ipv4_address(StringView input)
    {
        return input;
    }

    /**
     * @param input 
     * @param is_not_special 
     * @return Optional<String> 
     */
    static Optional<String> parse_host(StringView input, bool is_not_special = false)
    {
        if(input.starts_with('['))
        {
            if(!input.ends_with(']'))
            {
                report_validation_error();
                return {};
            }
            TODO();
        }

        if(is_not_special)
            return parse_opaque_host(input);
        VERIFY(!input.is_empty());

        auto domain = URL::percent_decode(input);
        
        auto& ascii_domain = domain;

        auto forbidden_host_code_points = "\0\t\n\r #%/:<>?@[\\]^|"sv;
        for(auto code_point : forbidden_host_code_points)
        {
            if(ascii_domain.view().contains(code_point))
            {
                report_validation_error();
                return {};
            }
        }

        auto ipv4_host = parse_ipv4_address(ascii_domain);
        return ipv4_host;
    }

    /**
     * @param input 
     * @return true 
     * @return false 
     */
    constexpr bool starts_with_windows_drive_letter(StringView input)
    {
        if(input.length() < 2)
            return false;
        if(!is_ascii_alpha(input[0]) && !(input[1] == ':' || input[1] == '|'))
            return false;
        if(input.length() == 2)
            return true;
        return "/\\?#"sv.contains(input[2]);
    }

    /**
     * @param input 
     * @return true 
     * @return false 
     */
    constexpr bool is_windows_drive_letter(StringView input)
    {
        return input.length() == 2 && is_ascii_alpha(input[0]) && (input[1] == ':' || input[1] == '|');
    }

    /**
     * @param input 
     * @return true 
     * @return false 
     */
    constexpr bool is_normalized_windows_drive_letter(StringView input)
    {
        return input.length() == 2 && is_ascii_alpha(input[0]) && input[1] == ':';
    }

    /**
     * @param input 
     * @return true 
     * @return false 
     */
    constexpr bool is_single_dot_path_segment(StringView input)
    {
        return input == "."sv || input.equals_ignoring_case("%2e"sv);
    }

    /**
     * @param input 
     * @return true 
     * @return false 
     */
    constexpr bool is_double_dot_path_segment(StringView input)
    {
        return input == ".."sv || input.equals_ignoring_case(".%2e"sv) || input.equals_ignoring_case("%2e."sv) || input.equals_ignoring_case("%2e%2e"sv);
    }

    /**
     * @param input 
     * @param percent_encode_set 
     * @param space_as_plus 
     * @return String 
     */
    static String percent_encode_after_encoding(StringView input, URL::PercentEncodeSet percent_encode_set, bool space_as_plus = false)
    {
        StringBuilder output;

        for(auto byte : input)
        {
            if(space_as_plus && byte == ' ')
            {
                output.append('+');
                continue;
            }

            u32 isomorph = byte;

            if(!URL::code_point_is_in_percent_encode_set(isomorph, percent_encode_set))
            {
                output.append_code_point(isomorph);
            }

            else
            {
                output.appendff("%{:02X}", byte);
            }
        }

        return output.to_string();
    }

    /**
     * @param raw_input 
     * @return Optional<URL> 
     */
    Optional<URL> URLParser::parse_data_url(StringView raw_input)
    {
        dbgln_if(URL_PARSER_DEBUG, "URLParser::parse_data_url: Parsing '{}'.", raw_input);
        VERIFY(raw_input.starts_with("data:"));
        auto input = raw_input.substring_view(5);
        auto comma_offset = input.find(',');

        if(!comma_offset.has_value())
            return {};

        auto mime_type = StringUtils::trim(input.substring_view(0, comma_offset.value()), "\t\n\f\r ", TrimMode::Both);
        auto encoded_body = input.substring_view(comma_offset.value() + 1);
        auto body = URL::percent_decode(encoded_body);
        bool is_base64_encoded = false;

        if(mime_type.ends_with("base64", CaseSensitivity::CaseInsensitive))
        {
            auto substring_view = mime_type.substring_view(0, mime_type.length() - 6);
            auto trimmed_substring_view = StringUtils::trim(substring_view, " ", TrimMode::Right);
            if(trimmed_substring_view.ends_with(';'))
            {
                is_base64_encoded = true;
                mime_type = trimmed_substring_view.substring_view(0, trimmed_substring_view.length() - 1);
            }
        }

        StringBuilder builder;
        if(mime_type.starts_with(";") || mime_type.is_empty())
        {
            builder.append("text/plain");
            builder.append(mime_type);
            mime_type = builder.string_view();
        }

        URL url{StringUtils::trim(mime_type, "\n\r\t ", TrimMode::Both), move(body), is_base64_encoded};
        dbgln_if(URL_PARSER_DEBUG, "URLParser::parse_data_url: Parsed data URL to be '{}'.", url.serialize());
        return url;
    }

    /**
     * @param raw_input 
     * @param base_url 
     * @param url 
     * @param state_override 
     * @return URL 
     */
    URL URLParser::parse(StringView raw_input, URL const* base_url, Optional<URL> url, Optional<State> state_override)
    {
        dbgln_if(URL_PARSER_DEBUG, "URLParser::parse: Parsing '{}'", raw_input);
        if(raw_input.is_empty())
            return {};

        if(raw_input.starts_with("data:"))
        {
            auto maybe_url = parse_data_url(raw_input);
            if(!maybe_url.has_value())
                return {};
            return maybe_url.release_value();
        }

        size_t start_index = 0;
        size_t end_index = raw_input.length();
        if(!url.has_value())
        {
            url = URL();

            bool has_validation_error = false;
            for(size_t i = 0; i < raw_input.length(); ++i)
            {
                i8 ch = raw_input[i];
                if(0 <= ch && ch <= 0x20)
                {
                    ++start_index;
                    has_validation_error = true;
                }
                else
                {
                    break;
                }
            }
            for(ssize_t i = raw_input.length() - 1; i >= 0; --i)
            {
                i8 ch = raw_input[i];
                if(0 <= ch && ch <= 0x20)
                {
                    --end_index;
                    has_validation_error = true;
                }
                else
                {
                    break;
                }
            }
            if(has_validation_error)
                report_validation_error();
        }
        if(start_index >= end_index)
            return {};

        String processed_input = raw_input.substring_view(start_index, end_index - start_index);

        if(processed_input.contains("\t") || processed_input.contains("\n"))
        {
            report_validation_error();
            processed_input = processed_input.replace("\t", "", true).replace("\n", "", true);
        }

        State state = state_override.value_or(State::SchemeStart);
        StringBuilder buffer;
        bool at_sign_seen = false;
        bool inside_brackets = false;
        bool password_token_seen = false;

        Utf8View input(processed_input);
        Utf8CodePointIterator iterator = input.begin();

        auto get_remaining = [&input, &iterator]
        {
            return input.substring_view(iterator - input.begin() + iterator.underlying_code_point_length_in_bytes()).as_string();
        };

        for(;;)
        {
            u32 code_point = end_of_file;
            if(!iterator.done())
                code_point = *iterator;

            if constexpr(URL_PARSER_DEBUG)
            {
                if(code_point == end_of_file)
                    dbgln("URLParser::parse: {} state with EOF.", state_name(state));
                else if(is_ascii_printable(code_point))
                    dbgln("URLParser::parse: {} state with code point U+{:04X} ({:c}).", state_name(state), code_point, code_point);
                else
                    dbgln("URLParser::parse: {} state with code point U+{:04X}.", state_name(state), code_point);
            }

            switch(state)
            {
            case State::SchemeStart:
                if(is_ascii_alpha(code_point))
                {
                    buffer.append_as_lowercase(code_point);
                    state = State::Scheme;
                }
                else
                {
                    state = State::NoScheme;
                    continue;
                }
                break;
            case State::Scheme:
                if(is_ascii_alphanumeric(code_point) || code_point == '+' || code_point == '-' || code_point == '.')
                {
                    buffer.append_as_lowercase(code_point);
                }
                else if(code_point == ':')
                {
                    url->m_scheme = buffer.to_string();
                    buffer.clear();
                    if(url->scheme() == "file")
                    {
                        if(!get_remaining().starts_with("//"))
                        {
                            report_validation_error();
                        }
                        state = State::File;
                    }
                    else if(url->is_special())
                    {
                        if(base_url && base_url->m_scheme == url->m_scheme)
                            state = State::SpecialRelativeOrAuthority;
                        else
                            state = State::SpecialAuthoritySlashes;
                    }
                    else if(get_remaining().starts_with("/"))
                    {
                        state = State::PathOrAuthority;
                        ++iterator;
                    }
                    else
                    {
                        url->m_cannot_be_a_base_url = true;
                        url->append_path("");
                        state = State::CannotBeABaseUrlPath;
                    }
                }
                else
                {
                    buffer.clear();
                    state = State::NoScheme;
                    iterator = input.begin();
                    continue;
                }
                break;
            case State::NoScheme:
                if(!base_url || (base_url->m_cannot_be_a_base_url && code_point != '#'))
                {
                    report_validation_error();
                    return {};
                }
                else if(base_url->m_cannot_be_a_base_url && code_point == '#')
                {
                    url->m_scheme = base_url->m_scheme;
                    url->m_paths = base_url->m_paths;
                    url->m_query = base_url->m_query;
                    url->m_fragment = "";
                    url->m_cannot_be_a_base_url = true;
                    state = State::Fragment;
                }
                else if(base_url->m_scheme != "file")
                {
                    state = State::Relative;
                    continue;
                }
                else
                {
                    state = State::File;
                    continue;
                }
                break;
            case State::SpecialRelativeOrAuthority:
                if(code_point == '/' && get_remaining().starts_with("/"))
                {
                    state = State::SpecialAuthorityIgnoreSlashes;
                    ++iterator;
                }
                else
                {
                    report_validation_error();
                    state = State::Relative;
                    continue;
                }
                break;
            case State::PathOrAuthority:
                if(code_point == '/')
                {
                    state = State::Authority;
                }
                else
                {
                    state = State::Path;
                    continue;
                }
                break;
            case State::Relative:
                url->m_scheme = base_url->m_scheme;
                if(code_point == '/')
                {
                    state = State::RelativeSlash;
                }
                else if(url->is_special() && code_point == '\\')
                {
                    report_validation_error();
                    state = State::RelativeSlash;
                }
                else
                {
                    url->m_username = base_url->m_username;
                    url->m_password = base_url->m_password;
                    url->m_host = base_url->m_host;
                    url->m_port = base_url->m_port;
                    url->m_paths = base_url->m_paths;
                    url->m_query = base_url->m_query;

                    if(code_point == '?')
                    {
                        url->m_query = "";
                        state = State::Query;
                    }
                    else if(code_point == '#')
                    {
                        url->m_fragment = "";
                        state = State::Fragment;
                    }
                    else if(code_point != end_of_file)
                    {
                        url->m_query = {};
                        if(url->m_paths.size())
                            url->m_paths.remove(url->m_paths.size() - 1);
                        state = State::Path;
                        continue;
                    }
                }
                break;
            case State::RelativeSlash:
                if(url->is_special() && (code_point == '/' || code_point == '\\'))
                {
                    if(code_point == '\\')
                        report_validation_error();
                    state = State::SpecialAuthorityIgnoreSlashes;
                }
                else if(code_point == '/')
                {
                    state = State::Authority;
                }
                else
                {
                    url->m_username = base_url->m_username;
                    url->m_password = base_url->m_password;
                    url->m_host = base_url->m_host;
                    url->m_port = base_url->m_port;
                    state = State::Path;
                    continue;
                }
                break;
            case State::SpecialAuthoritySlashes:
                if(code_point == '/' && get_remaining().starts_with("/"))
                {
                    state = State::SpecialAuthorityIgnoreSlashes;
                    ++iterator;
                }
                else
                {
                    report_validation_error();
                    state = State::SpecialAuthorityIgnoreSlashes;
                    continue;
                }
                break;
            case State::SpecialAuthorityIgnoreSlashes:
                if(code_point != '/' && code_point != '\\')
                {
                    state = State::Authority;
                    continue;
                }
                else
                {
                    report_validation_error();
                }
                break;
            case State::Authority:
                if(code_point == '@')
                {
                    report_validation_error();
                    if(at_sign_seen)
                    {
                        auto content = buffer.to_string();
                        buffer.clear();
                        buffer.append("%40");
                        buffer.append(content);
                    }
                    at_sign_seen = true;
                    StringBuilder builder;
                    for(auto c : Utf8View(builder.string_view()))
                    {
                        if(c == ':' && !password_token_seen)
                        {
                            password_token_seen = true;
                            continue;
                        }
                        builder.clear();
                        if(password_token_seen)
                        {
                            builder.append(url->password());
                            URL::append_percent_encoded_if_necessary(builder, c, URL::PercentEncodeSet::Userinfo);
                            
                            url->m_password = URL::percent_decode(builder.string_view());
                        }
                        else
                        {
                            builder.append(url->username());
                            URL::append_percent_encoded_if_necessary(builder, c, URL::PercentEncodeSet::Userinfo);

                            url->m_username = URL::percent_decode(builder.string_view());
                        }
                    }
                    buffer.clear();
                }
                else if(code_point == end_of_file || code_point == '/' || code_point == '?' || code_point == '#' || (url->is_special() && code_point == '\\'))
                {
                    if(at_sign_seen && buffer.is_empty())
                    {
                        report_validation_error();
                        return {};
                    }

                    iterator = input.iterator_at_byte_offset(iterator - input.begin() - buffer.length() - 1);
                    buffer.clear();
                    state = State::Host;
                }
                else
                {
                    buffer.append_code_point(code_point);
                }
                break;
            case State::Host:
            case State::Hostname:
                if(code_point == ':' && !inside_brackets)
                {
                    if(buffer.is_empty())
                    {
                        report_validation_error();
                        return {};
                    }
                    auto host = parse_host(buffer.string_view(), !url->is_special());
                    if(!host.has_value())
                        return {};
                    url->m_host = host.release_value();
                    buffer.clear();
                    state = State::Port;
                }
                else if(code_point == end_of_file || code_point == '/' || code_point == '?' || code_point == '#' || (url->is_special() && code_point == '\\'))
                {
                    if(url->is_special() && buffer.is_empty())
                    {
                        report_validation_error();
                        return {};
                    }
                    auto host = parse_host(buffer.string_view(), !url->is_special());
                    if(!host.has_value())
                        return {};
                    url->m_host = host.value();
                    buffer.clear();
                    state = State::Port;
                    continue;
                }
                else if(code_point == '[')
                {
                    inside_brackets = true;
                }
                else if(code_point == ']')
                {
                    inside_brackets = false;
                }
                else
                {
                    buffer.append_code_point(code_point);
                }
                break;
            case State::Port:
                if(is_ascii_digit(code_point))
                {
                    buffer.append_code_point(code_point);
                }
                else if(code_point == end_of_file || code_point == '/' || code_point == '?' || code_point == '#' || (url->is_special() && code_point == '\\'))
                {
                    if(!buffer.is_empty())
                    {
                        auto port = buffer.string_view().to_uint();
                        if(!port.has_value() || port.value() > 65535)
                        {
                            report_validation_error();
                            return {};
                        }
                        if(port.value() == URL::default_port_for_scheme(url->scheme()))
                            url->m_port = {};
                        else
                            url->m_port = port.value();
                        buffer.clear();
                    }
                    state = State::PathStart;
                    continue;
                }
                else
                {
                    report_validation_error();
                    return {};
                }
                break;
            case State::File:
                url->m_scheme = "file";
                url->m_host = "";
                if(code_point == '/' || code_point == '\\')
                {
                    if(code_point == '\\')
                        report_validation_error();
                    state = State::FileSlash;
                }
                else if(base_url && base_url->m_scheme == "file")
                {
                    url->m_host = base_url->m_host;
                    url->m_paths = base_url->m_paths;
                    url->m_query = base_url->m_query;
                    if(code_point == '?')
                    {
                        url->m_query = "";
                        state = State::Query;
                    }
                    else if(code_point == '#')
                    {
                        url->m_fragment = "";
                        state = State::Fragment;
                    }
                    else if(code_point != end_of_file)
                    {
                        url->m_query = {};
                        auto substring_from_pointer = input.substring_view(iterator - input.begin()).as_string();
                        if(!starts_with_windows_drive_letter(substring_from_pointer))
                        {
                            if(!url->paths().is_empty() && !(url->scheme() == "file" && url->paths().size() == 1 && is_normalized_windows_drive_letter(url->paths()[0])))
                                url->m_paths.remove(url->m_paths.size() - 1);
                        }
                        else
                        {
                            report_validation_error();
                            url->m_paths.clear();
                        }
                        state = State::Path;
                        continue;
                    }
                }
                break;
            case State::FileSlash:
                if(code_point == '/' || code_point == '\\')
                {
                    if(code_point == '\\')
                        report_validation_error();
                    state = State::FileHost;
                }
                else if(base_url && base_url->m_scheme == "file")
                {
                    url->m_host = base_url->m_host;
                    auto substring_from_pointer = input.substring_view(iterator - input.begin()).as_string();
                    if(!starts_with_windows_drive_letter(substring_from_pointer) && is_normalized_windows_drive_letter(base_url->m_paths[0]))
                        url->append_path(base_url->m_paths[0]);
                    state = State::Path;
                    continue;
                }
                break;
            case State::FileHost:
                if(code_point == end_of_file || code_point == '/' || code_point == '\\' || code_point == '?' || code_point == '#')
                {
                    if(is_windows_drive_letter(buffer.string_view()))
                    {
                        report_validation_error();
                        state = State::Path;
                    }
                    else if(buffer.is_empty())
                    {
                        url->m_host = "";
                        state = State::PathStart;
                    }
                    else
                    {
                        auto host = parse_host(buffer.string_view(), true);
                        if(!host.has_value())
                            return {};
                        if(host.value() == "localhost")
                            host = "";
                        url->m_host = host.release_value();
                        buffer.clear();
                        state = State::PathStart;
                    }
                    continue;
                }
                else
                {
                    buffer.append_code_point(code_point);
                }
                break;
            case State::PathStart:
                if(url->is_special())
                {
                    if(code_point == '\\')
                        report_validation_error();
                    state = State::Path;
                    if(code_point != '/' && code_point != '\\')
                        continue;
                }
                else if(code_point == '?')
                {
                    url->m_query = "";
                    state = State::Query;
                }
                else if(code_point == '#')
                {
                    url->m_fragment = "";
                    state = State::Fragment;
                }
                else if(code_point != end_of_file)
                {
                    state = State::Path;
                    if(code_point != '/')
                        continue;
                }
                break;
            case State::Path:
                if(code_point == end_of_file || code_point == '/' || (url->is_special() && code_point == '\\') || code_point == '?' || code_point == '#')
                {
                    if(url->is_special() && code_point == '\\')
                        report_validation_error();
                    if(is_double_dot_path_segment(buffer.string_view()))
                    {
                        if(!url->m_paths.is_empty() && !(url->m_scheme == "file" && url->m_paths.size() == 1 && is_normalized_windows_drive_letter(url->m_paths[0])))
                            url->m_paths.remove(url->m_paths.size() - 1);
                        if(code_point != '/' && !(url->is_special() && code_point == '\\'))
                            url->append_path("");
                    }
                    else if(is_single_dot_path_segment(buffer.string_view()) && code_point != '/' && !(url->is_special() && code_point == '\\'))
                    {
                        url->append_path("");
                    }
                    else if(!is_single_dot_path_segment(buffer.string_view()))
                    {
                        if(url->m_scheme == "file" && url->m_paths.is_empty() && is_windows_drive_letter(buffer.string_view()))
                        {
                            auto drive_letter = buffer.string_view()[0];
                            buffer.clear();
                            buffer.append(drive_letter);
                            buffer.append(':');
                        }

                        url->append_path(URL::percent_decode(buffer.string_view()));
                    }
                    buffer.clear();
                    if(code_point == '?')
                    {
                        url->m_query = "";
                        state = State::Query;
                    }
                    else if(code_point == '#')
                    {
                        url->m_fragment = "";
                        state = State::Fragment;
                    }
                }
                else
                {
                    if(!is_url_code_point(code_point) && code_point != '%')
                        report_validation_error();
                    
                    URL::append_percent_encoded_if_necessary(buffer, code_point, URL::PercentEncodeSet::Path);
                }
                break;
            case State::CannotBeABaseUrlPath:
                VERIFY(url->m_paths.size() == 1 && url->m_paths[0].is_empty());
                if(code_point == '?')
                {
                    url->m_paths[0] = URL::percent_decode(buffer.string_view());
                    url->m_query = "";
                    state = State::Query;
                }
                else if(code_point == '#')
                {
                    url->m_paths[0] = URL::percent_decode(buffer.string_view());
                    url->m_fragment = "";
                    state = State::Fragment;
                }
                else
                {
                    if(code_point != end_of_file && !is_url_code_point(code_point) && code_point != '%')
                        report_validation_error();

                    if(code_point != end_of_file)
                    {
                        URL::append_percent_encoded_if_necessary(buffer, code_point, URL::PercentEncodeSet::C0Control);
                    }
                    else
                    {
                        url->m_paths[0] = URL::percent_decode(buffer.string_view());
                    }
                }
                break;
            case State::Query:
                if(code_point == end_of_file || code_point == '#')
                {
                    VERIFY(url->m_query == "");
                    auto query_percent_encode_set = url->is_special() ? URL::PercentEncodeSet::SpecialQuery : URL::PercentEncodeSet::Query;
                    url->m_query = percent_encode_after_encoding(buffer.string_view(), query_percent_encode_set);
                    buffer.clear();
                    if(code_point == '#')
                    {
                        url->m_fragment = "";
                        state = State::Fragment;
                    }
                }
                else if(code_point != end_of_file)
                {
                    if(!is_url_code_point(code_point) && code_point != '%')
                        report_validation_error();

                    buffer.append_code_point(code_point);
                }
                break;
            case State::Fragment:
                if(code_point != end_of_file)
                {
                    if(!is_url_code_point(code_point) && code_point != '%')
                        report_validation_error();

                    buffer.append_code_point(code_point);
                }
                else
                {
                    url->m_fragment = URL::percent_decode(buffer.string_view());
                    buffer.clear();
                }
                break;
            default:
                VERIFY_NOT_REACHED();
            }

            if(iterator.done())
                break;
            ++iterator;
        }

        url->m_valid = true;
        dbgln_if(URL_PARSER_DEBUG, "URLParser::parse: Parsed URL to be '{}'.", url->serialize());
        return url.release_value();
    }

} // namespace Mods