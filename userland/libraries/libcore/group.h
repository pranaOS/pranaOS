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
    public:
    #ifndef MOD_OS_BSD_GENERIC
        /**
         * @param group 
         * @return ErrorOr<void> 
         */
        static ErrorOr<void> add_group(Group& group);
    #endif

        /**
         * @brief Construct a new Group object
         * 
         */
        Group() = default;

        /**
         * @brief Construct a new Group object
         * 
         * @param name 
         * @param id 
         * @param members 
         */
        Group(String name, gid_t id = 0, Vector<String> members = {});

        /**
         * @brief Destroy the Group object
         * 
         */
        ~Group() = default;

        /**
         * @return String const& 
         */
        String const& name() const 
        { 
            return m_name; 
        }

        /**
         * @brief Set the name object
         * 
         * @param name 
         */
        void set_name(String const& name) 
        { 
            m_name = name; 
        }

        /**
         * @return gid_t 
         */
        gid_t id() const 
        { 
            return m_id; 
        }

        /**
         * @brief Set the group id object
         * 
         * @param id 
         */
        void set_group_id(gid_t const id) 
        { 
            m_id = id; 
        }

        /**
         * @return Vector<String>& 
         */
        Vector<String>& members() 
        { 
            return m_members; 
        }

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
