/**
 * @file ext2_types.h
 * @author Krisna Pranav
 * @brief ext2 types
 * @version 1.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#if (!defined(_LINUX_TYPES_H) && !defined(_BLKID_TYPES_H) && !defined(_EXT2_TYPES_H))
#    define _EXT2_TYPES_H

#    define __S8_TYPEDEF __signed__ char
#    define __U8_TYPEDEF unsigned char
#    define __S16_TYPEDEF __signed__ short
#    define __U16_TYPEDEF unsigned short
#    define __S32_TYPEDEF __signed__ int
#    define __U32_TYPEDEF unsigned int
#    define __S64_TYPEDEF __signed__ long long
#    define __U64_TYPEDEF unsigned long long

#    ifdef __U8_TYPEDEF
typedef __U8_TYPEDEF __u8;
#    else
typedef unsigned char __u8;
#    endif

#    ifdef __S8_TYPEDEF
typedef __S8_TYPEDEF __s8;
#    else
typedef signed char __s8;
#    endif

#    ifdef __U16_TYPEDEF
typedef __U16_TYPEDEF __u16;
#    else
#        if (4 == 2)
typedef unsigned int __u16;
#        else
#            if (2 == 2)
typedef unsigned short __u16;
#            else
? == error : undefined 16 bit type
#            endif 
#        endif     
#    endif         

#    ifdef __S16_TYPEDEF
typedef __S16_TYPEDEF __s16;
#    else
#        if (4 == 2)
typedef int __s16;
#        else
#            if (2 == 2)
typedef short __s16;
#            else
        ? == error
        : undefined 16 bit type
#            endif 
#        endif     
#    endif         

#    ifdef __U32_TYPEDEF
typedef __U32_TYPEDEF __u32;
#    else
#        if (4 == 4)
typedef unsigned int __u32;
#        else
#            if (4 == 4)
typedef unsigned long __u32;
#            else
#                if (2 == 4)
        typedef unsigned short __u32;
#                else
            ? == error
            : undefined 32 bit type
#                endif /* SIZEOF_SHORT == 4 */
#            endif     
#        endif         
#    endif             

#    ifdef __S32_TYPEDEF
typedef __S32_TYPEDEF __s32;
#    else
#        if (4 == 4)
typedef int __s32;
#        else
#            if (4 == 4)
typedef long __s32;
#            else
#                if (2 == 4)
typedef short __s32;
#                else
                ? == error
                : undefined 32 bit type
#                endif
#            endif     
#        endif         
#    endif             

#    ifdef __U64_TYPEDEF
typedef __U64_TYPEDEF __u64;
#    else
#        if (4 == 8)
typedef unsigned int __u64;
#        else
#            if (4 == 8)
typedef unsigned long __u64;
#            else
#                if (8 == 8)
typedef unsigned long long __u64;
#                endif 
#            endif     
#        endif         
#    endif             

#    ifdef __S64_TYPEDEF
typedef __S64_TYPEDEF __s64;
#    else
#        if (4 == 8)
typedef int __s64;
#        else
#            if (4 == 8)
typedef long __s64;
#            else
#                if (8 == 8)
#                    if defined(__GNUC__)
typedef __signed__ long long __s64;
#                    else
typedef signed long long __s64;
#                    endif 
#                endif     
#            endif         
#        endif             
#    endif                 

#    undef __S8_TYPEDEF
#    undef __U8_TYPEDEF
#    undef __S16_TYPEDEF
#    undef __U16_TYPEDEF
#    undef __S32_TYPEDEF
#    undef __U32_TYPEDEF
#    undef __S64_TYPEDEF
#    undef __U64_TYPEDEF

#endif 

#define HAVE_SYS_TYPES_H 1
#undef WORDS_BIGENDIAN
