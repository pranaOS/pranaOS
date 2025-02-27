/**
 * @file configfile.cpp
 * @author Krisna Pranav
 * @brief config file
 * @version 6.0
 * @date 2025-02-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/lexicalpath.h>
#include <mods/stringbuilder.h>
#include <libcore/configfile.h>
#include <libcore/directory.h>
#include <libcore/standardpaths.h>
#include <libcore/system.h>
#include <pwd.h>
#include <sys/types.h>

namespace Core
{
    
    /**
     * @param lib_name 
     * @param allow_altering 
     * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
     */
    ErrorOr<NonnullRefPtr<ConfigFile>> ConfigFile::open_for_lib(String const& lib_name, AllowWriting allow_altering)
    {
        String directory_name = String::formatted("{}/lib", StandardPaths::config_directory());
        auto directory = TRY(Directory::create(directory_name, Directory::CreateDirectories::Yes));
        auto path = String::formatted("{}/{}.ini", directory, lib_name);

        return ConfigFile::open(path, allow_altering);
    }

    /**
     * @param app_name 
     * @param allow_altering 
     * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
     */
    ErrorOr<NonnullRefPtr<ConfigFile>> ConfigFile::open_for_app(String const& app_name, AllowWriting allow_altering)
    {
        auto directory = TRY(Directory::create(StandardPaths::config_directory(), Directory::CreateDirectories::Yes));
        auto path = String::formatted("{}/{}.ini", directory, app_name);

        return ConfigFile::open(path, allow_altering);
    }

    /**
     * @brief Construct a new ConfigFile::ConfigFile object
     * 
     * @param filename 
     * @param open_file 
     */
    ConfigFile::ConfigFile(String const& filename, OwnPtr<Stream::BufferedFile> open_file)
        : m_filename(filename)
        , m_file(move(open_file))
    {
    }

    /**
     * @brief Destroy the ConfigFile::ConfigFile object
     * 
     */
    ConfigFile::~ConfigFile()
    {
        MUST(sync());
    }

    /**
     * @return ErrorOr<void> 
     */
    ErrorOr<void> ConfigFile::reparse()
    {
        m_groups.clear();

        if (!m_file)
            return {};

        HashMap<String, String>* current_group = nullptr;

        auto buffer = TRY(ByteBuffer::create_uninitialized(4096));

        while (TRY(m_file->can_read_line())) {
            auto line = TRY(m_file->read_line(buffer));
            size_t i = 0;

            while (i < line.length() && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'))
                ++i;

            if (i >= line.length())
                continue;

            switch (line[i]) {
            case '#': 
            case ';': 
                continue;
            case '[': { 
                StringBuilder builder;
                ++i; 
                while (i < line.length() && (line[i] != ']')) {
                    builder.append(line[i]);
                    ++i;
                }
                current_group = &m_groups.ensure(builder.to_string());
                break;
            }
            default: { 
                StringBuilder key_builder;
                StringBuilder value_builder;
                while (i < line.length() && (line[i] != '=')) {
                    key_builder.append(line[i]);
                    ++i;
                }
                ++i; 
                while (i < line.length() && (line[i] != '\n')) {
                    value_builder.append(line[i]);
                    ++i;
                }
                if (!current_group) {
                    current_group = &m_groups.ensure("");
                }

                auto value_string = value_builder.to_string();
                current_group->set(key_builder.to_string(), value_string.trim_whitespace(TrimMode::Right));
            }
            }
        }
        return {};
    }

    /**
     * @param group 
     * @param key 
     * @param default_value 
     * @return String 
     */
    String ConfigFile::read_entry(String const& group, String const& key, String const& default_value) const
    {
        if (!has_key(group, key)) {
            return default_value;
        }
        
        auto it = m_groups.find(group);
        auto jt = it->value.find(key);
        return jt->value;
    }

} // namespace Core
