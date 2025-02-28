/**
 * @file group.h
 * @author Krisna Pranav
 * @brief group
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/string.h>
#include <mods/vector.h>
#include <grp.h>

namespace Core
{
    
    class Group
    {
    private:
        /**
         * @param name 
         * @return ErrorOr<bool> 
         */
        static ErrorOr<bool> name_exists(StringView name);

        /**
         * @param id 
         * @return ErrorOr<bool> 
         */
        static ErrorOr<bool> id_exists(gid_t id);

        /**
         * @return ErrorOr<struct group> 
         */
        ErrorOr<struct group> to_libc_group();

        String m_name;
        
        gid_t m_id { 0 };

        Vector<String> m_members;
    }; // class Group

} // namespace Core
