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

#include <mods/format.h>
#include <assert.h>
#include <mntent.h>

extern "C" {

/**
 * @return struct mntent* 
 */
struct mntent* getmntent(FILE*)
{
    dbgln("FIXME: Implement getmntent()");
    return nullptr;
}

/**
 * @return FILE* 
 */
FILE* setmntent(char const*, char const*)
{
    dbgln("FIXME: Implement setmntent()");
    return nullptr;
}

/**
 * @return int 
 */
int endmntent(FILE*)
{
    dbgln("FIXME: Implement endmntent()");
    return 0;
}

/**
 * @brief Get the mntent r object
 * 
 * @return struct mntent* 
 */
struct mntent* getmntent_r(FILE*, struct mntent*, char*, int)
{
    dbgln("FIXME: Implement getmntent_r()");
    return 0;
}
}
