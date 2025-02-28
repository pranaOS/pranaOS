/**
 * @file filepermissionsmask.h
 * @author Krisna Pranav
 * @brief File Permissions Mask
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/string.h>
#include <sys/stat.h>

namespace Core
{

    class FilePermissionMask
    {
    public:
        /**
         * @brief Construct a new FilePermissionMask object
         * 
         */
        FilePermissionMask()
            : m_clear_mask(0)
            , m_write_mask(0)
        {}

        /**
         * @param mode 
         * @return FilePermissionMask& 
         */
        FilePermissionMask& assign_permissions(mode_t mode);

        /**
         * @param mode 
         * @return FilePermissionMask& 
         */
        FilePermissionMask& add_permissions(mode_t mode);   

        /**
         * @param mode 
         * @return FilePermissionMask& 
         */
        FilePermissionMask& remove_permissions(mode_t mode);
        
    private:
        mode_t m_clear_mask;
        mode_t m_write_mask;
    }; // class FilePermissionMask

} // namespace Core
