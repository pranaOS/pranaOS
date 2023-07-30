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

#include <objc/objc.h>

#define UNIMPLEMENTED(x) void x() { ULTDBG(__func__); }

#ifdef ULTDEBUG
#define ULTDBG(x...) printf(x)
#else
#define ULTDBG(x...)
#endif

UNIMPLEMENTED(strToLower)
UNIMPLEMENTED(strToUpper)
UNIMPLEMENTED(ucol_closeElements)
UNIMPLEMENTED(ucol_next)
UNIMPLEMENTED(ucol_openElements)
UNIMPLEMENTED(ucol_open)
UNIMPLEMENTED(ucol_setAttribute)
UNIMPLEMENTED(ucol_strcollIter)
UNIMPLEMENTED(ucol_strcoll)
UNIMPLEMENTED(uiter_setString)
UNIMPLEMENTED(uiter_setUTF8)
UNIMPLEMENTED(ubrk_close)
UNIMPLEMENTED(ubrk_open)
UNIMPLEMENTED(ubrk_preceding)
UNIMPLEMENTED(ubrk_following)
UNIMPLEMENTED(ubrk_setText)