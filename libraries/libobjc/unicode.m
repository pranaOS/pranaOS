//
//  unicode.m
//  libobjc
//
//  Created by krisna pranav on 19/06/23.
//

#import <objc/objc.h>

#define UNIMPLEMENTED(x) void x() { ULTDBG(__func__); }

#ifdef ULTDEBUG
#define ULTDBG(x...) printf(x)
#else
#define ULTDBG(x...)
#endif

UNIMPLEMENTED(u_strToLower)
UNIMPLEMENTED(u_strToUpper)
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
