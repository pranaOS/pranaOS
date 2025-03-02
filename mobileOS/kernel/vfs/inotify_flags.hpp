/**
 * @file inotify_flags.hpp
 * @author Krisna Pranav
 * @brief inotifiy
 * @version 6.0
 * @date 2024-08-03
 *
 * @copyright Copyright (c) 2021 - 2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

namespace purefs::fs
{
    enum class inotify_flags : unsigned
    {
        attrib = 0x01,
        close_write = 0x02,
        close_nowrite = 0x04,
        del = 0x08,
        move_src = 0x10,
        move_dst = 0x20,
        open = 0x40,
        dmodify = 0x80,
    }; // enum class inotify_flags

    /**
     * @param fl1
     * @param fl2
     * @return auto
     */
    inline auto operator|(inotify_flags fl1, inotify_flags fl2)
    {
        return static_cast<inotify_flags>(static_cast<unsigned>(fl1) | static_cast<unsigned>(fl2));
    }

    /**
     * @param fl1
     * @param fl2
     * @return auto
     */
    inline auto operator&(inotify_flags fl1, inotify_flags fl2)
    {
        return static_cast<inotify_flags>(static_cast<unsigned>(fl1) & static_cast<unsigned>(fl2));
    }

    /**
     * @param fl1
     * @param fl2
     * @return true
     * @return false
     */
    inline auto operator&&(inotify_flags fl1, inotify_flags fl2) -> bool
    {
        return static_cast<bool>(static_cast<unsigned>(fl1) & static_cast<unsigned>(fl2));
    }

    /**
     * @param fl1
     * @param fl2
     * @return true
     * @return false
     */
    inline auto operator||(inotify_flags fl1, inotify_flags fl2) -> bool
    {
        return static_cast<bool>(static_cast<unsigned>(fl1) | static_cast<unsigned>(fl2));
    }

} // namespace purefs::fs
