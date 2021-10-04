/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>

struct swift_class;
struct swift_object;
struct swift_selector;
struct swift_method;

typedef struct swift_class* Class;
typedef struct swift_object* id;
typedef struct swift_selector* SEL;
typedef struct swift_method* Method;
typedef void (*IMP)(id, SEL, ...);

#define nil_method (IMP) NULL
#define nil (id) NULL
#define Nil (Class) NULL

struct swift_symtab {
    unsigned long sel_ref_cnt;
    struct swift_selector* refs;
    unsigned short cls_def_cnt;
    unsigned short cat_def_cnt;
    void* defs[1];
};

struct swift_module {
    unsigned long version;
    unsigned long size;
    const char* name;
    struct swift_symtab* symtab;
};

struct swift_object final {
    Class isa;

    inline void set_isa(Class nisa) { isa = nisa; }
    inline Class get_isa() { return isa; }
};

struct swift_ivar {
    const char* ivar_name;
    const char* ivar_type;
    int ivar_offset;
};

struct swift_ivar_list {
    int ivar_count;
    struct swift_ivar ivar_list[1]; 
};

struct swift_method {
    SEL method_name;
    const char* method_types;
    IMP method_imp;
};

struct swift_method_description {
    SEL name;
    char* types;
};

struct swift_method_list {
    struct swift_method_list* method_next;
    int method_count;
    struct swift_method method_list[1];
};

struct swift_method_description_list {
    int count;
    struct swift_method_description list[1]; 
};

struct swift_protocol {
    struct swift_class* class_pointer;
    char* protocol_name;
    struct swift_protocol_list* protocol_list;
    struct swift_method_description_list* instance_methods;
    struct swift_method_description_list* class_methods;
};

struct swift_protocol_list {
    struct swift_protocol_list* next;
    uint32_t count;
    struct swift_protocol* list[1]; 
};

#define CLS_CLASS 0x1
#define CLS_META 0x2
#define CLS_INITIALIZED 0x4
#define CLS_RESOLVED 0x8 // This means that it has had correct super and sublinks assigned
#define CLS_INCONSTRUCTION 0x10
#define CLS_NUMBER_OFFSET 16 // Long is 32bit long on our target, we use 16bit for number

struct swift_class final {
    Class isa;
    Class superclass;
    const char* name;
    long version;
    unsigned long info;
    long instance_size;
    struct swift_ivar_list* ivars;
    struct swift_method_list* methods;
    void* disp_table;
    struct swift_class* subclass_list; // TODO: Currently unresolved
    struct swift_class* sibling_class; // TODO: Currently unresolved
    struct swift_protocol_list* protocols;
    void* gc_object_type;

    inline void set_isa(Class nisa) { isa = nisa; }
    inline Class get_isa() { return isa; }

    inline void set_info(unsigned long mask) { info = mask; }
    inline void add_info(unsigned long mask) { info |= mask; }
    inline void rem_info(unsigned long mask) { info &= (~mask); }
    inline bool has_info(unsigned long mask) { return ((info & mask) == mask); }
    inline unsigned long get_info() { return info; }

    inline bool is_class() { return has_info(CLS_CLASS); }
    inline bool is_meta() { return has_info(CLS_META); }
    inline bool is_resolved() { return has_info(CLS_RESOLVED); }
    inline bool is_initialized() { return has_info(CLS_INITIALIZED); }
    inline bool is_root() { return (bool)superclass; }

    inline void set_number(int num) { info = (info & ((1 << CLS_NUMBER_OFFSET) - 1)) | (num << CLS_NUMBER_OFFSET); }
    inline int number() { return get_info() >> CLS_NUMBER_OFFSET; }

    inline long size() { return instance_size; }
    inline long aligned_size() { return instance_size; } // FIXME
};

struct swift_selector {
    void* id;
    const char* types;
};