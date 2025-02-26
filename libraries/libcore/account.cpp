    /**
 * @file account.cpp
 * @author Krisna Pranav
 * @brief account
 * @version 6.0
 * @date 2025-02-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/base64.h>
#include <mods/memory.h>
#include <mods/random.h>
#include <mods/scopeguard.h>
#include <libcore/account.h>
#include <libcore/system.h>
#include <libcore/umaskscope.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#ifndef MODS_OS_BSD_GENERIC
#    include <crypt.h>
#    include <shadow.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

namespace Core 
{
    /**
     * @brief Get the salt object
     * 
     * @return String 
     */
    static String get_salt()
    {
        char random_data[12];
        fill_with_random(random_data, sizeof(random_data));

        StringBuilder builder;
        builder.append("$5$");
        builder.append(encode_base64(ReadonlyBytes(random_data, sizeof(random_data))));

        return builder.build();
    }

    /**
     * @brief Get the extra gids object
     * 
     * @param pwd 
     * @return Vector<gid_t> 
     */
    static Vector<gid_t> get_extra_gids(passwd const& pwd)
    {
        StringView username { pwd.pwd_name };
        Vector<gid_t> extra_gids;
        setgrent();
        for (auto* group = getgrent(); group; group = getgrent()) {
            if (group->gr_gid == pwd.pw_gid)
                continue;
            
            for (size_t i = 0; group->gr_mem[i]; ++i) {
                if (username == group->gr_mem[i]) {
                    extra_gids.append(group->gr_gid);
                    break;
                }
            }
        }

        endgrent();
        return extra_gids;
    }

    void Account::delete_password()
    {
        m_password_hash = "";
    }

    /**
     * @brief Construct a new Account::Account object
     * 
     * @param pwd 
     * @param spwd 
     * @param extra_gids 
     */
    Account::Account(passwd const& pwd, spwd const& spwd, Vector<gid_t> extra_gids)
        : m_username(pwd.pw_name)
        , m_password_hash(spwd.sp_pwdp)
        , m_uid(pwd.pw_uid)
        , m_gid(pwd.pw_gid)
        , m_gecos(pwd.pw_gecos)
        , m_home_directory(pwd.pw_dir)
        , m_shell(pwd.pw_shell)
        , m_extra_gids(move(extra_gids))
    {
    }

    /**
     * @return ErrorOr<String> 
     */
    ErrorOr<String> Account::generate_passwd_file() const
    {
        StringBuilder builder;

        setpwent();

        struct passwd* p;
        errno = 0;
        while ((p = getpwent())) {
            if (p->pw_name == m_username) {
                builder.appendff("{}:!:{}:{}:{}:{}:{}\n",
                    m_username,
                    m_uid, m_gid,
                    m_gecos,
                    m_home_directory,
                    m_shell);

            } else {
                builder.appendff("{}:!:{}:{}:{}:{}:{}\n",
                    p->pw_name, p->pw_uid,
                    p->pw_gid, p->pw_gecos, p->pw_dir,
                    p->pw_shell);
            }
        }
        endpwent();

        if (errno)
            return Error::from_errno(errno);

        return builder.to_string();
    }

} // namespace Core 