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

    class FilePermissionsMask 
    {
    public:
        /**
         * @param string 
         * @return ErrorOr<FilePermissionsMask> 
         */
        static ErrorOr<FilePermissionsMask> parse(StringView string);

        /**
         * @param string 
         * @return ErrorOr<FilePermissionsMask> 
         */
        static ErrorOr<FilePermissionsMask> from_numeric_notation(StringView string);

        /**
         * @param string 
         * @return ErrorOr<FilePermissionsMask> 
         */
        static ErrorOr<FilePermissionsMask> from_symbolic_notation(StringView string);

        /**
         * @brief Construct a new FilePermissionsMask object
         * 
         */
        FilePermissionsMask()
            : m_clear_mask(0)
            , m_write_mask(0)
        {
        }

        /**
         * @param mode 
         * @return FilePermissionsMask& 
         */
        FilePermissionsMask& assign_permissions(mode_t mode);

        /**
         * @param mode 
         * @return FilePermissionsMask& 
         */
        FilePermissionsMask& add_permissions(mode_t mode);

        /**
         * @param mode 
         * @return FilePermissionsMask& 
         */
        FilePermissionsMask& remove_permissions(mode_t mode);

        /**
         * @param mode 
         * @return mode_t 
         */
        mode_t apply(mode_t mode) const 
        { 
            return m_write_mask | (mode & ~m_clear_mask); 
        }

        /**
         * @return mode_t 
         */
        mode_t clear_mask() const 
        { 
            return m_clear_mask; 
        }
        
        /**
         * @return mode_t 
         */
        mode_t write_mask() const 
        { 
            return m_write_mask; 
        }

    private:
        mode_t m_clear_mask; 
        mode_t m_write_mask; 
    }; // class FilePermissionsMask 

} // namespace Core
