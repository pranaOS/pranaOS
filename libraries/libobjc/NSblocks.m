//
//  NSblocks.m
//  libobjc
//
//  Created by KrisnaPranav on 06/03/22.
//

#import <stdbool.h>
#import <stdlib.h>
#import <stdio.h>
#import <string.h>

#undef memmove

static bool isGC = false;

/**
 * @brief NS Concrete values
 */
void * _NSConcreteStackBlock[32];
void * _NSConcreteMallocBlock[32];
void * _NSConcreteAutoBlock[32];
void * _NSConcreteFinalizingBlock[32];
void * _NSConcreteGlobalBlock[32];
void * _NSConcreteWeakBlockVariable[32];


/**
 * @brief enum BLOCK values
 */

enum {
    BLOCK_REFCOUNT_MASK =     (0xffff),
    BLOCK_NEEDS_FREE =        (1 << 24),
    BLOCK_HAS_COPY_DISPOSE =  (1 << 25),
    BLOCK_HAS_CTOR =          (1 << 26),
    BLOCK_IS_GC =             (1 << 27),
    BLOCK_IS_GLOBAL =         (1 << 28),
    BLOCK_HAS_DESCRIPTOR =    (1 << 29),
};

enum {
    BLOCK_FIELD_IS_OBJECT   =  3,
    BLOCK_FIELD_IS_BLOCK    =  7,
    BLOCK_FIELD_IS_BYREF    =  8,
    BLOCK_FIELD_IS_WEAK     = 16,
    BLOCK_BYREF_CALLER      = 128,
};

/**
 * @brief some block copy functions
 */
__unused static void *_Block_copy_class = _NSConcreteMallocBlock;
__unused static void *_Block_copy_finalizing_class = _NSConcreteMallocBlock;
__unused static int _Block_copy_flag = BLOCK_NEEDS_FREE;
static int _Byref_flag_initiale_value = BLOCK_NEEDS_FREE | 2;

static bool isGC = false;

/**
 * @brief latching incr int
 */
static int latching_incr_int(int *where) {
    while (1) {
        int old_value = *(volatile int *)where;
        if ((old_value & BLOCK_REFCOUNT_MASK) == BLOCK_REFCOUNT_MASK) {
            return BLOCK_REFCOUNT_MASK;
        }
        if ((*where = old_value+1) == old_value) {
            return old_value+1;
        }
    }
}

/**
 * @brief latching decr int
 */
static int latching_decr_int(int *where) {
    while (1) {
        int old_value = *(volatile int *)where;
        if ((old_value & BLOCK_REFCOUNT_MASK) == BLOCK_REFCOUNT_MASK) {
            return BLOCK_REFCOUNT_MASK;
        }
        if ((old_value & BLOCK_REFCOUNT_MASK) == 0) {
            return 0;
        }
        if ((*where = old_value-1) == old_value) {
            return old_value-1;
        }
    }
}

/**
 * @brief block descriptor
 */
struct block_descriptor {
    unsigned long int reserved;
    unsigned long int size;
    void (*copy)(void *dst, void *src);
    void (*dispose)(void *);
};

/**
 * @brief block layout
 */
struct block_layout {
    void *isa;
    int flags;
    int reserved;
    void (*invoke)(void *, ...);
    struct block_descriptor *descriptor;
};

/**
 * @brief block byref
 */
struct block_byref {
    void *isa;
    struct Block_byref *forwarding;
    int flags;
    int size;
    void (*byref_keep)(struct Block_byref *dst, struct Block_byref *src);
    void (*byref_destroy)(struct Block_byref *);
};

/**
 * @brief block byref header
 */
struct block_byref_header {
    void *isa;
    struct Block_byref *forwarding;
    int flags;
    int size;
};

/**
 * @brief block alloc default
 */
static void *_block_alloc_default(const unsigned size, const bool initialCountIsOne, const bool isObject) {
    return malloc(size);
}

/**
 * @brief block assign default
 */
static void _block_assign_default(void *value, void **destptr) {
    *destptr = value;
}

/**
 * @brief block set has refcount default
 */
static void _block_setHasRefcount_default(const void *ptr, const bool hasRefcount) {
    
}

/**
 * @brief block do nothing
 */
__unused static void _block_do_nothing(const void *aBlock) { }

/**
 * @brief block retain object default
 */
static void _block_retain_object_default(const void *ptr) {
    if (!ptr) return;
}

/**
 * @brief block reelase object default
 */
static void _block_release_object_default(const void *ptr) {
    if (!ptr) return;
}

/**
 * @brief block assign weak default
 */
static void _block_assign_weak_default(const void *ptr, void *dest) {
    *(long *)dest = (long)ptr;
}

/**
 * @brief block memmove default
 */
static void _block_memmove_default(void *dst, void *src, unsigned long size) {
    memmove(dst, src, (size_t)size);
}


static void *(*_block_allocator)(const unsigned long, const bool isOne, const bool isObject) = _block_alloc_default;
static void (*_block_deallocator)(const void *) = (void (*)(const void *))free;
static void (*_block_assign)(void *value, void **destptr) = _block_assign_default;
static void (*_block_setHasRefcount)(const void *ptr, const bool hasRefcount) = _block_setHasRefcount_default;
static void (*_block_retain_object)(const void *ptr) = _block_retain_object_default;
static void (*_block_release_object)(const void *ptr) = _block_release_object_default;
static void (*_block_assign_weak)(const void *dest, void *ptr) = _block_assign_weak_default;
static void (*_block_memmove)(void *dest, void *src, unsigned long size) = _block_memmove_default;

void _block_release(const void *arg) {
    struct block_layout *ablock = (struct block_layout *)arg;
    int32_t newCount;
    if (!ablock) return;
    newCount = latching_decr_int(&ablock->flags) & BLOCK_REFCOUNT_MASK;
    if (newCount > 0) return;

    if (ablock->flags & BLOCK_IS_GC) {
        _block_setHasRefcount(ablock, false);
    }
    else if (ablock->flags & BLOCK_NEEDS_FREE) {
        if (ablock->flags & BLOCK_HAS_COPY_DISPOSE)(*ablock->descriptor->dispose)(ablock);
        _block_deallocator(ablock);
    }
    else if (ablock->flags & BLOCK_IS_GLOBAL) {
        ;
    }
    else {
        printf("Block_release called upon a stack Block: %p, ignored\n", ablock);
    }
}

static void _block_destroy(const void *arg) {
    struct block_layout *ablock;
    if (!arg) return;
    ablock = (struct block_layout *)arg;
    if (ablock->flags & BLOCK_IS_GC) {
        return;
    }
    _block_release(ablock);
}

unsigned long int _block_size(void *arg) {
    return ((struct block_layout *)arg)->descriptor->size;
}

static void *_block_copy_internal(const void *arg, const int flags) {
    struct block_layout *ablock;
    const bool wantsOne = ((1 << 16) & flags) == (1 << 16);
    
    if (!arg) return NULL;
    
    ablock = (struct block_layout *)arg;
    if (ablock->flags & BLOCK_NEEDS_FREE) {
        latching_incr_int(&ablock->flags);
        return ablock;
    } else if (ablock->flags & BLOCK_IS_GC) {
        if (wantsOne && ((latching_incr_int(&ablock->flags) & BLOCK_REFCOUNT_MASK) == 1)) {
            _block_setHasRefcount(ablock, true);
        }
        return ablock;
    } else if (ablock->flags & BLOCK_IS_GLOBAL) {
        return ablock;
    }
    
    if (!isGC) {
        struct Block_layout *result = malloc(ablock->descriptor->size);
        if (!result) return (void *)0;
        memmove(result, ablock, ablock->descriptor->size);
        result->flags &= ~(BLOCK_REFCOUNT_MASK);
        result->flags |= BLOCK_NEEDS_FREE | 1;
        result->isa = _NSConcreteMallocBlock;
        if (result->flags & BLOCK_HAS_COPY_DISPOSE) {
            (*ablock->descriptor->copy)(result, ablock);
        }
        return result;
    } else {
        unsigned long int flags = ablock->flags;
        bool hasCTOR = (flags & BLOCK_HAS_CTOR) != 0;
        struct Block_layout *result = _Block_allocator(ablock->descriptor->size, wantsOne, hasCTOR);
        if (!result) return (void *)0;
        memmove(result, ablock, ablock->descriptor->size);
        flags &= ~(BLOCK_NEEDS_FREE|BLOCK_REFCOUNT_MASK);
        if (wantsOne)
            flags |= BLOCK_IS_GC | 1;
        else
            flags |= BLOCK_IS_GC;
        result->flags = (int)flags;
        if (flags & BLOCK_HAS_COPY_DISPOSE) {
            (*aBlock->descriptor->copy)(result, aBlock); 
        }
        if (hasCTOR) {
            result->isa = _NSConcreteFinalizingBlock;
        } else {
            result->isa = _NSConcreteAutoBlock;
        }
        return result;
    }
}

void *_Block_copy(const void *arg){
    return _Block_copy_internal(arg, (1 << 16));
}


static void _Block_byref_assign_copy(void *dest, const void *arg, const int flags) {
    struct Block_byref **destp = (struct Block_byref **) dest;
    struct Block_byref **src = (struct Block_byref *) arg;
    
    if (src->forwarding->flags & BLOCK_IS_GC) {
        ;
    }
    
    else if((src->forwarding->flags & BLOCK_REFCOUNT_MASK) == 0) {
        bool isWeak = ((flags & (BLOCK_FIELD_IS_BYREF|BLOCK_FIELD_IS_WEAK)) == (BLOCK_FIELD_IS_BYREF|BLOCK_FIELD_IS_WEAK));
        struct Block_byref *copy = (struct Block_byref *)_block_allocator(src->size, false, isWeak);
        copy->flags = src->flags;
        copy->forwarding = copy;
        copy->size = src->size;
        
        if (isWeak) {
            copy->isa = &_NSConcreteWeakBlockVariable;
        }
        if (src->flags & BLOCK_HAS_COPY_DISPOSE) {
            copy->byref_keep = src->byref_keep;
            copy->byref_destroy = src->byref_destroy;
            (*src->byref_keey)(copy, src);
        }
        else {
            _Block_memmove((void *)&copy->byref_keep, (void *)&src->byref_keep, src->size - sizeof(struct Block_byref_header));
        }
    }
    
    else if ((src->forwarding->flags & BLOCK_NEEDS_FREE) == BLOCK_NEEDS_FREE) {
        latching_incr_int(&src->forwarding->flags);
    }
    
    _block_assign(src->forwarding, (void**) destp);
}


