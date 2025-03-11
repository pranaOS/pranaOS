/**
 * @file tar.h
 * @author Krisna Pranav
 * @brief tar
 * @version 6.0
 * @date 2025-02-17
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/string.h>
#include <mods/stringview.h>
#include <string.h>
#include <sys/types.h>

namespace Archive
{

    enum class TarFileType : char
    {
        NormalFile = '0',
        AlternateNormalFile = '\0',
        HardLink = '1',
        SymLink = '2',
        CharacterSpecialFile = '3',
        BlockSpecialFile = '4',
        Directory = '5',
        FIFO = '6',
        ContiguousFile = '7',
        GlobalExtendedHeader = 'g',
        ExtendedHeader = 'x'
    }; // enum class TarFileType : char

    constexpr size_t block_size = 512;
    constexpr StringView gnu_magic = "ustar ";
    constexpr StringView gnu_version = " ";
    constexpr StringView ustar_magic = "ustar";
    constexpr StringView ustar_version = "00";
    constexpr StringView posix1_tar_magic = "";
    constexpr StringView posix1_tar_version = "";

    /**
     * @brief Get the field as integral object
     *
     * @tparam N
     * @return size_t
     */
    template <size_t N>
    static size_t get_field_as_integral(char const (&field)[N])
    {
        size_t value = 0;

        for(size_t i = 0; i < N; ++i)
        {
            if(field[i] == 0 || field[i] == ' ')
                break;

            VERIFY(field[i] >= '0' && field[i] <= '7');
            value *= 8;
            value += field[i] - '0';
        }

        return value;
    }

    /**
     * @brief Get the field as string view object
     *
     * @tparam N
     * @return StringView
     */
    template <size_t N>
    static StringView get_field_as_string_view(char const (&field)[N])
    {
        return {field, min(__builtin_strlen(field), N)};
    }

    /**
     * @brief Set the field object
     *
     * @tparam N
     * @tparam TSource
     * @param source
     */
    template <size_t N, class TSource>
    static void set_field(char (&field)[N], TSource&& source)
    {
        if constexpr(requires { source.characters_without_null_termination(); })
        {
            memcpy(field, source.characters_without_null_termination(), min(N, source.length()));
        }
        else
        {
            auto success = source.copy_characters_to_buffer(field, N);
            VERIFY(success);
        }
    }

    /**
     * @brief Set the octal field object
     *
     * @tparam TSource
     * @tparam N
     * @param source
     */
    template <class TSource, size_t N>
    static void set_octal_field(char (&field)[N], TSource&& source)
    {
        set_field(field, String::formatted("{:o}", forward<TSource>(source)));
    }

    class [[gnu::packed]] TarFileHeader
    {
    public:
        /**
         * @return StringView
         */
        StringView filename() const
        {
            return get_field_as_string_view(m_filename);
        }

        /**
         * @return mode_t
         */
        mode_t mode() const
        {
            return get_field_as_integral(m_mode);
        }

        /**
         * @return uid_t
         */
        uid_t uid() const
        {
            return get_field_as_integral(m_uid);
        }

        /**
         * @return gid_t 
         */
        gid_t gid() const
        {
            return get_field_as_integral(m_gid);
        }
        
        /**
         * @return size_t 
         */
        size_t size() const
        {
            return get_field_as_integral(m_size);
        }

        /**
         * @return time_t 
         */
        time_t timestamp() const
        {
            return get_field_as_integral(m_timestamp);
        }

        /**
         * @return unsigned 
         */
        unsigned checksum() const
        {
            return get_field_as_integral(m_checksum);
        }

        /**
         * @return TarFileType 
         */
        TarFileType type_flag() const
        {
            return TarFileType(m_type_flag);
        }

        /**
         * @return StringView 
         */
        StringView link_name() const
        {
            return m_link_name;
        }

        /**
         * @return StringView 
         */
        StringView magic() const
        {
            return get_field_as_string_view(m_magic);
        }

        /**
         * @return StringView 
         */
        StringView version() const
        {
            return get_field_as_string_view(m_version);
        }   

        /**
         * @return StringView 
         */
        StringView owner_name() const
        {
            return get_field_as_string_view(m_owner_name);
        }

        /**
         * @return StringView 
         */
        StringView group_name() const
        {
            return get_field_as_string_view(m_group_name);
        }

        /**
         * @return int 
         */
        int major() const
        {
            return get_field_as_integral(m_major);
        }

        /**
         * @return int 
         */
        int minor() const
        {
            return get_field_as_integral(m_minor);
        }
        
        /**
         * @return StringView 
         */
        StringView prefix() const
        {
            return get_field_as_string_view(m_prefix);
        }

        /**
         * @brief Set the filename object
         * 
         * @param filename 
         */
        void set_filename(String const& filename)
        {
            set_field(m_filename, filename);
        }

        /**
         * @brief Set the mode object
         * 
         * @param mode 
         */
        void set_mode(mode_t mode)
        {
            set_octal_field(m_mode, mode);
        }

        /**
         * @brief Set the uid object
         * 
         * @param uid 
         */
        void set_uid(uid_t uid)
        {
            set_octal_field(m_uid, uid);
        }

        /**
         * @brief Set the gid object
         * 
         * @param gid 
         */
        void set_gid(gid_t gid)
        {
            set_octal_field(m_gid, gid);
        }

        /**
         * @brief Set the size object
         * 
         * @param size 
         */
        void set_size(size_t size)
        {
            set_octal_field(m_size, size);
        }

        /**
         * @brief Set the timestamp object
         * 
         * @param timestamp 
         */
        void set_timestamp(time_t timestamp)
        {
            set_octal_field(m_timestamp, timestamp);
        }

        /**
         * @brief Set the type flag object
         * 
         * @param type 
         */
        void set_type_flag(TarFileType type)
        {
            m_type_flag = to_underlying(type);
        }

        /**
         * @brief Set the link name object
         * 
         * @param link_name 
         */
        void set_link_name(String const& link_name)
        {
            set_field(m_link_name, link_name);
        }
        
        /**
         * @brief Set the magic object
         * 
         * @param magic 
         */
        void set_magic(StringView magic)
        {
            set_field(m_magic, magic);
        }
        
        /**
         * @brief Set the version object
         * 
         * @param version 
         */
        void set_version(StringView version)
        {
            set_field(m_version, version);
        }

        /**
         * @brief Set the owner name object
         * 
         * @param owner_name 
         */
        void set_owner_name(String const& owner_name)
        {
            set_field(m_owner_name, owner_name);
        }

        /**
         * @brief Set the group name object
         * 
         * @param group_name 
         */
        void set_group_name(String const& group_name)
        {
            set_field(m_group_name, group_name);
        }

        /**
         * @brief Set the major object
         * 
         * @param major 
         */
        void set_major(int major)
        {
            set_octal_field(m_major, major);
        }

        /**
         * @brief Set the minor object
         * 
         * @param minor 
         */
        void set_minor(int minor)
        {
            set_octal_field(m_minor, minor);
        }

        /**
         * @brief Set the prefix object
         * 
         * @param prefix 
         */
        void set_prefix(String const& prefix)
        {
            set_field(m_prefix, prefix);
        }

        unsigned expected_checksum() const;
        void calculate_checksum();

        bool content_is_like_extended_header() const;

    private:
        char m_filename[100]{0};
        char m_mode[8]{0};
        char m_uid[8]{0};
        char m_gid[8]{0};
        char m_size[12]{0};
        char m_timestamp[12]{0};
        char m_checksum[8]{0};
        char m_type_flag{0};
        char m_link_name[100]{0};
        char m_magic[6]{0};
        char m_version[2]{0};
        char m_owner_name[32]{0};
        char m_group_name[32]{0};
        char m_major[8]{0};
        char m_minor[8]{0};
        char m_prefix[155]{0}; 
    }; // class [[gnu::packed]] TarFileHeader

} // namespace Archive
