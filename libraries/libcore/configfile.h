/**
 * @file configfile.h
 * @author Krisna Pranav
 * @brief config file
 * @version 6.0
 * @date 2025-02-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>
#include <mods/hashMap.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/string.h>
#include <mods/vector.h>
#include <libcore/stream.h>
#include <libgfx/color.h>

namespace Core 
{

    class ConfigFile : public RefCounted<ConfigFile> 
    {
    public:
        enum class AllowWriting 
        {
            Yes,
            No,
        }; // enum class AllowWriting 

        /**
         * @param lib_name 
         * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
         */
        static ErrorOr<NonnullRefPtr<ConfigFile>> open_for_lib(String const& lib_name, AllowWriting = AllowWriting::No);

        /**
         * @param app_name 
         * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
         */
        static ErrorOr<NonnullRefPtr<ConfigFile>> open_for_app(String const& app_name, AllowWriting = AllowWriting::No);

        /**
         * @param app_name 
         * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
         */
        static ErrorOr<NonnullRefPtr<ConfigFile>> open_for_system(String const& app_name, AllowWriting = AllowWriting::No);

        /**
         * @param filename 
         * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
         */
        static ErrorOr<NonnullRefPtr<ConfigFile>> open(String const& filename, AllowWriting = AllowWriting::No);

        /**
         * @param filename 
         * @param fd 
         * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
         */
        static ErrorOr<NonnullRefPtr<ConfigFile>> open(String const& filename, int fd);
        
        /**
         * @brief Destroy the ConfigFile object
         * 
         */
        ~ConfigFile();

        /**
         * @return true 
         * @return false 
         */
        bool has_group(String const&) const;

        /**
         * @param group 
         * @param key 
         * @return true 
         * @return false 
         */
        bool has_key(String const& group, String const& key) const;

        /**
         * @return Vector<String> 
         */
        Vector<String> groups() const;

        /**
         * @param group 
         * @return Vector<String> 
         */
        Vector<String> keys(String const& group) const;

        /**
         * @return size_t 
         */
        size_t num_groups() const 
        { 
            return m_groups.size(); 
        }

        /**
         * @param group 
         * @param key 
         * @param default_value 
         * @return String 
         */
        String read_entry(String const& group, String const& key, String const& default_value = String()) const;

        /**
         * @param group 
         * @param key 
         * @param default_value 
         * @return int 
         */
        int read_num_entry(String const& group, String const& key, int default_value = 0) const;

        /**
         * @param group 
         * @param key 
         * @param default_value 
         * @return true 
         * @return false 
         */
        bool read_bool_entry(String const& group, String const& key, bool default_value = false) const;

        /**
         * @param group 
         * @param key 
         * @param value 
         */
        void write_entry(String const& group, String const& key, String const& value);

        /**
         * @param group 
         * @param key 
         * @param value 
         */
        void write_num_entry(String const& group, String const& key, int value);

        /**
         * @param group 
         * @param key 
         * @param value 
         */
        void write_bool_entry(String const& group, String const& key, bool value);

        /**
         * @param group 
         * @param key 
         * @param value 
         */
        void write_color_entry(String const& group, String const& key, Color value);

        void dump() const;

        /**
         * @return true 
         * @return false 
         */
        bool is_dirty() const 
        { 
            return m_dirty; 
        }

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> sync();

        /**
         * @param group 
         */
        void remove_group(String const& group);

        /**
         * @param group 
         * @param key 
         */
        void remove_entry(String const& group, String const& key);

        /**
         * @return String const& 
         */
        String const& filename() const 
        { 
            return m_filename; 
        }

    private:
        /**
         * @brief Construct a new ConfigFile object
         * 
         * @param filename 
         * @param open_file 
         */
        ConfigFile(String const& filename, OwnPtr<Stream::BufferedFile> open_file);

        ErrorOr<void> reparse();

        String m_filename;
        OwnPtr<Stream::BufferedFile> m_file;
        HashMap<String, HashMap<String, String>> m_groups;
        bool m_dirty { false };
    }; // class ConfigFile : public RefCounted<ConfigFile>

} // namespace Core
