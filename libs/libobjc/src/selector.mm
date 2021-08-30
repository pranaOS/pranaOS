/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libobjc/memory.h>
#include <libobjc/objc.h>
#include <libobjc/runtime.h>
#include <libobjc/selector.h>
#include <string.h>

static struct objc_selector* selector_pool_start;
static struct objc_selector* selector_pool_next;

#define CONST_DATA true
#define VOLATILE_DATA false

static SEL selector_table_add(const char* name, const char* types, bool const_data)
{
    for (struct objc_selector* cur_sel = selector_pool_start; cur_sel != selector_pool_next; cur_sel++) {
        if (strcmp(name, (char*)cur_sel->id) == 0) {
            if (cur_sel->types == 0 || types == 0) {
                if (cur_sel->types == types) {
                    return (SEL)cur_sel;
                }
            } else {
                if (strcmp(types, cur_sel->types) == 0) {
                    return (SEL)cur_sel;
                }
            }
        }
    }

    SEL sel = (SEL)selector_pool_next++;
    if (const_data) {
        sel->id = (char*)name;
        sel->types = types;
    } else {
        int name_len = strlen(name);
        char* name_data = (char*)objc_malloc(name_len + 1);
        memcpy(name_data, name, name_len);
        name_data[name_len] = '\0';
        sel->id = name_data;
        sel->types = 0;

        if (types) {
            int types_len = strlen(types);
            char* types_data = (char*)objc_malloc(types_len + 1);
            memcpy(types_data, types, types_len);
            types_data[types_len] = '\0';
            sel->types = types_data;
        }
    }

    return sel;
}
