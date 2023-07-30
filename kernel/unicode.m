/**
 * @file unicde.m
 * @author Krisna Pranav
 * @brief Unicode Support
 * @version 1.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
*/

#import <objc/objc.h>

#define UNIMPLEMENTED(x) void x() { ULTDBG(__func__); }

#ifdef ULTDEBUG
#define UTLDBG(x...) printf(x)
#else
#define UTLDBG(x...)
#endif

UNIMPLEMENTED(u_strToLower)