/*
 * Copyright (c) 2021, Krisna Pranav
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

}
