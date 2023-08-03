/**
 * @file pwd.cpp
 * @author Krisna Pranav
 * @brief pwd
 * @version 1.0
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <errno_codes.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mods/string.h>
#include <mods/vector.h>

extern "C" 
{
    static FILE* s_stream = nullptr;
    static unsigned s_line_number = 0;
    static struct passwd s_passwd_entry;
    
    static String s_name;
    static String s_passwd;
    static String s_gecos;
    static String s_dir;
    static String s_shell;

    void setpwent()
    {
        s_line_number = 0;

        if (s_stream) {
            rewind(s_stream);
        } else {
            s_stream = fopen("/etc/passwd", "r");

            if (!s_stream) {
                perror("open /etc/passwd");
            }
        }
    } // void setpwent

    void endpwent()
    {
        s_line_number = 0;

        if (s_stream) {
            fclose(s_stream);
            s_stream = nullptr;
        }

        memset(&s_passwd_entry, 0, sizeof(s_passwd_entry));

        s_name = {};
        s_passwd = {};
        s_gecos = {};
        s_dir = {};
        s_shell = {};
    }

    /**
     * @param uid 
     * @return struct passwd* 
     */
    struct passwd* getpwuid(uid_t uid)
    {
        setpwent();

        while (auto* pw = getpwent()) {
            if (pw->pw_uid == uid)
                return pw;
        }

        return nullptr;
    }
} // extern "C"