/**
 * @file mntent.cpp
 * @author Krisna Pranav
 * @brief mntent
 * @version 6.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mntent.h>
#include <assert.h>

extern "C" 
{
    /**
     * @return struct mntent* 
     */
    struct mntent* getmntent(FILE*)
    {
        ASSERT_NOT_REACHED();
        return nullptr;
    } // struct getmntent

} // extern
