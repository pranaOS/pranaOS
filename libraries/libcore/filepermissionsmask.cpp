/**
 * @file filepermissionsmask.cpp
 * @author Krisna Pranav
 * @brief File permissions mask
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/assertions.h>
#include <mods/charactertypes.h>
#include <mods/stringutils.h>
#include <libcore/filepermissionsmask.h>

namespace Core
{

    enum State
    {
        Classes,
        Mode
    }; // enum State

    enum ClassFlag 
    {
        Other = 1,
        Group = 2,
        User = 4,
        All = 7
    }; // enum ClassFlag 

    enum Operation
    {
        Add,
        Remove,
        Assign
    }; // enum Operation

    /**
     * @param mode 
     * @return FilePermissionsMask& 
     */
    FilePermissionsMask& FilePermissionsMask::assign_permissions(mode_t mode)
    {
        m_write_mask = mode;
        m_clear_mask = 0777;
        return *this;
    }

    /**
     * @param mode 
     * @return FilePermissionsMask& 
     */
    FilePermissionsMask& FilePermissionsMask::add_permissions(mode_t mode)
    {
        m_write_mask |= mode;
        return *this;
    }

    /**
     * @param mode 
     * @return FilePermissionsMask& 
     */
    FilePermissionsMask& FilePermissionsMask::remove_permissions(mode_t mode)
    {
        m_clear_mask |= mode;
        return *this;
    }

} // namespace Core