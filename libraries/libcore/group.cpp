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
} // namespace Core