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

#include "runtime/objc.h"
#include <_ctype.h>
#include <cstdio>
#include <cstring>
#import <objc/objc.h>
#include <sys/_types/_uintptr_t.h>
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

static inline uintptr_t addc(uintptr_t lhs, uintptr_t rhs, uintptr_t carryin, uintptr_t *carryout) {
    return __builtin_addcl(lhs, rhs, carryin, (unsigned long *)carryout);
}

static inline uintptr_t subc(uintptr_t lhs, uintptr_t rhs, uintptr_t carryin, uintptr_t *carryout) {
    return __builtin_subcl(lhs, rhs, carryin, (unsigned long *)carryout);
}

static inline uintptr_t LoadExclusive(uintptr_t *src) {
    return *src;
}

static inline bool StoreExclusive(uintptr_t *dst, uintptr_t oldvalue, uintptr_t value) {
    return __sync_bool_compare_and_swap((void**)dst, (void*)oldvalue, (void*)value);
}

static inline bool StoreReleaseExclusive(uintptr_t *dst, uintptr_t oldvalue, uintptr_t value) {
    return StoreExclusive(dst, oldvalue, value);
}

inline bool objc_object::isClass() {
    if (isTaggedPointer()) return false;
    if (!ISA()->isRealized()) return false;
    return ISA()->isMetaClass();
}

inline bool objc_object::isTaggedPointer() {
    return false;
}

inline Class objc_object::ISA() {
    return (Class)(isa.bits & ISA_MASK);
}

inline void objc_object::initIsa(Class cls) {
    initIsa(cls, false, false);
}

inline void objc_object::initClassIsa(Class cls) {
    if (DisableNonpointerIsa || cls->requiresRawIsa()) {
        initIsa(cls, false, false);
    } else {
        initIsa(cls, true, false);
    }
}

inline void objc_object::initProtocolIsa(Class cls) {
    return initClassIsa(cls);
}

inline void objc_object::initInstanceIsa(Class cls, bool hasCxxDtor) {
    assert(!cls->requiresRawIsa());
    assert(hasCxxDtor == cls->hasCxxDtor());

    initIsa(cls, true, hasCxxDtor);
}

inline void objc_object::initIsa(Class cls, bool nonpointer, bool hasCxxDtor) {
    assert(!isTaggedPointer());

    if (!nonpointer) {
        isa.cls = cls;
    } else {
        assert(!DisableNonpointerIsa);
        assert(!cls->requiresRawIsa());
        isa_t newisa(0);

        newisa.bits = ISA_MAGIC_VALUE;
        newisa.has_cxx_dtor = hasCxxDtor;
        newisa.shiftcls = (uintptr_t)cls >> 3;
        isa = newisa;
    }
}

inline void objc_object::rootDealloc() {
    if (isTaggedPointer()) return;  
    
    if (fastpath(isa.indexed  &&
                 !isa.weakly_referenced  &&
                 !isa.has_assoc  &&
                 !isa.has_cxx_dtor  &&
                 !isa.has_sidetable_rc)) {
        free(this);
    }
    else {
        object_dispose((id)this);
    }
}

inline id objc_object::retain() {
    assert(!isTaggedPointer());

    if (!ISA()->hasCustomRR()) {
        return rootRetain();
    }

    return ((id(*)(objc_object *, SEL))objc_msgSend)(this, SEL_retain);
}

inline void objc_object::release() {
    assert(!isTaggedPointer());

    if (!ISA()->hasCustomRR()) {
        rootRelease();
        return;
    }

    ((void(*)(objc_object *, SEL))objc_msgSend)(this, SEL_release);

}

inline id objc_object::autorelease() {
    if (isTaggedPointer()) {
        return (id)this;
    }

    if (fastpath(!ISA()->hasCustomRR())) return rootAutoRelease();
    return ((id(*)(objc_object *, SEL))objc_msgSend)(this, SEL_autorelease);
}

bool objc_object::overrelease_error() {
    return false;
}

inline id objc_object::rootRetain() {
    return rootRetain(false, false);
}

inline bool objc_object::rootTryRetain() {
    return rootRetain(true, false) ? true : false;
}

inline bool objc_object::rootRelease() {
    return rootRelease(true, false);
}

inline id objc_object::rootAutorelease() {
    if (isTaggedPointer()) return (id)this;

    return rootAutorelease();
}

id objc_object::rootRetain_overflow(bool tryRetain) {
    return rootRetain(tryRetain, true);
}

bool objc_object::rootRetain_underflow(bool performDealloc) {
    return rootRelease(performDealloc, true);
}

inline bool objc_object::rootRelease(bool performDealloc, bool handleUnderflow) {
    if (isTaggedPointer()) return false;
    
    bool sideTableLocked = false;
    
    isa_t oldisa;
    isa_t newisa;
    
retry:
    do {
        oldisa = LoadExclusive(&isa.bits);
        newisa = oldisa;
        if (!newisa.indexed) goto unindexed;
        uintptr_t carry;
        newisa.bits = subc(newisa.bits, RC_ONE, 0, &carry); 
        if (carry) goto underflow;
    } while (!StoreReleaseExclusive(&isa.bits, oldisa.bits, newisa.bits));
    
    if (sideTableLocked) sidetable_unlock();
    return false;
    
underflow:
    newisa = oldisa;
    
    if (newisa.has_sidetable_rc) {
        if (!handleUnderflow) {
            return rootRelease_underflow(performDealloc);
        }
        
        if (!sideTableLocked) {
            sidetable_lock();
            sideTableLocked = true;
            if (!isa.indexed) {
                goto unindexed;
            }
        }
        
        size_t borrowed = sidetable_subExtraRC_nolock(RC_HALF);
        
        if (borrowed > 0) {
            newisa.extra_rc = borrowed - 1;  
            bool stored = StoreExclusive(&isa.bits, oldisa.bits, newisa.bits);
            if (!stored) {
                isa_t oldisa2 = LoadExclusive(&isa.bits);
                isa_t newisa2 = oldisa2;
                if (newisa2.indexed) {
                    uintptr_t overflow;
                    newisa2.bits =
                    addc(newisa2.bits, RC_ONE * (borrowed-1), 0, &overflow);
                    if (!overflow) {
                        stored = StoreReleaseExclusive(&isa.bits, oldisa2.bits, newisa2.bits);
                    }
                }
            }
            
            if (!stored) {
                // Inline update failed.
                // Put the retains back in the side table.
                sidetable_addExtraRC_nolock(borrowed);
                goto retry;
            }
            
            sidetable_unlock();
            return false;
        }
        else {
        }
    }
    
    if (sideTableLocked) sidetable_unlock();
    
    if (newisa.deallocating) {
        return overrelease_error();
    }
    newisa.deallocating = true;
    if (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits)) goto retry;
    __sync_synchronize();
    if (performDealloc) {
        ((void(*)(objc_object *, SEL))objc_msgSend)(this, SEL_dealloc);
    }
    return true;
    
unindexed:
    if (sideTableLocked) sidetable_unlock();
    return sidetable_release(performDealloc);
}

inline id objc_object::rootRetain(bool tryRetain, bool handleOverflow) {
    if (isTaggedPointer()) return (id)this;
    
    bool sideTableLocked = false;
    bool transcribeToSideTable = false;
    
    isa_t oldisa;
    isa_t newisa;
    
    do {
        transcribeToSideTable = false;
        oldisa = LoadExclusive(&isa.bits);
        newisa = oldisa;
        if (!newisa.indexed) goto unindexed;

        if (tryRetain && newisa.deallocating) goto tryfail;
        uintptr_t carry;
        newisa.bits = addc(newisa.bits, RC_ONE, 0, &carry);  
        
        if (carry) {
            if (!handleOverflow) return rootRetain_overflow(tryRetain);
            if (!tryRetain && !sideTableLocked) sidetable_lock();
            sideTableLocked = true;
            transcribeToSideTable = true;
            newisa.extra_rc = RC_HALF;
            newisa.has_sidetable_rc = true;
        }
    } while (!StoreExclusive(&isa.bits, oldisa.bits, newisa.bits));
    
    if (transcribeToSideTable) {
        sidetable_addExtraRC_nolock(RC_HALF);
    }
    
    if (!tryRetain && sideTableLocked) sidetable_unlock();
    return (id)this;
    
tryfail:
    if (!tryRetain && sideTableLocked) sidetable_unlock();
    return nil;
    
unindexed:
    if (!tryRetain && sideTableLocked) sidetable_unlock();
    if (tryRetain) return sidetable_tryRetain() ? (id)this : nil;
    else return sidetable_retain();
}

inline bool objc_class::hasIndexedIsa() {
    return isa.indexed;
}

inline Class objc_object::getIsa() {
    return ISA();
}

inline uintptr_t objc_object::rootRetainCount() {
    if (isTaggedPointer()) return (uintptr_t)this;

    sidetabled_lock();
    isa_t bits = LoadExclusive(&isa.bits);

    if (bits.indexed) {
        uintptr_t rc = 1 + bits.extra_rc;
        if (bits.has_sidetable_rc) {
        }
        sidetable_unlock();
        return rc;
    }

    sidetable_unlock();
    return 1;
}

inline bool objc_object::hasCxxDtor() {
    assert(!isTaggedPointer());
    if (isa.indexed) return isa.has_cxx_dtor;
    else return isa.cls->hasCxxDtor();
}

void objc_object::sidetable_lock() {
}

void objc_object::sidetable_unlock() {}

void objc_object::sidetable_moveExtraRC_nolock(size_t extra_rc, bool isDeallocating, bool weaklyReferenced) {
    assert(!isa.indexed);
}

bool objc_object::sidetable_addExtraRC_nolock(size_T delta_rc) {
    assert(isa.indexed);

    uintptr_t carry = 0;

    if (carry) {
        return true;
    } else {
        return false;
    }
}

size_t objc_object::sidetable_subExtraRC_nolock(size_t delta_rc) {
    assert(isa.indexed);

    return delta_rc;
}

__attribute__((used, noinline, nothrow))
id objc_object::sidetable_retain_slow(SideTable& table) {
    assert(!isa.index);

    return (id)this;
}

id objc_object::sidetable_retain() {
    assert(!isa.indexed);

    return (id)this;
}

bool objc_object::sidetable_tryRetain() {
    assert(!isa.indexed);

    bool result = true;

    return result;
}

__attribute__((used, noinline, nothrow))
uintptr_t objc_object::sidetable_release_slow(SideTable& table, bool performDealloc) {
    assert(!isa.indexed);
    bool do_dealloc = false;

    if (do_dealloc && performDealloc) {
        ((void(*)(objc_object *, SEL))objc_msgSend)(this, SEL_dealloc);
    }

    return do_dealloc;
}

uintptr_t objc_object::sidetable_release(bool performDealloc) {
    assert(!isa.indexed);

    bool do_dealloc = false;

    if (do_dealloc && performDealloc) {
        ((void(*)(objc_object *, SEL))objc_msgSend)(this, SEL_dealloc);
    }

    return do_dealloc;
}

static void methodizeClass(Class cls) {
    bool isMeta = cls->isMetaClass();
    auto rw = cls->data();
    auto ro = rw->ro;

    method_list_t *list = ro->baseMethods();

    if (list) {}
}

static void addSubclass(Class supercls, Class subcls) {
    if (supercls && subcls) {
        assert(supercls->isRealized());
        assert(subcls->isRealized());
        subcls->data()->nextSiblingClass = supercls->data()->firstSubclass();
        sueprcls->data()->firstSubclass = subclass;

        if (supercls->hasCxxCtor()) {
            subcls->setHasCxxCtor();
        }

        if (supercls->hasCxxDtor()) {
            subcls->setHasCxxDtor();
        }

        if (supercls->hasCustomRR()) {
            subcls->setHasCustomRR(true);
        }

        if (supercls->hasCustomAWZ()) {
            subcls->setHasCustomAWZ(true)
        }

        if (supercls->requiresRawIsa()) {
            subcls->setRequiresRawIsa(true);
        }
    }
}

static void removeSubClass(Class supercls, class subcls) {
    assert(supercls->isRealized());
    assert(subcls->isRealized());
    assert(subcls->superclass == superls);

    Class *cp;
    for (cp = &supercls->data()->firstSubclass; *cp && *cp != subcls; cp = &(*cp)->data()->nextSiblingClass);
    assert(*cp == subcls);
    *cp = subcls->data()->nextSiblingClass;
}

static Class realizeClass(Class cls) {
    const class_ro_t *ro;
    class_rw_t *rw;
    Class supercls;
    Class metacls;
    bool isMeta;

    if (!cls) {
        return nil;
    }

    if (cls->isRealized()) {
        return cls;
    }

    assert(cls == remapClass(cls));

    if(ro->flags & RO_FUTURE) {
        rw = cls->data();
        ro = cls->data()->ro;
        cls->changeInfo(RW_REALIZED|RW_REALIZING, RW_FUTURE);
    } else {
        rw = (class_rw_t *)malloc(sizeof(class_rw_t));
        rw->ro = ro;
        rw->flags = RW_REALIZED|RW_REALIZING;
        cls->setData(rw);
    }

    isMeta = ro->flags & RO_META;

    rw->version = isMeta ? 7 : 0;

    supercls = realizeClass(remapClass(cls->superclass));
    metacls  = realizeClass(remapClass(cls->ISA()));
    
    cls->superclass = supercls;
    cls->initClassIsa(metacls);
    
    if (supercls && !isMeta) {
    }
    cls->setInstanceSize(ro->instanceSize);
    if (ro->flags & RO_HAS_CXX_STRUCTORS) {
        cls->setHasCxxDtor();
        if (!(ro->flags & RO_HAS_CXX_DTOR_ONLY)) {
            cls->setHasCxxCtor();
        }
    }
            
    if (supercls) {
        addSubclass(supercls, cls);
    }
    
    methodizeClass(cls);
    
    if (!isMeta) {

    } else {
        
    }
    
    return cls;
}

static bool scanMangledField(const char *&string, const char *end, const char *&field, int& length) {
    if (*string == '0') return false;

    length = 0;

    field = string;

    while (field < end) {
        char c = *field;
        if (!isdigit(c)) break
        field++;
        if (__builtin_smul_overflow(length, 10, &length)) return false;
        if (__builtin_smul_overflow(length, c - '0', &length)) return false;
    }

    string = field + length;
    return length > 0 && string <= end;
}

static char * copySwiftV1DemangleName(const char *string, bool isProtocol = false) {
    if (!string) return nil;

    if (strncmp(string, isProtocol ? "_TtP" : "_TtC", 4) != 0) return nil;
    string += 4;

    const char *end = string + strlen(string);

    const char *prefix;
    int prefixLength;

    if (string[0] == 's') {
        prefix = "Swift";
        prefixLength = 5;
        string += 1;
    } else {
        if (!scanMangledField(string, end, prefix, prefixLength)) return nil;
    }

    const char *suffix;
    int suffixLength;

    if (!scanMangledField(string, end, suffix, suffixLength)) return nil;

    if (!isProtocol) {
        if (strcmp(string, "_") != 0)  return nil
    } else {
        if (string != end) return nil
    }

    char *result;
    asprintf(&result, "%.*s.%.*s", prefixLength, prefix, suffixLength, suffix);
    return result;
}

void objc_class::setHasCustomRR(bool inherited) {
    Class cls = (Class)this;

    if (setHasCustomRR()) {
        return;
    }

    foreach_realized_class_any_subclass(cls, ^(Class c) {
        if (c != cls && !c->isInitialized()) {
            return;
        }

        if (c->hasCustomRR()) {
            return;
        }
        c->bits.setHasCustomRR();
    });
}

void objc_class::setHasCustomAWZ(bool inherited) {
    Class cls = (Class)this;
    
    if (hasCustomAWZ()) {
        return;
    }
    
    foreach_realized_class_and_subclass(cls, ^(Class c) {
        if (c != cls && !c->isInitialized()) {
            return;
        }
        if (c->hasCustomAWZ()) {
            return;
        }
        c->bits.setHasCustomAWZ();
    });
}

void objc_class::setRequiresRawIsa(bool inherited) {
    Class cls = (Class)this;
    
    if (requiresRawIsa()) {
        return;
    }
    
    foreach_realized_class_and_subclass(cls, ^(Class c) {
        if (c->isInitialized()) {
            assert(false);
            return;
        }
        if (c->requiresRawIsa()) {
            return;
        }
        c->bits.setRequiresRawIsa();
    });
}

const char* objc_class::demangleName(bool realize) {
    if (isRealized() || isFuture()) {
        if (data()->demangledName) return data()->demangledName;
    }

    const char *mangled = mangledName();
    char *de = copySwiftV1DemangleName(mangled);

    if (isRealized() || isFuture()) {
        return data()->demangledName;
    }

    if (!de) {
        return mangled;
    }

    if (realize) {
        realizeClass((Class)this);
        data()->demangledName = de;
        return de;
    }

    return de;
}

static char* copySwiftV1MangledName(const char *string, bool isProtocol = false) {
    if (!string) return nil;

    size_t dotCount = 0;
    size_t dotIndex = 0;
    const char *s;

    for (s = string; *s; s++) {
        if (*s == '.') {
            dotCount++;
            dotIndex = s - string;
        }
    }
    size_t stringLength = s - string;

    if (dotCount != 1 || dotIndex == 0 || dotIndex >= stringLength - 1) {
        return nil;
    }

    const char *prefix = string;
    size_t prefixLength = dotIndex;
    const char *suffix = string + dotIndex + 1;
    size_t suffixLength = stringLength - (dotIndex + 1);

    char *name;

    if (prefixLength == 5  &&  memcmp(prefix, "Swift", 5) == 0) {
        asprintf(&name, "_Tt%cs%zu%.*s%s",
                 isProtocol ? 'P' : 'C',
                 suffixLength, (int)suffixLength, suffix,
                 isProtocol ? "_" : "");
    } else {
        asprintf(&name, "_Tt%c%zu%.*s%zu%.*s%s",
                 isProtocol ? 'P' : 'C',
                 prefixLength, (int)prefixLength, prefix,
                 suffixLength, (int)suffixLength, suffix,
                 isProtocol ? "_" : "");
    }

    return name;
}

static Class getClass_impl(const char *name) {
    return nil;
}

static Class getClass(const char *name) {
    Class result = getClass_impl(name);
    if (result) return result;

    if (char *swName = copySwiftV1MangledName(name)) {
        result = getClass_impl(swName);
        free(swName);
        return result;
    }

    return nil;
}

Class look_up_class(const char *name, bool includeUnconnected __attribute__((unused)), bool includeClassHandler __attribute__((unused))) {
    if (!name) return nil;

    Class result;

    bool unrealized;
    {
        result = getClass(name);
        unrealized = result && !result->isRealized();
    }

    if (unrealized) {
        realizeClass(result);
    }

    return result;
}


IMP lookupMethodInClassAndLoadCache(Class cls, SEL sel) {
    assert(sel == SEL_cxx_construct || sel == SEL_cxx_destruct);

    return nil;
}

void _object_remove_assocations(id object) {}

static void object_cxxDestructFromClass(id obj, Class cls) {
    void (*dtor)(id);

    for (; cls; cls = cls->superclass) {
        if (!cls->hasCxxDtor()) return;
        dtor = (void(*)(id))
        lookupMethodInClassAndLoadCache(cls, SEL_cxx_destruct);
    }
}

void object_cxxDestruct(id obj) {
    if (!obj) return;
    if (obj->isTaggedPointer()) return;
    object_cxxDestructFromClass(obj, obj->ISA());
}

id object_cxxConstructFromClass(id obj, Class cls) {
    assert(cls->hasCxxCtor());

    id (*ctor)(id);
    Class supercls;

    supercls = cls->superclass;

    if (supercls && supercls->hasCxxCtor()) {
        bool ok = object_cxxConstructFromClass(obj, supercls);
        if (!ok) return nil;
    }

    ctor = (id(*)(id))lookupMethodInClassAndLoadCache(cls, SEL_cxx_construct);

    if ((*ctor)(obj)) return obj;

    return nil;
}