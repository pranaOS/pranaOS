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

struct block_descriptor {
    unsigned long int reserved;
    unsigned long int size;
    void (*copy)(void *dst, void *src);
    void (*dispose)(void *);
};

struct block_layout {
    void *isa;
    int flags;
    int reserved;
    void (*invoke)(void *, ...);
    struct block_descriptor *descriptor;
};

struct block_byref {
    void *isa;
    struct Block_byref *forwarding;
    int flags;
    int size;
    void (*byref_keep)(struct Block_byref *dst, struct Block_byref *src);
    void (*byref_destroy)(struct Block_byref *);
};

struct block_byref_header {
    void *isa;
    struct Block_byref *forwarding;
    int flags;
    int size;
};
