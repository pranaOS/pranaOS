/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/HashMap.h>
#include <base/RefCounted.h>
#include <base/RefPtr.h>
#include <base/String.h>
#include <base/Vector.h>
#include <libgfx/Color.h>

namespace Core {

class ConfigFile : public RefCounted<ConfigFile> {
public:
    static NonnullRefPtr<ConfigFile> get_for_lib(const String& lib_name);
    static NonnullRefPtr<ConfigFile> get_for_app(const String& app_name);
    static NonnullRefPtr<ConfigFile> get_for_system(const String& app_name);
    static NonnullRefPtr<ConfigFile> open(const String& path);
    ~ConfigFile();

    bool has_group(const String&) const;
    bool has_key(const String& group, const String& key) const;

    Vector<String> groups() const;
    Vector<String> keys(const String& group) const;

    size_t num_groups() const { return m_groups.size(); }

    String read_entry(const String& group, const String& key, const String& default_value = String()) const;
    int read_num_entry(const String& group, const String& key, int default_value = 0) const;
    bool read_bool_entry(const String& group, const String& key, bool default_value = false) const;

    void write_entry(const String& group, const String& key, const String& value);
    void write_num_entry(const String& group, const String& key, int value);
    void write_bool_entry(const String& group, const String& key, bool value);
    void write_color_entry(const String& group, const String& key, Color value);

    void dump() const;

    bool is_dirty() const { return m_dirty; }

    bool sync();

    void remove_group(const String& group);
    void remove_entry(const String& group, const String& key);

    String filename() const { return m_filename; }

private:
    explicit ConfigFile(const String& filename);

    void reparse();

    String m_filename;
    HashMap<String, HashMap<String, String>> m_groups;
    bool m_dirty { false };
};

}
