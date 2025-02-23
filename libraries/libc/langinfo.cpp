/**
 * @file langinfo.cpp
 * @author Krisna Pranav
 * @brief langinfo
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

extern "C" {

/**
 * @param item 
 * @return char* 
 */
char* nl_langinfo(nl_item item)
{
    return const_cast<char*>(__nl_langinfo(item));
}
}