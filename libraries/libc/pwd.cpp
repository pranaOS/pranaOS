/**
 * @file pwd.cpp
 * @author Krisna Pranav
 * @brief pwd
 * @version 6.0
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

    /// @brief: static functionalities
    static FILE* s_stream = nullptr;
    static unsigned s_line_number = 0;
    static struct passwd s_passwd_entry;
    static String s_name;
    static String s_passwd;
    static String s_gecos;
    static String s_dir;
    static String s_shell;

    /// @brief: setpwent
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
    }

    /// @brief: endpwent
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

    /**
     * @param name 
     * @return struct passwd* 
     */
    struct passwd* getpwnam(const char* name)
    {
        setpwent();

        while (auto* pw = getpwent()) {
            if (!strcmp(pw->pw_name, name))
                return pw;
        }

        return nullptr;
    }

    /**
     * @param line 
     * @return true 
     * @return false 
     */
    static bool parse_pwddb_entry(const String& line)
    {
        auto parts = line.split_view(':', true);

        if (parts.size() != 7) {
            fprintf(stderr, "getpwent: Malformed entry founded on line %u\n", s_line_number);
            return false;
        }

        s_name = parts[0];
        s_passwd = parts[1];
        auto& uid_string = parts[2];
        auto& gid_string = parts[3];
        s_gecos = parts[4];
        s_dir = parts[5];
        s_shell = parts[6];

        auto uid = uid_string.to_uint();

        if (!uid.has_value()) {
            fprintf(stderr, "getpwent: Malformed UID founded on line %u\n", s_line_number);
            return false;
        }

        auto gid = gid_string.to_uint();

        if (!gid.has_value()) {
            fprintf(stderr, "getpwent(): Malformed GID founded on line %u\n", s_line_number);
            return false;
        }

        s_passwd_entry.pw_name = const_cast<char*>(s_name.characters());
        s_passwd_entry.pw_passwd = const_cast<char*>(s_passwd.characters());
        s_passwd_entry.pw_uid = uid.value();
        s_passwd_entry.pw_gid = gid.value();
        s_passwd_entry.pw_gecos = const_cast<char*>(s_gecos.characters());
        s_passwd_entry.pw_dir = const_cast<char*>(s_dir.characters());
        s_passwd_entry.pw_shell = const_cast<char*>(s_shell.characters());

        return true;
    }

    /**
     * @return struct passwd* 
     */
    struct passwd* getpwent()
    {
        if (!s_stream)
            setpwent();

        while (true) {
            if (!s_stream || feof(s_stream))
                return nullptr;

            if (ferror(s_stream)) {
                fprintf(stderr, "getpwent(): Read error: %s\n", strerror(ferror(s_stream)));
                return nullptr;
            }

            char buffer[1024];
            ++s_line_number;
            char* s = fgets(buffer, sizeof(buffer), s_stream);

            if ((!s || !s[0]) && feof(s_stream))
                return nullptr;

            String line(s, Chomp);

            if (parse_pwddb_entry(line))
                return &s_passwd_entry;
        }
    }

    /**
     * @param p 
     * @param stream 
     * @return int 
     */
    int putpwent(const struct passwd* p, FILE* stream)
    {
        if (!p || !stream || !p->pw_passwd || !p->pw_name || !p->pw_dir || !p->pw_gecos || !p->pw_shell) {
            errno = EINVAL;
            return -1;
        }

        auto is_valid_field = [](const char* str) {
            return str && !strpbrk(str, ":\n");
        };

        if (!is_valid_field(p->pw_name) || !is_valid_field(p->pw_dir) || !is_valid_field(p->pw_gecos) || !is_valid_field(p->pw_shell)) {
            errno = EINVAL;
            return -1;
        }

        int nwritten = fprintf(stream, "%s:%s:%u:%u:%s,,,:%s:%s\n", p->pw_name, p->pw_passwd, p->pw_uid, p->pw_gid, p->pw_gecos, p->pw_dir, p->pw_shell);
        
        if (!nwritten || nwritten < 0) {
            errno = ferror(stream);
            return -1;
        }

        return 0;
    }
}
