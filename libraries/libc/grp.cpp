/**
 * @file grp.cpp
 * @author Krisna Pranav
 * @brief GRP
 * @version 6.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/string.h>
#include <mods/vector.h>
#include <errno.h>
#include <errno_codes.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern "C" {

static FILE* s_stream = nullptr;
static unsigned s_line_number = 0;
static struct group s_group;

static String s_name;
static String s_passwd;
static Vector<String> s_members;
static Vector<char const*> s_members_ptrs;

void setgrent()
{
    s_line_number = 0;
    if (s_stream) {
        rewind(s_stream);
    } else {
        s_stream = fopen("/etc/group", "r");
        if (!s_stream) {
            perror("open /etc/group");
        }
    }
}

void endgrent()
{
    s_line_number = 0;
    if (s_stream) {
        fclose(s_stream);
        s_stream = nullptr;
    }

    memset(&s_group, 0, sizeof(s_group));

    s_name = {};
    s_passwd = {};
    s_members = {};
    s_members_ptrs = {};
}

/**
 * @param gid 
 * @return struct group* 
 */
struct group* getgrgid(gid_t gid)
{
    setgrent();
    while (auto* gr = getgrent()) {
        if (gr->gr_gid == gid)
            return gr;
    }
    return nullptr;
}

/**
 * @param name 
 * @return struct group* 
 */
struct group* getgrnam(char const* name)
{
    setgrent();
    while (auto* gr = getgrent()) {
        if (!strcmp(gr->gr_name, name))
            return gr;
    }
    return nullptr;
}

/**
 * @param line 
 * @return true 
 * @return false 
 */
static bool parse_grpdb_entry(String const& line)
{
    auto parts = line.split_view(':', true);
    if (parts.size() != 4) {
        warnln("getgrent(): Malformed entry on line {}: '{}' has {} parts", s_line_number, line, parts.size());
        return false;
    }

    s_name = parts[0];
    s_passwd = parts[1];

    auto& gid_string = parts[2];
    String members_string = parts[3];

    auto gid = gid_string.to_uint();
    if (!gid.has_value()) {
        warnln("getgrent(): Malformed GID on line {}", s_line_number);
        return false;
    }

    s_members = members_string.split(',');
    s_members_ptrs.clear_with_capacity();
    s_members_ptrs.ensure_capacity(s_members.size() + 1);
    for (auto& member : s_members) {
        s_members_ptrs.append(member.characters());
    }
    s_members_ptrs.append(nullptr);

    s_group.gr_gid = gid.value();
    s_group.gr_name = const_cast<char*>(s_name.characters());
    s_group.gr_passwd = const_cast<char*>(s_passwd.characters());
    s_group.gr_mem = const_cast<char**>(s_members_ptrs.data());

    return true;
}

/**
 * @return struct group* 
 */
struct group* getgrent()
{
    if (!s_stream)
        setgrent();

    while (true) {
        if (!s_stream || feof(s_stream))
            return nullptr;

        if (ferror(s_stream)) {
            warnln("getgrent(): Read error: {}", strerror(ferror(s_stream)));
            return nullptr;
        }

        char buffer[1024];
        ++s_line_number;
        char* s = fgets(buffer, sizeof(buffer), s_stream);

        if ((!s || !s[0]) && feof(s_stream))
            return nullptr;

        String line(s, Chomp);
        if (parse_grpdb_entry(line))
            return &s_group;
    }
}

/**
 * @param user 
 * @param extra_gid 
 * @return int 
 */
int initgroups(char const* user, gid_t extra_gid)
{
    size_t count = 0;
    gid_t gids[32];
    bool extra_gid_added = false;
    setgrent();
    while (auto* gr = getgrent()) {
        for (auto* mem = gr->gr_mem; *mem; ++mem) {
            if (!strcmp(*mem, user)) {
                gids[count++] = gr->gr_gid;
                if (gr->gr_gid == extra_gid)
                    extra_gid_added = true;
                break;
            }
        }
    }
    endgrent();
    if (!extra_gid_added)
        gids[count++] = extra_gid;
    return setgroups(count, gids);
}

/**
 * @param group 
 * @param stream 
 * @return int 
 */
int putgrent(const struct group* group, FILE* stream)
{
    if (!group || !stream || !group->gr_name || !group->gr_passwd) {
        errno = EINVAL;
        return -1;
    }

    auto is_valid_field = [](char const* str) {
        return str && !strpbrk(str, ":\n");
    };

    if (!is_valid_field(group->gr_name) || !is_valid_field(group->gr_passwd)) {
        errno = EINVAL;
        return -1;
    }

    int nwritten = fprintf(stream, "%s:%s:%u:", group->gr_name, group->gr_passwd, group->gr_gid);
    if (!nwritten || nwritten < 0) {
        errno = ferror(stream);
        return -1;
    }

    if (group->gr_mem) {
        for (size_t i = 0; group->gr_mem[i] != nullptr; i++) {
            nwritten = fprintf(stream, i == 0 ? "%s" : ",%s", group->gr_mem[i]);
            if (!nwritten || nwritten < 0) {
                errno = ferror(stream);
                return -1;
            }
        }
    }

    nwritten = fprintf(stream, "\n");
    if (!nwritten || nwritten < 0) {
        errno = ferror(stream);
        return -1;
    }

    return 0;
}
}
