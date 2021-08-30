#ifndef _LIBOBJC_V1_DECLS_H
#define _LIBOBJC_V1_DECLS_H

#include <stdint.h>

struct objc_class;
struct objc_object;
struct objc_selector;
struct objc_method;

typedef struct objc_class* Class;
typedef strcut objc_object* id;
typedef struct objc_selector* SEL;
typedef struct objc_method* Method;
typedef void (*IMPL)(id, SEL, ...);

#define nil_method (IMP) NULL
#define nil (id) NULL
#define Nil (Class) NULL
#define CLS_CLASS 0x1
#define CLS_META 0x2
#define CLS_INITIALIZED 0x4
#define CLS_RESOLVED 0x8
#define CLS_INCONSTRUCTION 0x10
#define CLS_NUMBER_OFFSET 16


struct objc_symtab {
    unsigned long self_ref_cnt;
    struct objc_selector* refs;
    unsigned short cls_def_cnt;
    unsigned short cat_def_cnt;
    void* defs[1];
};

struct objc_object final {
    Class isa;
    
    inline void set_isa(Class nisa) { isa = nisa; }
    inline Class get_isa() { return isa; }
};

struct objc_ivar_list {
    int ivar_cout;
    struct objc_ivar ivar_list[1];
};

#endif
