/**
 * @file bytereader.h
 * @author Krisna Pranav
 * @brief Byte Reader
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stdlibextradetail.h>
#include <mods/types.h>

namespace Mods
{
    struct ByteReader 
    {
        /**
         * @tparam T 
         */
        template<typename T>
        requires(IsTriviallyCopyable<T>) static void store(u8* addr, T value)
        {
            __builtin_memcpy(addr, &value, sizeof(T));
        }

        /**
         * @tparam T 
         */
        template<typename T>
        requires(IsTriviallyConstructible<T>) static void load(u8 const* addr, T& value)
        {
            __builtin_memcpy(&value, addr, sizeof(T));
        }

        /**
         * @tparam T 
         * @param address 
         * @return T* 
         */
        template<typename T>
        static T* load_pointer(u8 const* address)
        {
            FlatPtr value;
            load<FlatPtr>(address, value);
            return reinterpret_cast<T*>(value);
        }

        /**
         * @param address 
         * @return u16 
         */
        static u16 load16(u8 const* address)
        {
            u16 value;
            load(address, value);
            return value;
        }

        /**
         * @param address 
         * @return u32 
         */
        static u32 load32(u8 const* address)
        {
            u32 value;
            load(address, value);
            return value;
        }   

        /**
         * @param address 
         * @return u64 
         */
        static u64 load64(u8 const* address)
        {
            u64 value;
            load(address, value);
            return value;
         }
    }; // struct ByteReader
} // namespace Mods

using Mods::ByteReader;
