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
     * @param app_name 
     * @param allow_altering 
     * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
     */
    ErrorOr<NonnullRefPtr<ConfigFile>> ConfigFile::open_for_system(String const& app_name, AllowWriting allow_altering)
    {
        auto path = String::formatted("/etc/{}.ini", app_name);

        return ConfigFile::open(path, allow_altering);
    }

    /**
     * @param filename 
     * @param allow_altering 
     * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
     */
    ErrorOr<NonnullRefPtr<ConfigFile>> ConfigFile::open(String const& filename, AllowWriting allow_altering)
    {
        auto maybe_file = Stream::File::open(filename, allow_altering == AllowWriting::Yes ? Stream::OpenMode::ReadWrite : Stream::OpenMode::Read);
        OwnPtr<Stream::BufferedFile> buffered_file;

        if (maybe_file.is_error()) {
            if (!(allow_altering == AllowWriting::No && maybe_file.error().code() == ENOENT))
                return maybe_file.error();
        } else {
            buffered_file = TRY(Stream::BufferedFile::create(maybe_file.release_value()));
        }

        auto config_file = TRY(adopt_nonnull_ref_or_enomem(new (nothrow) ConfigFile(filename, move(buffered_file))));
        TRY(config_file->reparse());
        
        return config_file;
    }

    /**
     * @param filename 
     * @param fd 
     * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
     */
    ErrorOr<NonnullRefPtr<ConfigFile>> ConfigFile::open(String const& filename, int fd)
    {
        auto file = TRY(Stream::File::adopt_fd(fd, Stream::OpenMode::ReadWrite));
        auto buffered_file = TRY(Stream::BufferedFile::create(move(file)));

        auto config_file = TRY(adopt_nonnull_ref_or_enomem(new (nothrow) ConfigFile(filename, move(buffered_file))));
        TRY(config_file->reparse());

        return config_file;
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

    /**
     * @param group 
     * @param key 
     * @param default_value 
     * @return int 
     */
    int ConfigFile::read_num_entry(String const& group, String const& key, int default_value) const
    {
        if (!has_key(group, key)) {
            return default_value;
        }

        return read_entry(group, key).to_int().value_or(default_value);
    }

    /**
     * @param group 
     * @param key 
     * @param default_value 
     * @return true 
     * @return false 
     */
    bool ConfigFile::read_bool_entry(String const& group, String const& key, bool default_value) const
    {
        auto value = read_entry(group, key, default_value ? "true" : "false");
        return value == "1" || value.equals_ignoring_case("true"sv);
    }

    /**
     * @param group 
     * @param key 
     * @param value 
     */
    void ConfigFile::write_entry(String const& group, String const& key, String const& value)
    {
        m_groups.ensure(group).ensure(key) = value;
        m_dirty = true;
    }

    /**
     * @param group 
     * @param key 
     * @param value 
     */
    void ConfigFile::write_num_entry(String const& group, String const& key, int value)
    {
        write_entry(group, key, String::number(value));
    }

    /**
     * @param group 
     * @param key 
     * @param value 
     */
    void ConfigFile::write_bool_entry(String const& group, String const& key, bool value)
    {
        write_entry(group, key, value ? "true" : "false");
    }

    /**
     * @param group 
     * @param key 
     * @param value 
     */
    void ConfigFile::write_color_entry(String const& group, String const& key, Color value)
    {
        write_entry(group, key, String::formatted("{},{},{},{}", value.red(), value.green(), value.blue(), value.alpha()));
    }

    /**
     * @return ErrorOr<void> 
     */
    ErrorOr<void> ConfigFile::sync()
    {
        if (!m_dirty)
            return {};

        if (!m_file)
            return Error::from_errno(ENOENT);

        TRY(m_file->truncate(0));
        TRY(m_file->seek(0, Stream::SeekMode::SetPosition));

        for (auto& it : m_groups) {
            TRY(m_file->write(String::formatted("[{}]\n", it.key).bytes()));
            for (auto& jt : it.value)
                TRY(m_file->write(String::formatted("{}={}\n", jt.key, jt.value).bytes()));
            TRY(m_file->write("\n"sv.bytes()));
        }

        m_dirty = false;
        return {};
    }

    void ConfigFile::dump() const
    {
        for (auto& it : m_groups) {
            outln("[{}]", it.key);

            for (auto& jt : it.value)
                outln("{}={}", jt.key, jt.value);

            outln();
        }
    }

    /**
     * @return Vector<String> 
     */
    Vector<String> ConfigFile::groups() const
    {
        return m_groups.keys();
    }

    /**
     * @param group 
     * @return Vector<String> 
     */
    Vector<String> ConfigFile::keys(String const& group) const
    {
        auto it = m_groups.find(group);

        if (it == m_groups.end())
            return {};

        return it->value.keys();
    }

    /**
     * @param group 
     * @param key 
     * @return true 
     * @return false 
     */
    bool ConfigFile::has_key(String const& group, String const& key) const
    {
        auto it = m_groups.find(group);

        if (it == m_groups.end())
            return {};

        return it->value.contains(key);
    }

    /**
     * @param group 
     * @return true 
     * @return false 
     */
    bool ConfigFile::has_group(String const& group) const
    {
        return m_groups.contains(group);
    }

    /**
     * @param group 
     */
    void ConfigFile::remove_group(String const& group)
    {
        m_groups.remove(group);
        m_dirty = true;
    }   

    /**
     * @param group 
     * @param key 
     */
    void ConfigFile::remove_entry(String const& group, String const& key)
    {
        auto it = m_groups.find(group);
        if (it == m_groups.end())
            return;
        it->value.remove(key);
        m_dirty = true;
    }

} // namespace Core
