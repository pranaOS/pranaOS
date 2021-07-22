/*
 * Copyright (c) 2021, Krisna Pranav, Alex5xt
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/LexicalPath.h>
#include <base/StringBuilder.h>
#include <base/StringView.h>
#include <base/Vector.h>

namespace Base {

char s_single_dot = '.';

LexicalPath::LexicalPath(String path)
    : m_string(canonicalized_path(move(path)))
{
    if (m_string.is_empty()) {
        m_string = ".";
        m_dirname = m_string;
        m_basename = {};
        m_title = {};
        m_extension = {};
        m_parts.clear();
        return;
    }

    m_parts = m_string.split_view('/');

    auto last_slash_index = m_string.view().find_last('/');
    if (!last_slash_index.has_value()) {
        m_dirname = { &s_single_dot, 1 };
    } else if (*last_slash_index == 0) {
        m_dirname = m_string.substring_view(0, 1);
    } else {
        m_dirname = m_string.substring_view(0, *last_slash_index);
    }

    if (m_string == "/")
        m_basename = m_string;
    else {
        VERIFY(m_parts.size() > 0);
        m_basename = m_parts.last();
    }

    auto last_dot_index = m_basename.find_last('.');
    if (last_dot_index.has_value() && *last_dot_index != 0) {
        m_title = m_basename.substring_view(0, *last_dot_index);
        m_extension = m_basename.substring_view(*last_dot_index + 1);
    } else {
        m_title = m_basename;
        m_extension = {};
    }
}

Vector<String> LexicalPath::parts() const
{
    Vector<String> vector;
    vector.ensure_capacity(m_parts.size());
    for (auto& part : m_parts)
        vector.unchecked_append(part);
    return vector;
}

bool LexicalPath::has_extension(StringView const& extension) const
{
    return m_string.ends_with(extension, CaseSensitivity::CaseInsensitive);
}

String LexicalPath::canonicalized_path(String path)
{
    if (path.is_null())
        return {};

    if (path.is_empty())
        return ".";

    if (!path.contains("."sv) && !path.contains("//"sv) && !path.ends_with('/'))
        return path;

    auto is_absolute = path[0] == '/';
    auto parts = path.split_view('/');
    size_t approximate_canonical_length = 0;
    Vector<String> canonical_parts;

    for (auto& part : parts) {
        if (part == ".")
            continue;
        if (part == "..") {
            if (canonical_parts.is_empty()) {
                if (is_absolute) {
                    continue;
                }
            } else {
                if (canonical_parts.last() != "..") {
                    canonical_parts.take_last();
                    continue;
                }
            }
        }
        approximate_canonical_length += part.length() + 1;
        canonical_parts.append(part);
    }

    if (canonical_parts.is_empty() && !is_absolute)
        canonical_parts.append(".");

    StringBuilder builder(approximate_canonical_length);
    if (is_absolute)
        builder.append('/');
    builder.join('/', canonical_parts);
    return builder.to_string();
}

String LexicalPath::relative_path(StringView const& a_path, StringView const& a_prefix)
{
    if (!a_path.starts_with('/') || !a_prefix.starts_with('/')) {
        return {};
    }

    if (a_path == a_prefix)
        return ".";

    auto path = canonicalized_path(a_path);
    auto prefix = canonicalized_path(a_prefix);

    if (path == prefix)
        return ".";

    if (prefix == "/"sv)
        return path.substring_view(1);

    if (path.starts_with(prefix) && path[prefix.length()] == '/') {
        return path.substring_view(prefix.length() + 1);
    }

    return path;
}


}
