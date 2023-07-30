/**
 * @file object.mm
 * @author Krisna Pranav
 * @brief Object
 * @version 1.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
*/

#import <objc/objc.h>
#import <stdio.h>
#import <ctype.h>
#import <malloc/malloc.h>
#import "runtime/runtime-private.h"
#import "runtime/objc-private.h"

extern "C" 
{
    extern id _Nullable
    objc_msgSend(id _Nullable self, SEL _Nonnull op, ...);
    extern id
    _objc_constructOrFree(id bytes, Class cls);

    extern void *objc_destructInstance(id obj);

    extern id object_dispose(id obj);
}

extern bool DisableNonpointerIsa;
static Class remapClass(Class cls);

#define fastpath(x) (__builtin_expect(bool(x), 1))
#define slowpath(x) (__builtin_expect(bool(x), 0))

#define TAG_COUNT 8 
#define TAG_SLOT_MASK 0xf

#define TAG_MASK 1
#define TAG_SLOT_SHIFT 0
#define TAG_PAYLOAD_LSHIFT 0
#define TAG_PAYLOAD_RSHIFT 4

SEL SEL_load = NULL;
SEL SEL_initialize = NULL;
SEL SEL_resolveInstanceMethod = NULL;
SEL SEL_resolveClassMethod = NULL;
SEL SEL_cxx_construct = NULL;
SEL SEL_cxx_destruct = NULL;
SEL SEL_retain = NULL;
SEL SEL_release = NULL;
SEL SEL_autorelease = NULL;
SEL SEL_retainCount = NULL;
SEL SEL_alloc = NULL;
SEL SEL_allocWithZone = NULL;
SEL SEL_dealloc = NULL;
SEL SEL_copy = NULL;
SEL SEL_new = NULL;
SEL SEL_finalize = NULL;
SEL SEL_forwardInvocation = NULL;
SEL SEL_tryRetain = NULL;
SEL SEL_isDeallocating = NULL;
SEL SEL_retainWeakReference = NULL;
SEL SEL_allowsWeakReference = NULL;