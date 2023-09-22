/**
 * @file account.h
 * @author Krisna Pranav
 * @brief account
 * @version 6.0
 * @date 2023-09-22
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/result.h>
#include <mods/string.h>
#include <mods/vector.h>
#include <mods/types.h>
#include <pwd.h>
#include <sys/types.h>

namespace Core
{

    class Account
    {
    public:
        /**
         * @param username 
         * @return Result<Account, String> 
         */
        static Result<Account, String> from_name(const char* username);

        /**
         * @param uid 
         * @return Result<Account, String> 
         */
        static Result<Account, String> from_uid(uid_t uid);

        /**
         * @param password 
         * @return true 
         * @return false 
         */
        bool authenticate(const char* password) const;

        bool login() const;

        /**
         * @return String 
         */
        String username() const
        {
            return m_username;
        }

        String password_hash() const
        {
            return m_password_hash;
        }

        uid_t uid() const
        {
            return m_uid;
        }

        gid_t gid() const
        {
            return m_gid;
        }

    private:
        /**
         * @param pwd 
         * @param extra_gids 
         */
        Account(struct passwd* pwd, Vector<gid_t> extra_gids);

        String m_username;
        String m_password_hash;
        String m_gecos;
        String m_home_directory;
        String m_shell;

        Vector<gid_t> m_extra_gids;

        uid_t m_uid { 0 };
        gid_t m_gid { 0 };
    }; // class Account

} // namespace Core