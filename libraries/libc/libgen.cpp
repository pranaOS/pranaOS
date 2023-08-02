/**
 * @file libgen.cpp
 * @author Krisna Pranav
 * @brief libgen
 * @version 1.0
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libgen.h>
#include <string.h>
#include <mods/assertions.h>

static char dot[] = ".";
static char slash[] = "/";

/**
 * @param path 
 * @return char* 
 */
char* dirname(char* path)
{
    if (path == nullptr)
        return dot;

    int len = strlen(path);

    if (len == 0)
        return dot;

    while (len > 1 && path[len - 1] == '/') {
        path[len - 1] = 0;
        len--;
    }

    char* last_slash = strrchr(path, '/');

    if (last_slash == nullptr)
        return dot;

    if (last_slash == path)
        return slash;

    *last_slash = 0;

    return path;
}

/**
 * @param path 
 * @return char* 
 */
char* basename(char* path)
{
    if (path == nullptr)
        return dot;

    int len = strlen(path);

    if (len == 0)
        return dot;

    while (len > 1 && path[len - 1] == '/') {
        path[len - 1] = 0;
        len--;
    }

    char* last_slash = strrchr(path, '/');
    
    if (last_slash == nullptr)
        return path;

    if (len == 1) {
        ASSERT(last_slash == path);
        ASSERT(path[0] == '/');
        return slash;
    }

    return last_slash + 1;
}
