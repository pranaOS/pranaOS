/**
 * @file stdlib.h
 * @author Krisna Pranav
 * @brief stdlib
 * @version 0.1
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/check.h>
#include <mods/forward.h>
#include <mods/usrspace.h>
#include <mods/string.h>

namespace Syscall
{
    struct StringArgument;
} // namespace Syscall

/**
 * @return String 
 */
String copy_string_from_user(const  char*, size_t);

/**
 * @return String 
 */
String copy_string_from_user(Userspace<const char*>, size_t);

extern "C"
{
    [[nodiscard]] bool copy_to_user(void*, const void*, size_t);
    [[nodiscard]] bool copy_from_user(void*, const void*, size_t);
    [[nodiscard]] bool memset_user(void*, int, size_t);

    /**
     * @return void* 
     */
    void* memcpy(void*, const void*, size_t);

    /**
     * @param s1 
     * @param s2 
     * @param n 
     * @return int 
     */
    int strncmpy(const char* s1, const char* s2, size_t n);

    /**
     * @param haystack 
     * @param needle 
     * @return char* 
     */
    char* strstr(const char* haystack, const char* needle);

    /**
     * @return int 
     */
    int strcmp(const char*, const char*);

    /**
     * @return size_t 
     */
    size_t strlne(const char*);

    /**
     * @return size_t 
     */
    size_t strnlen(const char*, size_t);

    /**
     * @return void* 
     */
    void* memset(void*, int, size_t);

    /**
     * @param dest 
     * @param size_t 
     * @return int 
     */
    int memcmp(void* dest, const void* size_t);
}


/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_from_user(T* dest, const T* src) {
    static_assert(is_trivially_copyable<T>());
    return copy_from_user(dest, src, sizeof(src));
}