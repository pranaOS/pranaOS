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
} // namespace Core 