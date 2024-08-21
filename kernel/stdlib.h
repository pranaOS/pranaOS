/**
 * @file stdlib.h
 * @author Krisna Pranav
 * @brief stdlib
 * @version 0.1
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
String copy_string_from_user(const char*, size_t);

/**
 * @return String 
 */
String copy_string_from_user(Userspace<const char*>, size_t);

extern "C" 
{

    /**
     * @return true 
     * @return false 
     */
    [[nodiscard]] bool copy_to_user(void*, const void*, size_t);

    /**
     * @return true 
     * @return false 
     */
    [[nodiscard]] bool copy_from_user(void*, const void*, size_t);

    /**
     * @return true 
     * @return false 
     */
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
    int strncmp(const char* s1, const char* s2, size_t n);

    /**
     * @param haystack 
     * @param needle 
     * @return char* 
     */
    char* strstr(const char* haystack, const char* needle);

    /**
     * @return int 
     */
    int strcmp(char const*, const char*);

    /**
     * @return size_t 
     */
    size_t strlen(const char*);

    /**
     * @return size_t 
     */
    size_t strnlen(const char*, size_t);

    /**
     * @return void* 
     */
    void* memset(void*, int, size_t);

    /**
     * @return int 
     */
    int memcmp(const void*, const void*, size_t);

    /**
     * @param dest 
     * @param src 
     * @param n 
     * @return void* 
     */
    void* memmove(void* dest, const void* src, size_t n);

    /**
     * @param haystack 
     * @param needle 
     * @return const void* 
     */
    const void* memmem(const void* haystack, size_t, const void* needle, size_t);

    /**
     * @param w 
     * @return u16 
     */
    inline u16 ntohs(u16 w) { 
        return (w & 0xff) << 8 | ((w >> 8) & 0xff); 
    }

    /**
     * @param w 
     * @return u16 
     */
    inline u16 htons(u16 w) { 
        return (w & 0xff) << 8 | ((w >> 8) & 0xff); 
    }
}


/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_from_user(T* dest, const T* src)
{
    static_assert(is_trivially_copyable<T>());
    return copy_from_user(dest, src, sizeof(T));
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_to_user(T* dest, const T* src)
{
    static_assert(is_trivially_copyable<T>());
    return copy_to_user(dest, src, sizeof(T));
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_from_user(T* dest, Userspace<const T*> src)
{
    static_assert(is_trivially_copyable<T>());
    return copy_from_user(dest, src.unsafe_userspace_ptr(), sizeof(T));
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_from_user(T* dest, Userspace<T*> src)
{
    static_assert(is_trivially_copyable<T>());
    return copy_from_user(dest, src.unsafe_userspace_ptr(), sizeof(T));
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_to_user(Userspace<T*> dest, const T* src)
{
    static_assert(is_trivially_copyable<T>());
    return copy_to_user(dest.unsafe_userspace_ptr(), src, sizeof(T));
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @param size 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_to_user(Userspace<T*> dest, const void* src, size_t size)
{
    static_assert(is_trivially_copyable<T>());
    return copy_to_user(dest.unsafe_userspace_ptr(), src, size);
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @param size 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_from_user(void* dest, Userspace<const T*> src, size_t size)
{
    static_assert(is_trivially_copyable<T>());
    return copy_from_user(dest, src.unsafe_userspace_ptr(), size);
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @param count 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_n_from_user(T* dest, const T* src, size_t count)
{
    static_assert(is_trivially_copyable<T>());
    Checked size = sizeof(T);
    size *= count;
    if (size.has_overflow())
        return false;
    return copy_from_user(dest, src, sizeof(T) * count);
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @param count 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_n_to_user(T* dest, const T* src, size_t count)
{
    static_assert(is_trivially_copyable<T>());
    Checked size = sizeof(T);
    size *= count;
    if (size.has_overflow())
        return false;
    return copy_to_user(dest, src, sizeof(T) * count);
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @param count 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_n_from_user(T* dest, Userspace<const T*> src, size_t count)
{
    static_assert(is_trivially_copyable<T>());
    Checked size = sizeof(T);
    size *= count;
    if (size.has_overflow())
        return false;
    return copy_from_user(dest, src.unsafe_userspace_ptr(), sizeof(T) * count);
}

/**
 * @tparam T 
 * @param dest 
 * @param src 
 * @param count 
 * @return true 
 * @return false 
 */
template<typename T>
[[nodiscard]] inline bool copy_n_to_user(Userspace<T*> dest, const T* src, size_t count)
{
    static_assert(is_trivially_copyable<T>());
    Checked size = sizeof(T);
    size *= count;
    if (size.has_overflow())
        return false;
    return copy_to_user(dest.unsafe_userspace_ptr(), src, sizeof(T) * count);
}
