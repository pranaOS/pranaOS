/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <assert.h>
#include <libobjc/class.h>
#include <libobjc/memory.h>
#include <libobjc/module.h>
#include <libobjc/objc.h>
#include <libobjc/runtime.h>
#include <libobjc/selector.h>
#include <string.h>

struct class_node {
    const char* name;
    int length;
    Class cls;
};

static class_node class_table_storage[512];
static int class_table_next_free = 0;

static Class unresolved_classes[128];
static int unresolved_classes_next = 0;

void class_table_init()
{    
}

bool class_add(Class cls)
{
    Class fnd = class_table_find(cls->name);

    if (fnd) {
        return false;
    } else {
        static int next_class_num = 1;
        cls->set_number(next_class_num);
        cls->get_isa()->set_number(next_class_num);
        class_table_add(cls->name, cls);
        return true;
    }
}

static inline void class_disp_table_preinit(Class cls)
{
    cls->disp_table = DISPATCH_TABLE_NOT_INITIALIZED;
}

void class_resolve_all_unresolved()
{
    for (int i = 0; i < unresolved_classes_next; i++) {
        class_resolve_links(unresolved_classes[i]);
    }
}

Class objc_getClass(const char* name)
{
    Class cls = class_table_find(name);
    return cls;
}

OBJC_EXPORT Class objc_lookup_class(const char* name)
{
    return objc_getClass(name);
}

IMP class_get_implementation(Class cls, SEL sel)
{
    sel = sel_registerTypedName((char*) sel->types);
    Method method = class_lookup_method_in_hierarchy(cls, sel);

    if (!method) {
        return nil_method;
    }

    return method->method_imp;
}