/**
 * @file group.cpp
 * @author Krisna Pranav
 * @brief group
 * @version 6.0 
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/charactertypes.h>
#include <mods/scopeguard.h>
#include <libcore/group.h>
#include <libcore/system.h>

namespace Core 
{

    #ifndef MOD_OS_BSD_GENERIC
    /**
     * @param group 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Group::add_group(Group& group)
    {
        if (group.name().is_empty())
            return Error::from_string_literal("Group name can not be empty.");

        if (strpbrk(group.name().characters(), "\\/!@#$%^&*()~+=`:\n"))
            return Error::from_string_literal("Group name has invalid characters.");

        if (group.name().starts_with('_') || group.name().starts_with('-') || !is_ascii_alpha(group.name().characters()[0]))
            return Error::from_string_literal("Group name has invalid characters.");

        if (TRY(name_exists(group.name())))
            return Error::from_string_literal("Group name already exists.");

        if (group.id() > 0) {
            if (TRY(id_exists(group.id())))
                return Error::from_string_literal("Group ID already exists.");
        } else {
            gid_t group_id = 100;
            while (true) {
                if (!TRY(id_exists(group_id)))
                    break;
                group_id++;
            }
            group.set_group_id(group_id);
        }

        auto gr = TRY(group.to_libc_group());

        FILE* file = fopen("/etc/group", "a");
        if (!file)
            return Error::from_errno(errno);

        ScopeGuard file_guard { [&] {
            fclose(file);
        } };

        if (putgrent(&gr, file) < 0)
            return Error::from_errno(errno);

        return {};
    }
    #endif

    /**
     * @brief Construct a new Group::Group object
     * 
     * @param name 
     * @param id 
     * @param members 
     */
    Group::Group(String name, gid_t id, Vector<String> members)
        : m_name(move(name))
        , m_id(id)
        , m_members(move(members))
    {
    }

    /**
     * @param name 
     * @return ErrorOr<bool> 
     */
    ErrorOr<bool> Group::name_exists(StringView name)
    {
        return TRY(Core::System::getgrnam(name)).has_value();
    }   

    /**
     * @param id 
     * @return ErrorOr<bool> 
     */
    ErrorOr<bool> Group::id_exists(gid_t id)
    {
        return TRY(Core::System::getgrgid(id)).has_value();
    }

    /**
     * @return ErrorOr<struct group> 
     */
    ErrorOr<struct group> Group::to_libc_group()
    {
        struct group gr;
        gr.gr_name = const_cast<char*>(m_name.characters());
        gr.gr_passwd = const_cast<char*>("x");
        gr.gr_gid = m_id;
        gr.gr_mem = nullptr;

        static Vector<char*> members;
        members.clear_with_capacity();

        if (m_members.size() > 0) {
            TRY(members.try_ensure_capacity(m_members.size() + 1));
            for (auto member : m_members)
                members.unchecked_append(const_cast<char*>(member.characters()));
            members.unchecked_append(nullptr);

            gr.gr_mem = const_cast<char**>(members.data());
        }

        return gr;
    }

}
