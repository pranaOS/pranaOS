/**
 * @file account.h
 * @author Krisna Pranav
 * @brief account
 * @version 6.0
 * @date 2023-09-22
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string.h>
#include <mods/types.h>
#include <mods/vector.h>
#include <libcore/secretstring.h>
#include <pwd.h>
#ifndef MODS_OS_BSD_GENERIC
#    include <shadow.h>
#endif
#include <sys/types.h>

namespace Core 
{

    #ifdef MODS_OS_BSD_GENERIC
    struct spwd 
    {
        char* sp_namp;
        char* sp_pwdp;
    }; // struct spwd 
    #endif

    class Account 
    {
    public:
        enum class Read 
        {
            All,
            PasswdOnly
        }; // enum class Read 

        /**
         * @param options 
         * @return ErrorOr<Account> 
         */
        static ErrorOr<Account> self(Read options = Read::All);

        /**
         * @param username 
         * @param options 
         * @return ErrorOr<Account> 
         */
        static ErrorOr<Account> from_name(char const* username, Read options = Read::All);

        /**
         * @param uid 
         * @param options 
         * @return ErrorOr<Account> 
         */
        static ErrorOr<Account> from_uid(uid_t uid, Read options = Read::All);

        /**
         * @param password 
         * @return true 
         * @return false 
         */
        bool authenticate(SecretString const& password) const;

        /**
         * @return true 
         * @return false 
         */
        bool login() const;

        /**
         * @return String 
         */
        String username() const 
        { 
            return m_username; 
        }

        /**
         * @return String 
         */
        String password_hash() const 
        { 
            return m_password_hash; 
        }

        /**
         * @brief Set the password object
         * 
         * @param password 
         */
        void set_password(SecretString const& password);

        /**
         * @brief Set the password enabled object
         * 
         * @param enabled 
         */
        void set_password_enabled(bool enabled);

        /**
         * @brief Set the home directory object
         * 
         * @param home_directory 
         */
        void set_home_directory(char const* home_directory) 
        { 
            m_home_directory = home_directory; 
        }

        /**
         * @brief Set the uid object
         * 
         * @param uid 
         */
        void set_uid(uid_t uid) 
        { 
            m_uid = uid; 
        }

        /**
         * @brief Set the gid object
         * 
         * @param gid 
         */
        void set_gid(gid_t gid) 
        { 
            m_gid = gid; 
        }

        /**
         * @brief Set the shell object
         * 
         * @param shell 
         */
        void set_shell(char const* shell) 
        { 
            m_shell = shell; 
        }

        /**
         * @brief Set the gecos object
         * 
         * @param gecos 
         */
        void set_gecos(char const* gecos) 
        { 
            m_gecos = gecos; 
        }

        void delete_password();

        /**
         * @return true 
         * @return false 
         */
        bool has_password() const 
        { 
            return !m_password_hash.is_empty() || m_password_hash.is_null(); 
        }

        /**
         * @return uid_t 
         */
        uid_t uid() const 
        { 
            return m_uid; 
        }

        /**
         * @return gid_t 
         */
        gid_t gid() const 
        { 
            return m_gid; 
        }

        /**
         * @return String const& 
         */
        String const& gecos() const 
        { 
            return m_gecos; 
        }

        /**
         * @return String const& 
         */
        String const& home_directory() const 
        { 
            return m_home_directory; 
        }

        /**
         * @return String const& 
         */
        String const& shell() const 
        { 
            return m_shell; 
        }

        /**
         * @return Vector<gid_t> const& 
         */
        Vector<gid_t> const& extra_gids() const 
        { 
            return m_extra_gids; 
        }

        /**
         * @return ErrorOr<void> 
         */
        ErrorOr<void> sync();

    private:
        /**
         * @return ErrorOr<Account> 
         */
        static ErrorOr<Account> from_passwd(passwd const&, spwd const&);

        /**
         * @brief Construct a new Account object
         * 
         * @param pwd 
         * @param spwd 
         * @param extra_gids 
         */
        Account(passwd const& pwd, spwd const& spwd, Vector<gid_t> extra_gids);

        /**
         * @return ErrorOr<String> 
         */
        ErrorOr<String> generate_passwd_file() const;
    #ifndef MODS_OS_BSD_GENERIC
        ErrorOr<String> generate_shadow_file() const;
    #endif

        String m_username;

        String m_password_hash;
        uid_t m_uid { 0 };
        gid_t m_gid { 0 };
        String m_gecos;
        String m_home_directory;
        String m_shell;
        Vector<gid_t> m_extra_gids;
    }; // class Account

} // namespace Core
